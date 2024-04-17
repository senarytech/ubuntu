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

#ifndef __BOARD_MODEL_H__
#define __BOARD_MODEL_H__

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef TRUSTZONE
#include "macros.h"
#else
#ifndef STRINGIFY
# define __STRINGIFY(x) #x
# define STRINGIFY(x)   __STRINGIFY(x)
#endif
#endif

__BEGIN_DECLS

#define BOARD_MODEL_KEY     "board_model="
#define SZ_BOARD_MODEL      (sizeof(BOARD_MODEL_KEY)-1)
#define BOARD_MODEL_NAME(x) BOARD_MODEL_KEY STRINGIFY(x)
#define MAX_CMDLINE_SIZE    512



static inline int get_board_model (void)
{
    int fd, ret;
    char buf[MAX_CMDLINE_SIZE+1];
    char *p = (char *)BOARD_MODEL_NAME(0);

    fd = open ("/proc/cmdline", O_RDONLY);
    if (fd > 0)
    {
        ret = read (fd, buf, MAX_CMDLINE_SIZE);
        if (ret > 0)
        {
            char *t;
            /* make sure NULL terminated */
            buf[MAX_CMDLINE_SIZE] = 0;
            t = strstr (buf, BOARD_MODEL_KEY);
            if (t) p = t;
        }

        close (fd);
    }
    return (int) strtoul (p+SZ_BOARD_MODEL, NULL, 0);
}

__END_DECLS

#endif
