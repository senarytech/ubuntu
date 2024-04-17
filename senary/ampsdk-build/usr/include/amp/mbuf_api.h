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

////////////////////////////////////////////////////////////////////////////////
//! \file mbuf_api.h
//!
//! \brief This file define all common data structure, enum and API for mbuf
//!        management.
//!
//! Purpose:
//!
//!
//! Version, Date and Author :
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////
#ifndef __OSAL_MBUF_H__
#define __OSAL_MBUF_H__

///////////////////////////////////////////////////////////////////////////////
//! [Part1] Header file include
///////////////////////////////////////////////////////////////////////////////
#include <sys/types.h>
#include <sys/un.h>

///////////////////////////////////////////////////////////////////////////////
//! [Part2] Declaration of external variables or functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//! [Part3] Local macros, type definitions
///////////////////////////////////////////////////////////////////////////////

#define MBUF_ADDR_MAX 32
#define UNIX_SOCKET_PATH_MAX 108

#if MBUF_ADDR_MAX > UNIX_SOCKET_PATH_MAX
#error "Please reduce MBUF_ADDR_MAX"
#endif

#ifndef _typedef___HRESULT
#define _typedef___HRESULT
typedef signed int HRESULT;
#endif /* _typedef___HRESULT */

/**
 * Private data allocated and managed by mbuf implementation.
 */
typedef struct MV_MBUF_HANDLE_internal_s *MV_MBUF_HANDLE_internal_ptr;

/**
 * Structure to represent a mbuf
 */
typedef struct {
	char address[MBUF_ADDR_MAX];
	MV_MBUF_HANDLE_internal_ptr pPrivate;
} MV_MBUF_HANDLE;

typedef enum {
    MV_MBUF_DIRECTION_READ,
    MV_MBUF_DIRECTION_WRITE
} MV_MBUF_DIRECTION;

#define  MV_MBUF_OK		(0)   /*No error occurred*/
#define  MV_MBUF_ERR_NOMEM	(-10) /*No memory for create mbuf*/
#define  MV_MBUF_ERR_NORES	(-11) /*No resource to create mbuf*/
#define  MV_MBUF_ERR_EXISTS	(-12) /*The address was provided, but it exist already*/
#define  MV_MBUF_ERR_NOTFOUND	(-13) /*No address could be found*/
#define  MV_MBUF_ERR_PEERGONE	(-14) /*Funcion returned because the mbuf was closed or destroyed on the remote side*/
#define  MV_MBUF_ERR_CLOSED	(-15) /*Function returned because the mbuf was closed or destroyed in this process*/
#define  MV_MBUF_ERR_MISMATCH	(-16) /*Write() attempted on a mbuf handle opened or created for reading*/
#define  MV_MBUF_ERR_BADPARAM	(-17) /*Either buf pointer or phMbuf handle is NULL*/
#define  MV_MBUF_ERR_TIMEOUT	(-18) /*Time out of get data*/


///////////////////////////////////////////////////////////////////////////////
//! [Part4] Declaration of local/global variables
///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////////////////
//! [Part5] Declarations of local static functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! [Part6] Global functions
///////////////////////////////////////////////////////////////////////////////


