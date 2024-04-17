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
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#ifndef __DB_SHARE_H__
#define __DB_SHARE_H__
#if USGIN_MYSQL
#include <mysql/my_global.h>
#include <mysql/mysql.h>
static char *opt_host_name = "10.38.54.227";      /* server host (default=localhost) */
static char *opt_user_name = "root";      /* username (default=login name) */
static char *opt_password = "123456";       /* password (default=none) */
static unsigned int opt_port_num = 3306;   /* port number (use built-in value) */
static char *opt_socket_name = NULL;    /* socket name (use built-in value) */
static char *opt_db_name = "kms";        /* database name (default=none) */
static unsigned int opt_flags = 0;      /* connection flags (none) */
static MYSQL *conn;                     /* pointer to connection handler */
#endif

//KMS Table Lists
#define KMS_TABLE_CONFIGURATION "kms_configure"
#define KMS_TABLE_OTP_KEY   "kms_otp_keys"
#define KMS_TABLE_IMG_KEY   "kms_image_keys"
#define KMS_TABLE_CUSTOMER_IMG_KEY  "kms_customer_image_keys"
#define KMS_TABLE_CUSTOMER_DEV_KEY  "kms_customer_device_keys"
#define KMS_TABLE_AACS_KEY  "kms_aacs_key"
#define KMS_TABLE_AACS_CERT "kms_aacs_cert"
#define KMS_TABLE_BDPLUS_DEV_KEY_CERT   "kms_bdplus_dev_key_cert"
#define KMS_TABLE_BDPLUS_MODVER_KEY_CERT    "kms_bdplus_modver_key_cert"
#define KMS_TABLE_HDCP_KEY  "kms_hdcp_key"

//function sets
int db_init();
int db_store(char * kms_table, char * data, unsigned int datalen);
int db_retrieve(char * kms_table, int id, char *data, unsigned int * datalen);
int db_close();
int db_request_newid();

#endif
