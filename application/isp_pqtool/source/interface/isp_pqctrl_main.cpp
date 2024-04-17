/*
 * INTERNAL USE ONLY
 *
 * Unpublished Work Copyright (C) 2013-2020 Synaptics Incorporated.
 * All rights reserved.
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
 */

////////////////////////////////////////////////////////////////////////////////
//! \file isp_main.cpp
//!
//! \brief This file launches server that launches ISP PQ Control
//!
//! Purpose:
//!     configures AMP ISP component to provide different PQ control functions
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////


#include "isppq.hpp"

int main(int argc, char **argv)
{
    startPQ(argc, argv);

    return 0;
}
