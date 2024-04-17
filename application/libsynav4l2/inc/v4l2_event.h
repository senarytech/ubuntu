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
#ifndef SYNA_V4L2_EVENT_H__
#define SYNA_V4L2_EVENT_H__

#include <linux/videodev2.h>
#include "v4l2_types.h"
#include "v4l2_buffer.h"
#include "v4l2_m2m.h"
#include "v4l2_stream.h"

struct v4l2_subscribed_event_ops;
struct _v4l2_subscribed_event_t;


typedef struct _v4l2_event_ {
    struct _v4l2_subscribed_event_t *sev;
    struct v4l2_event event;
}V4L2Event;

typedef struct _v4l2_subscribed_event_t {
    UINT32 type;
    UINT32 id;
    UINT32 flags;
    UINT32 elems;
    UINT32 first;
    UINT32 in_use;
    void *fh;
    const struct v4l2_subscribed_event_ops *ops;
    V4L2Event *events;
} V4L2SubscribedEvent;


typedef struct _v4l2_event_ctx {
    std_list *subscribed;       //sbuscibed event list
    std_list *event_queued;    //queued event list
    UINT32 num_available;          //queued event number
    UINT32 sequence;               //event sequence
    //MV_OSAL_HANDLE_SEM_t event_sem;
}V4L2EventCtx;

/**
 * struct v4l2_subscribed_event_ops - Subscribed event operations.
 *
 * @add:  Optional callback, called when a new listener is added
 * @del:  Optional callback, called when a listener stops listening
 * @replace:  Optional callback that can replace event 'old' with event 'new'.
 * @merge:  Optional callback that can merge event 'old' into event 'new'.
 */
struct v4l2_subscribed_event_ops {
  int  (*add)(V4L2SubscribedEvent *sev, unsigned int elems);
  void (*del)(V4L2SubscribedEvent *sev);
  void (*replace)(struct v4l2_event *old, const struct v4l2_event *new);
  void (*merge)(const struct v4l2_event *old, struct v4l2_event *new);
};

/**
* v4l2_event_dequeue - Dequeue events from video device.
*
* @fh: pointer to m2m ctx
* @event: pointer to struct v4l2_event
* @nonblocking: if not zero, waits for an event to arrive
*/
int v4l2_event_dequeue(void *fh, struct v4l2_event *event,
                       int nonblocking);
/**
 * v4l2_event_queue - Queue events to video device.
 *
 * @fh: pointer to m2m ctx
 * @ev: pointer to &struct v4l2_event
 *
 * The event will be queued for all &struct v4l2_fh file handlers.
 *
 * .. note::
 *    The driver's only responsibility is to fill in the type and the data
 *    fields.The other fields will be filled in by  V4L2.
 */
void v4l2_event_queue(void *fh, const struct v4l2_event *ev);
/**
 * v4l2_event_pending - Check if an event is available
 *
 * @fh: pointer to m2m ctx
 *
 * Returns the number of pending events.
 */
int v4l2_event_pending(void *fh);

/**
 * v4l2_event_subscribe - Subscribes to an event
 *
 * @fh: pointer to m2m ctx
 * @sub: pointer to &struct v4l2_event_subscription
 * @elems: size of the events queue
 * @ops: pointer to &v4l2_subscribed_event_ops
 *
 * .. note::
 *
 *    if @elems is zero, the framework will fill in a default value,
 *    with is currently 1 element.
 */
int v4l2_event_subscribe(void *fh,
       const struct v4l2_event_subscription *sub,
       unsigned int elems,
       const struct v4l2_subscribed_event_ops *ops);
/**
 * v4l2_event_unsubscribe - Unsubscribes to an event
 *
 * @fh: pointer to m2m ctx
 * @sub: pointer to &struct v4l2_event_subscription
 */
int v4l2_event_unsubscribe(void *fh,
         const struct v4l2_event_subscription *sub);
/**
 * v4l2_event_unsubscribe_all - Unsubscribes to all events
 *
 * @fh: pointer to m2m ctx
 */
void v4l2_event_unsubscribe_all(void *fh);
/**
 * v4l2_src_change_event_subscribe - helper function that calls
 *  v4l2_event_subscribe() if the event is %V4L2_EVENT_SOURCE_CHANGE.
 *
 * @fh: pointer to m2m ctx
 * @sub: pointer to &struct v4l2_event_subscription
 */
int v4l2_src_change_event_subscribe(void *fh,
            const struct v4l2_event_subscription *sub);

/**
 * v4l2_event_init  init event context
 *
 * @ectx: pointer to event ctx
 */
int v4l2_event_init(V4L2EventCtx *ectx);

/**
 * v4l2_event_init  deinit event context
 *
 * @ectx: pointer to event ctx
 */
int v4l2_event_deinit(V4L2EventCtx *ectx);


#endif /* SYNA_V4L2_EVENT_H */


