/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated (“Synaptics”). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS,” AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS’ TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY
*/
#include <linux/videodev2.h>
#include <sys/time.h>
#include <poll.h>

#include "v4l2_types.h"
#include "v4l2_buffer.h"
#include "v4l2_m2m.h"
#include "v4l2_stream.h"
#include "v4l2_misc.h"
#include "std_list.h"

static unsigned sev_pos(V4L2SubscribedEvent *sev, unsigned idx)
{
    idx += sev->first;
    return idx >= sev->elems ? idx - sev->elems : idx;
}

static V4L2SubscribedEvent* get_v4l2_event_subscribed(V4L2EventCtx *ectx,
                                            int type, int id)
{
    std_list_node *pNode=NULL;
    V4L2SubscribedEvent* sev=NULL;
    V4L2SubscribedEvent* tmp=NULL;


    while (1) {
        std_list_iter(ectx->subscribed, &pNode);
        if(pNode == NULL)
            break;
        tmp = (V4L2SubscribedEvent*)(pNode->pData);
        if((tmp->type == type) && (tmp->id == id)) {
            sev = tmp;
            break;
        } else {
            continue;
        }
    }
    return sev;
}

static int _v4l2_event_dequeue(V4L2EventCtx *ectx, struct v4l2_event *event)
{
    V4L2Event *evt;
    unsigned long flags;

    if (list_is_empty(ectx->event_queued)) {
        return ERROR_ENOENT;
    }

    if(ectx->num_available == 0) {
        V4L2LOGE("!!error, num_available should > 0");
    }

    std_list_pop_front(ectx->event_queued, (void*)&evt);

    AmpMemcpy(event, &evt->event, sizeof(struct v4l2_event));

    ectx->num_available--;
    evt->event.pending = ectx->num_available;
    evt->sev->first = sev_pos(evt->sev, 1);
    evt->sev->in_use--;

    V4L2LOGH("deque event: type %d, u %d\n", event->type,
                event->u.src_change.changes);
    return 0;
}

int v4l2_event_dequeue(void *fh, struct v4l2_event *event,
                       int nonblocking)
{
    int ret = 0;
    M2MCTX(fh)

    if(nonblocking)
        return _v4l2_event_dequeue(&ctx->event, event);
#if 0 // poll service is wake up by event, this seems can be delete.
    do {
        ret = MV_OSAL_Sem_Wait(ctx->event.event_sem);
        if(ret != V4L2_SUCCESS)
            break;

        ret = _v4l2_event_dequeue(&ctx->event, event);
    } while(ret == ERROR_ENOENT);
#endif
    return ret;
}

void v4l2_event_queue(void *fh, const struct v4l2_event *ev)
{
    M2MCTX(fh)
    V4L2SubscribedEvent *sev;
    V4L2Event *evt;
    bool copy_payload = true;
    struct timespec timestamp;
    V4L2EventCtx *ectx = &ctx->event;


    sev = get_v4l2_event_subscribed(ectx, ev->type, ev->id);
    if(sev == NULL)
        return;

    ectx->sequence++; //sequence num ++
    if(sev->in_use == sev->elems) {
        evt = sev->events + sev_pos(sev, 0);
        sev->in_use--;
        sev->first = sev_pos(sev, 1);
        ectx->num_available--;
        if(sev->elems == 1) {
            if(sev->ops && sev->ops->replace) {
                sev->ops->replace(&evt->event, ev);
                copy_payload = false;
            }
        } else if (sev->ops && sev->ops->merge) {
            V4L2Event *sec_evt = sev->events + sev_pos(sev, 0);
            sev->ops->merge(&evt->event, &sec_evt->event);
        }
    }
    UINT32 idx = sev_pos(sev, sev->in_use);
    evt = &sev->events[idx];
    evt->event.type = ev->type;
    evt->event.id = ev->id;

    if(copy_payload) {
        evt->event.u = ev->u;
    }
    clock_gettime(CLOCK_MONOTONIC, &timestamp);
    evt->event.timestamp = timestamp;
    evt->event.sequence = ectx->sequence;
    sev->in_use ++;

    //push to queued event list
    std_list_push_back(ectx->event_queued, evt);
    ectx->num_available++;
    V4L2LOGD("queue %p event %p, type %d, evt->sev %p\n",
                sev, evt, evt->event.type, evt->sev);
    /*
     *  if Poll() is called, need to notify client when there is new event coming
     *  so that client can call DQEVENT().
     */
    ctx->recieve_event |= POLLPRI;
    PUT_POLL_SEM(fh);
}

int v4l2_event_pending(void *fh)
{
    M2MCTX(fh)
    return ctx->event.num_available;
}


