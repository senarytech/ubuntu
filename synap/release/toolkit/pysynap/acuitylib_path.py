#!/usr/bin/env python3
#
# NDA AND NEED-TO-KNOW REQUIRED
#
# Copyright (C) 2013-2020 Synaptics Incorporated. All rights reserved.
#
# This file contains information that is proprietary to Synaptics
# Incorporated ("Synaptics"). The holder of this file shall treat all
# information contained herein as confidential, shall use the
# information only for its intended purpose, and shall not duplicate,
# disclose, or disseminate any of this information in any manner
# unless Synaptics has otherwise provided express, written
# permission.
#
# Use of the materials may require a license of intellectual property
# from a third party or from Synaptics. This file conveys no express
# or implied licenses to any intellectual property rights belonging
# to Synaptics.
#
# INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS", AND
# SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
# INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
# INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
# CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
# OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
# BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
# COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
# DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
# TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
#

import os
import sys
import pathlib

toolkit_dir = pathlib.Path(__file__).parent.parent.absolute()
internal_tree = False
# check if pysynap is run from the source tree
vsi_acuity_dir = f"{toolkit_dir}/../../external/vsi_acuity"
if os.path.exists(f"{vsi_acuity_dir}/acuitylib/__init__.py"):
    sys.path.append(os.path.abspath(vsi_acuity_dir))
    internal_tree = True
else:
    # pysynap is run within install or vssdk tree, make sure that acuitylib is present
    acuitylib_init = f"{toolkit_dir}/acuitylib/__init__.py"
    if not os.path.exists(acuitylib_init):
        raise Exception("Cannot find %s " % acuitylib_init)
