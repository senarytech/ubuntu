/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2017 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
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
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#include "amp_malloc.h"
#include "stat.h"

#ifdef ANDROID
#define DEFAULT_DUMP_PATH  "/data/"
#else // ANDROID
#define DEFAULT_DUMP_PATH  "/mnt/"
#endif // ANDROID

#define STAT_BUFFER_LEN         256
#define DEFAULT_LOG_PATH        DEFAULT_DUMP_PATH"perf.log"

static double diffTimeMS(struct timeval *a, struct timeval *b)
{
    return (a->tv_sec - b->tv_sec) * 1000.0 +
           (a->tv_usec - b->tv_usec) / (1000.0);
}

static void stat_show(Stat_T *p, char *s, int len, int frameCount,
                      double costTime)
{
    snprintf(s, len, "frame %06d cost %5.2fms, "
             "avgfps=%6.2f minfps=%6.2f, maxfps=%6.2f, "
             "encodeOneFrameTime=%5.2f, encodeOneFrameTimeAvg=%5.2fms, "
             "encodeOneFrameTimeMin=%5.2fms, encodeOneFrameTimeMax=%5.2fms\n",
             frameCount, costTime,
             p->fAvgFPS, p->fMinFPS, p->fMaxFPS,
             p->fEncOneFrameTime, p->fEncOneFrameTimeAvg,
             p->fEncOneFrameTimeMin, p->fEncOneFrameTimeMax);
}

void stat_tic(Stat_T *p)
{
    struct timeval currentTime;
    double costTime;

    if (p->stSeqList == NULL) {
        p->stSeqList = (SEQ_LIST_ST *)malloc(sizeof(SEQ_LIST_ST));
        memset(p->stSeqList, 0x0, sizeof(SEQ_LIST_ST));
        gettimeofday(&p->stSeqList->seq, NULL);

        p->seqHead = p->stSeqList;
        p->stSeqList->prev = NULL;
        p->stSeqList->next = NULL;
        p->seqTail = p->stSeqList;
    } else {
        p->seqTail->next = (SEQ_LIST_ST *)malloc(sizeof(SEQ_LIST_ST));
        memset(p->seqTail->next, 0x0, sizeof(SEQ_LIST_ST));
        gettimeofday(&p->seqTail->next->seq, NULL);

        p->seqTail->next->prev = p->seqTail;
        p->seqTail = p->seqTail->next;
        p->seqTail->next = NULL;
    }

    if (p->uiIdx == 0) {
        costTime = 0;
        goto exit;
    } else {
        costTime = diffTimeMS(&p->seqTail->seq, &p->seqTail->prev->seq);
    }

    p->fEncTotalTime += costTime;
    p->fEncOneFrameTime = costTime;
    p->fEncOneFrameTimeAvg = p->fEncTotalTime / p->uiIdx;

    if (costTime > p->fEncOneFrameTimeMax) {
        p->fEncOneFrameTimeMax = costTime;
    }
    if (p->fEncOneFrameTimeMin == 0.0 || costTime < p->fEncOneFrameTimeMin) {
        p->fEncOneFrameTimeMin = costTime;
    }

    memset(&currentTime, 0x0, sizeof(struct timeval));
    gettimeofday(&currentTime, NULL);

    if (diffTimeMS(&currentTime, &p->startTime) >= 1000) {
        // update fMinFPS and fMaxFPS per second
        double tmpFPS = (p->uiIdx - p->uiReadIdx) /
            diffTimeMS(&currentTime, &p->startTime) * 1000;

            if (tmpFPS > p->fMaxFPS) {
                p->fMaxFPS = tmpFPS;
            }
            if (p->fMinFPS == 0.0 || tmpFPS < p->fMinFPS) {
                p->fMinFPS = tmpFPS;
            }

        gettimeofday(&p->startTime, NULL);
        p->uiReadIdx = p->uiIdx;
    }

    p->fAvgFPS = 1000.0 / p->fEncOneFrameTimeAvg;

    char s[STAT_BUFFER_LEN] = {0};
    // record log in background
    stat_show(p, s, STAT_BUFFER_LEN, p->uiIdx, costTime);
    fwrite(s, strlen(s), sizeof(char), p->fp);
    fflush(p->fp);

exit:
    p->uiIdx++;
}

bool stat_summary(Stat_T *p)
{
    char s[STAT_BUFFER_LEN] = {0};

    snprintf(s, STAT_BUFFER_LEN, "Total frames     :  %d\n"
                                 "Average fps      :  %-6.2f\n",
                                 p->uiIdx, p->fAvgFPS);

    printf("Summary:\n");
    printf("%s", s);
    return true;
}

Stat_T *stat_create(const char *log_name)
{
    Stat_T *p = (Stat_T *)AmpMalloc(sizeof(Stat_T));
    memset(p, 0x0, sizeof(p));

    if (log_name != NULL && strlen(log_name) != 0) {
        memcpy(p->log_name, log_name, strlen(log_name));
    } else {
        memcpy(p->log_name, DEFAULT_LOG_PATH, strlen(DEFAULT_LOG_PATH));
    }

    gettimeofday(&p->startTime, NULL);
    p->fp = fopen(p->log_name, "w");
    if (p->fp == NULL) {
        printf("open perf log failed");
    }
    stat_tic(p);
    return p;
}

void stat_destroy(Stat_T *p)
{
    fclose(p->fp);
    while (p->seqHead != NULL) {
        struct SEQ_LIST *tmp = p->seqHead->next;
        free(p->seqHead);
        p->seqHead = tmp;
    }
    free(p);
}