/** \fn  HRESULT MV_MBUF_Init(void)
 *
 *  \brief   This function is used to init mbuf, it will be call by MV_OSAL_Init()
 *
 *
 * \retval MV_MBUF_OK              init mbuf succeeded
 *
 * \retval MV_MBUF_ERR_NOMEM         no memory for init mbuf.
 *
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Init(void);


/** \fn  void MV_MBUF_Exit(void)
 *
 *  \brief   This function is used to mbuf exit, it will be call by MV_OSAL_Exit()
 *
 *
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
void MV_MBUF_Exit(void);


/** \fn  HRESULT MV_MBUF_Create(
 *		size_t uSize,
 *		MV_MBUF_HANDLE *phMbuf);
 *
 *  \brief   This function is used to create a mbuf to send or receive data
 *           sent by another process or thread.
 *
 *           mbuf is only used for linux user space interprocess
 *           communication, and this memory is implement
 *           by linux system memory, it is not physically
 *           contiguous, so it can't be used for DMA.
 *
 *           mbuf used for two user space processes communication,
 *           one process will call MV_MBUF_Create() to get a share
 *           memory buffer, and this process will get a mbuf's
 *           handle.
 *
 *           phMbuf should be initialized to zero. Optionally an
 *           address can be placed in phMbuf->address.
 *
 *           if phMbuf->name is an empty string, this function will
 *           auto create a globally unique address name as mbuf
 *           address and write it there. If an address is provided as
 *           a null-terminated string in phMbuf->address, this will be
 *           the address of the new mbuf. If the address is already
 *           the address of an existing mbuf, it is an error. The
 *           remote side can use MV_MBUF_Open() specifying the same
 *           address to create a connection to this mbuf.
 *
 *           The direction of the mbuf is read.
 *
 *           Note:   A handle from MV_MBUF_Create() only can be
 *                      freed with MV_MBUF_Destroy().
 *
 * \param[in]  uSize  The mbuf memory size to be allocated
 *
 * \param[inout]  phMbuf The mbuf structure with communication address.
 *
 * \retval MV_MBUF_OK                create mbuf succeeded
 *
 * \retval MV_MBUF_ERR_NOMEM         no memory for create mbuf.
 * \retval MV_MBUF_ERR_NORES         No resource to create mbuf (system resource other than memory).
 * \retval MV_MBUF_ERR_BADPARAM      phMbuf is NULL
 * \retval MV_MBUF_ERR_EXISTS        The address was provided, but it exist already
 *
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Create(
		size_t uSize,
		MV_MBUF_HANDLE *phMbuf);


/** \fn  HRESULT MV_MBUF_Destroy(
 *		MV_MBUF_HANDLE *phMbuf);
 *
 *  \brief This function used to destroy mbuf with a mbuf handle
 *
 *           MV_MBUF_Destroy() is paired with MV_MBUF_Create().
 *
 *           When an mbuf is destroyed, any blocking calls on it will
 *           wake up and return an error code. Remote peers (peers
 *           that used MV_MBUF_Open()) will be woken up by
 *           MV_MBUF_ERR_PEERGONE while local peers (peers that used
 *           MV_MBUF_Create()) will be woken up with MV_MBUF_ERR_CLOSED.
 *
 *           mbuf used for two user space processes communication,
 *           which process call MV_MBUF_Create() to got a mbuf handle,
 *           it must call MV_MBUF_Destroy() to free this mbuf handle.
 *           MV_MBUF_Close() can't free a mbuf's handle by call
 *           MV_MBUF_Create() got.
 *
 *
 * \param[in]  phMbuf  a mbuf Handle to be destroyed.
 *
 *
 * \retval MV_MBUF_OK            no error occurred
 *
 * \retval MV_MBUF_ERR_BADPARAM  phMbuf is NULL
 *
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Destroy(
		MV_MBUF_HANDLE *phMbuf);



/** \fn  HRESULT MV_MBUF_Open(
 *		MV_MBUF_HANDLE *phMbuf);
 *
 *  \brief This function is used to open a mbuf for
 *           interprocess communication by address.
 *
 *           The remote side must have created an mbuf with
 *           MV_MBUF_Create(). The phMbuf->address member must be
 *           initialized to the mbuf address, and other fields set to
 *           zero. Then MV_MBUF_Open() will initialize the handle to
 *           represent the mbuf with this address.
 *
 *           The direction of the mbuf is write, this function can be call
 *           multiple times in a process, it will get a the same
 *           handle, and it must close it when the application don't use
 *           it. Must be opened once, Close once, They are paired.
 *
 *           only make sure MV_MBUF_Create() and MV_MBUF_Open() used
 *           communication's address is the same, it can communicate.
 *
 *           Note:   A handle from MV_MBUF_Open() only can be
 *                      closed with MV_MBUF_Close().
 *
 * \param[inout]  phMbuf  The mbuf handle containing the address
 *
 *
 * \retval MV_MBUF_OK             open mbuf succeeded
 *
 * \retval MV_MBUF_ERR_NOMEM      no memory for create mbuf.
 * \retval MV_MBUF_ERR_NORES      No resource to create mbuf (system resource other than memory).
 * \retval MV_MBUF_ERR_NOTFOUND   no mbuf with given address could be found
 * \retval MV_MBUF_ERR_BADPARAM   phMbuf is NULL
 *
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Open(
		MV_MBUF_HANDLE *phMbuf);


/** \fn  HRESULT MV_MBUF_Close(
 *		MV_MBUF_HANDLE *phMbuf);
 *
 *  \brief This function used to close mbuf with a mbuf handle
 *
 *           MV_MBUF_Close() is paired with MV_MBUF_Open().
 *
 *           When the mbuf is closed, any blocking call will be woken
 *           up. Remote peers will wake up with error code
 *           MV_MBUF_ERR_PEERGONE. Local peers will wake up with error code
 *           MV_MBUF_ERR_CLOSED.
 *
 *           mbuf used for two user space processes communication,
 *           which process call MV_MBUF_Open() to got a mbuf handle,
 *           it must call MV_MBUF_Close() to close this mbuf handle.
 *           MV_MBUF_Destroy() can't free a mbuf's handle by call
 *           MV_MBUF_Open() got.
 *
 *
 * \param[in]  phMbuf  a mbuf Handle to be freed.
 *
 * \retval MV_MBUF_OK             no error occurred
 *
 * \retval MV_MBUF_ERR_BADPARAM   phMbuf is NULL
 *
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Close(
		MV_MBUF_HANDLE *phMbuf);


/** \fn  HRESULT MV_MBUF_Write(
 *		MV_MBUF_HANDLE *phMbuf,
 *		void *buf,
 *		size_t uNbytes);
 *
 *  \brief This function is used to send some data to another process
 *         through an mbuf handle opened or created for reading.
 *
 *           this function will send \c uNbytes bytes data to another
 *           process, The parameter \c *buf is the data's pointer.
 *           the other process can use MV_MBUF_Read() to got this
 *           data. This function will return when all data has been
 *           transferred.
 *
 *
 * \param[in]  phMbuf  Handle of mbuf
 *
 * \param[in]  buf  a pointer fo send data.
 *
 * \param[in]  uNbytes  send data len.
 *
 *
 * \retval MV_MBUF_OK             no error occurred
 *
 * \retval MV_MBUF_ERR_PEERGONE   function returned because the mbuf was closed or destroyed on the remote side
 * \retval MV_MBUF_ERR_CLOSED     function returned because the mbuf was closed or destroyed in this process
 * \retval MV_MBUF_ERR_BADPARAM   Either buf pointer or phMbuf handle is NULL
 * \retval MV_MBUF_ERR_MISMATCH   Write() attempted on a mbuf handle opened or created for reading.
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Write(
		MV_MBUF_HANDLE *phMbuf,
		void *buf,
		size_t uNbytes);


/** \fn  HRESULT MV_MBUF_Read(
 *		MV_MBUF_HANDLE *phMbuf,
 *		void *buf,
 *		size_t uNbytes);
 *
 *  \brief This function is used to receive some data from another
 *         process through an mbuf handle opened or created for reading.
 *
 *           this function want receive \c uNbytes bytes data from another
 *           process, The parameter \c *buf is save data's memory pointer.
 *           this function can get MV_MBUF_Write() function send data from
 *           the other process. The call will return when all bytes have been received
 *
 *
 * \param[in]  phMbuf     Handle of mbuf
 *
 * \param[out]  buf       a pointer fo receive data.
 *
 * \param[in]  uNbytes    want receive data len.
 *
 * \retval MV_MBUF_OK             no error occurred
 *
 * \retval MV_MBUF_ERR_PEERGONE   remote side closed the mbuf
 * \retval MV_MBUF_ERR_CLOSED     mbuf was closed in this process
 * \retval MV_MBUF_ERR_BADPARAM   Either buf pointer or phMbuf handle is NULL
 * \retval MV_MBUF_ERR_MISMATCH   Read() attempted on a mbuf handle opened or created for writing.
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_Read(
		MV_MBUF_HANDLE *phMbuf,
		void *buf,
		size_t uNbytes);


/** \fn  HRESULT MV_MBUF_ReadTimeOut(
 *		MV_MBUF_HANDLE *phMbuf,
 *		void *buf,
 *		size_t uNbytes,
 *		int timeout);
 *
 *  \brief This function is used to receive some data with a timeout from
 *          another process through an mbuf handle opened or created for
 *          reading.
 *
 *           this function want receive \c uNbytes bytes data from another
 *           process, The parameter \c *buf is save data's memory pointer.
 *
 *           The parameter \c timeout specifies an upper limit on the time for
 *           this function will block, in milliseconds.  Specifying a negative
 *           value in timeout means an infinite timeout.
 *
 *           this function can get MV_MBUF_Write() function send data from
 *           the other process. The call will return when all bytes have been received
 *
 *
 * \param[in]  phMbuf     Handle of mbuf
 *
 * \param[out]  buf       a pointer fo receive data.
 *
 * \param[in]  uNbytes    want receive data len.
 *
 * \param[in]  timeout    pecifies an upper limit on the time, in milliseconds.
 *
 * \retval MV_MBUF_OK           no error occurred
 *
 * \retval MV_MBUF_ERR_PEERGONE   remote side closed the mbuf
 * \retval MV_MBUF_ERR_CLOSED     mbuf was closed in this process
 * \retval MV_MBUF_ERR_BADPARAM   Either buf pointer or phMbuf handle is NULL
 * \retval MV_MBUF_ERR_MISMATCH   Read() attempted on a mbuf handle opened or created for writing.
 * \retval MV_MBUF_ERR_TIMEOUT    Read data time out.
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_TryWrite
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_ReadTimeOut(
		MV_MBUF_HANDLE *phMbuf,
		void *buf,
		size_t uNbytes,
		int timeout);


/** \fn  HRESULT MV_MBUF_TryWrite(
 *		MV_MBUF_HANDLE *phMbuf,
 *		void *buf,
 *		size_t uNbytes,
 *		size_t *puBytesWritten);
 *
 *  \brief This function is used to try send some data to another
 *         process without blocking, using an mbuf handle opened or created for writing.
 *
 *           this function will send at most \c uNbytes bytes data to another
 *           process, The parameter \c *buf is the data's pointer.
 *           the other process can use MV_MBUF_Read() to got this
 *           data.
 *
 *           This function send \c uNbytes bytes data without
 *           blocking. If puBytesWritten is not NULL, the actual number
 *           of bytes transferred will be put there.
 *
 * \param[in]  phMbuf  Handle of mbuf
 *
 * \param[in]  buf  a pointer to data to send.
 *
 * \param[in]  uNbytes  send data len.
 *
 * \param[out] puBytesWritten   number of bytes actually written (if pointer is not NULL)
 *
 * \retval MV_MBUF_OK             no error occurred (but 0 bytes may have been written)
 *
 * \retval MV_MBUF_ERR_PEERGONE   remote side closed the mbuf
 * \retval MV_MBUF_ERR_CLOSED     mbuf was closed in this process
 * \retval MV_MBUF_ERR_BADPARAM   Either buf pointer or phMbuf handle is NULL
 * \retval MV_MBUF_ERR_MISMATCH   TryWrite() attempted on a mbuf handle opened or created for reading.
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryRead
 *
**/
HRESULT MV_MBUF_TryWrite(
		MV_MBUF_HANDLE *phMbuf,
		void *buf,
		size_t uNbytes,
		size_t *puBytesWritten);


