/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright (C) 2013-2022 Synaptics Incorporated. All rights reserved.
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

#pragma once

#include "vsi_nn_types.h"
#include "vsi_nn_ops.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _synap_op_move_local_data_t
{
   int32_t placeholder;
} synap_op_move_local_data_t;


typedef struct _synap_op_move_param
{
    struct _synap_op_move_local_data_t* local;

    // Add parameters here
} synap_op_move_param;

#define MOVE_INPUT_NUM  (1)
#define MOVE_OUTPUT_NUM (1)

vsi_status synap_op_move_compute
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    );

vsi_bool synap_op_move_check
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    );

vsi_bool synap_op_move_setup
    (
    vsi_nn_node_t * self,
    vsi_nn_tensor_t ** inputs,
    vsi_nn_tensor_t ** outputs
    );

vsi_status synap_op_move_init
    (
    vsi_nn_node_t* self
    );

vsi_status synap_op_move_deinit
    (
    vsi_nn_node_t* self
    );

static vsi_nn_op_proc_t synap_op_move = {
    .init = synap_op_move_init,
    .compute = synap_op_move_compute,
    .deinit = synap_op_move_deinit,
    .check = synap_op_move_check,
    .setup = synap_op_move_setup,
    .optimize = NULL,
    .input_num = MOVE_INPUT_NUM,
    .output_num = MOVE_OUTPUT_NUM,
    .name = "SyNAP_MOVE"
};


#ifdef __cplusplus
}
#endif
