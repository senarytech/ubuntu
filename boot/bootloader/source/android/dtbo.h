/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
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
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
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
#ifndef __DTBO_H
#define __DTBO_H

#define DT_TABLE_MAGIC 0xd7b7ab1e

struct dt_table_header {
  uint32_t magic;             // DT_TABLE_MAGIC
  uint32_t total_size;        // includes dt_table_header + all dt_table_entry
                              // and all dtb/dtbo
  uint32_t header_size;       // sizeof(dt_table_header)

  uint32_t dt_entry_size;     // sizeof(dt_table_entry)
  uint32_t dt_entry_count;    // number of dt_table_entry
  uint32_t dt_entries_offset; // offset to the first dt_table_entry
                              // from head of dt_table_header

  uint32_t page_size;         // flash page size we assume
  uint32_t version;           // DTBO image version, the current version is 0.
                              // The version will be incremented when the
                              // dt_table_header struct is updated.
};

struct dt_table_entry {
  uint32_t dt_size;
  uint32_t dt_offset;         // offset from head of dt_table_header

  uint32_t id;                // optional, must be zero if unused
  uint32_t rev;               // optional, must be zero if unused
  uint32_t custom[4];         // optional, must be zero if unused
};

#endif
