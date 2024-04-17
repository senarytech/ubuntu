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


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "OSAL_api.h"
#include "amp_client.h"
#include "amp_client_support.h"
#include "amp_component.h"
#include "isl/amp_logger.h"
#include "isl/amp_buf_desc.h"


extern void ndi_avin(int argc, char **argv, AMP_FACTORY factory);

static char *client_argv[] =
    { "client", "iiop:1.0//127.0.0.1:999/AMP::FACTORY/factory" };



static AMP_LIB gAMPLib;

static void main_diaplay_usage(void)
{
    printf
    (
"Usage: ampclient_ndi \n"
"\n"
    );
}

int main(int argc, char **argv)
{
    HRESULT result = 0xFFFFFFFF;
    AMP_FACTORY hFactory;

    MV_OSAL_Init();

    AMPLOGD("NDI Start!\n");
    /* Get factory */
    result = AMP_Initialize(2, client_argv, &hFactory);
    assert(result == SUCCESS);
    result = AMP_GetFactory(&hFactory);
    assert(result == SUCCESS);

    printf("Senary NDI start ... \n");


    ndi_avin(argc, argv, hFactory);

    AMP_Deinitialize();
    AMPLOGD("End!\n");
    MV_OSAL_Exit();

    return result;
}