/** \fn  HRESULT MV_MBUF_TryRead(
 *		MV_MBUF_HANDLE *phMbuf,
 *		void *buf,
 *		size_t uNbytes,
 *		size_t *puBytesRead);
 *
 *  \brief This function is used to receive some data from another
 *         process without blocking.
 *
 *           this function want receive \c uNbytes bytes data from another
 *           process, The parameter \c *buf is save data's memory pointer.
 *           this function can get MV_MBUF_Write() function send data from
 *           the other process.
 *
 *           This function receive at most \c uNbytes bytes data
 *           without blocking. If puBytesRead is not NULL, the
 *           actual number of bytes read is put there.
 *
 * \param[in]  phMbuf  Handle of mbuf
 *
 * \param[out]  buf  a pointer to receive data.
 *
 * \param[in]  uNbytes  want receive data len.
 * \param[out] puBytesRead   Number of bytes actually read (if pointer is not NULL)
 *
 * \retval MV_MBUF_OK             no error occurred
 *
 * \retval MV_MBUF_ERR_PEERGONE   remote side closed the mbuf
 * \retval MV_MBUF_ERR_CLOSED     mbuf was closed or destroyed in this process
 * \retval MV_MBUF_ERR_BADPARAM   Either buf pointer or phMbuf handle is NULL
 * \retval MV_MBUF_ERR_MISMATCH   TryRead() attempted on a mbuf handle opened or created for writing.
 *
 * \sa MV_MBUF_Create
 * \sa MV_MBUF_Destroy
 * \sa MV_MBUF_Open
 * \sa MV_MBUF_Close
 * \sa MV_MBUF_Write
 * \sa MV_MBUF_Read
 * \sa MV_MBUF_TryWrite
 *
**/
HRESULT MV_MBUF_TryRead(
		MV_MBUF_HANDLE *phMbuf,
		void *buf,
		size_t uNbytes,
		size_t *puBytesRead);


#ifdef __cplusplus
}
#endif


#endif  /* __OSAL_MBUF_H__ */