int v4l2_event_subscribe(void *fh,
       const struct v4l2_event_subscription *sub,
       unsigned int elems,
       const struct v4l2_subscribed_event_ops *ops)
{
    int ret = 0;
    int i;
    V4L2SubscribedEvent *sev, *found_sev;
    V4L2Event *pEvt;

    M2MCTX(fh);

    if(sub->type == V4L2_EVENT_ALL)
        return ERROR_EINVAL;

    elems = (elems < 1)? 1 : elems;

    sev = (V4L2SubscribedEvent*)malloc(sizeof(V4L2SubscribedEvent));
    memset(sev, 0, sizeof(V4L2SubscribedEvent));

    sev->events = (V4L2Event *)malloc(sizeof(V4L2Event) * elems);
    memset(sev->events, 0, sizeof(V4L2Event) * elems);

    for(i=0; i<elems; i++) {
        pEvt = &sev->events[i];
        pEvt->sev = sev;
    }
    sev->type = sub->type;
    sev->id = sub->id;
    sev->fh = fh;
    sev->ops = ops;
    sev->elems = elems;

    //TODO:  lock list
    found_sev = get_v4l2_event_subscribed(&ctx->event, sub->type, sub->id);
    if(!found_sev) {
        std_list_push_back(ctx->event.subscribed, (void *)sev);
    }

    V4L2LOGH("subscribe sev %p event %p type %d, id %d\n",
                sev, sev->events, sev->type, sev->id);
    //TODO: unlock list
    if(found_sev) {
        V4L2LOGD("found old sev %p event %p\n", sev, sev->events);
        free(sev->events);
        free(sev);
    }
    return ret;
}

static void _v4l2_event_unsubscribe(V4L2EventCtx *ectx,
                                V4L2SubscribedEvent *sev)
{
    std_list_node *pNode;
    V4L2Event *evt;
    V4L2SubscribedEvent *sevt;
    std_list *list;

    //TODO: list lock??

    //remove node in queued list
    list = ectx->event_queued;
    while (1) {
        std_list_iter(list, &pNode);
        if(pNode == NULL)
            break;
        evt = (V4L2Event*)(pNode->pData);
        if(evt && evt->sev == sev) {
           std_list_remove(list, pNode);
           ectx->num_available--;
        }
    }

    //revome node in suscibed list
    pNode = std_list_search(ectx->subscribed, (void*)sev);
    if(pNode) {
        std_list_remove(ectx->subscribed, pNode);
    }

    return;
}

void v4l2_event_unsubscribe_all(void *fh)
{
    M2MCTX(fh)
    V4L2EventCtx *ectx = &ctx->event;

    struct v4l2_event_subscription sub;
    V4L2SubscribedEvent *sev;
    unsigned long flags;

    do {
        sev = NULL;
        if (!list_is_empty(ectx->subscribed)) {
            std_list_pop_front(ectx->subscribed, (void**)&sev);
        }
        if (sev) {
            sub.type = sev->type;
            sub.id = sev->id;
            v4l2_event_unsubscribe(ectx, &sub);
        }
    } while (sev);

    return;
}

int v4l2_event_unsubscribe(void *fh,
         const struct v4l2_event_subscription *sub)
{
    V4L2SubscribedEvent *sev;
    unsigned long flags;
    M2MCTX(fh)
    V4L2EventCtx* ectx = &ctx->event;

    if (sub->type == V4L2_EVENT_ALL) {
        v4l2_event_unsubscribe_all(ectx);
        return 0;
    }

    //TODO
   // mutex_lock(ectx->subscribe_lock);
    sev = get_v4l2_event_subscribed(ectx, sub->type, sub->id);
    if(sev == NULL) {
        return ERROR_ENOENT;
    }

    _v4l2_event_unsubscribe(ectx, sev);

    if (sev && sev->ops && sev->ops->del)
        sev->ops->del(sev);

    //mutex_unlock(ectx->subscribe_lock);
    if(sev->events) free(sev->events);
    if(sev) free(sev);

    return 0;
}

static void v4l2_event_src_replace(struct v4l2_event *old,
                const struct v4l2_event *new)
{
    UINT32 old_changes = old->u.src_change.changes;

    old->u.src_change = new->u.src_change;
    old->u.src_change.changes |= old_changes;
}

static void v4l2_event_src_merge(const struct v4l2_event *old,
                struct v4l2_event *new)
{
    new->u.src_change.changes |= old->u.src_change.changes;
}

static const struct v4l2_subscribed_event_ops v4l2_event_src_ch_ops = {
    .replace = v4l2_event_src_replace,
    .merge = v4l2_event_src_merge,
};

int v4l2_src_change_event_subscribe(void *fh,
            const struct v4l2_event_subscription *sub)
{
    if(sub->type == V4L2_EVENT_SOURCE_CHANGE) {
        return v4l2_event_subscribe(fh, sub, 0, &v4l2_event_src_ch_ops);
    }

    return ERROR_EINVAL;

}

int v4l2_event_init(V4L2EventCtx *ectx)
{
    ectx->subscribed = std_list_ctor(NULL, NULL);
    ectx->event_queued = std_list_ctor(NULL, NULL);
    //MV_OSAL_Sem_Create(&ectx->event_sem, 0);
    ectx->num_available = 0;
    ectx->sequence = 0;
    return 0;
}

int v4l2_event_deinit(V4L2EventCtx *ectx)
{
    std_list_dtor(ectx->subscribed);
    std_list_dtor(ectx->event_queued);
   // MV_OSAL_Sem_Destroy(&ectx->event_sem);
    return 0;
}

