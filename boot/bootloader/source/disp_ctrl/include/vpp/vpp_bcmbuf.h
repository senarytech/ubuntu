/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

#ifndef _BCMBUF_H_
#define _BCMBUF_H_

typedef unsigned int UINT32;
typedef void *SHM_HANDLE;

typedef struct DHUB_CFGQ_T {
	SHM_HANDLE handle;
	UINT32 shm_offset;
	UINT32 *addr;
	UINT32 len;
	UINT32 *phy_addr;
} DHUB_CFGQ;

/* structure of a buffer descriptor */
typedef struct BCMBUF_T {
	unsigned int *head;
	unsigned int *tail;
	unsigned int *writer;
	int size;
	SHM_HANDLE handle;
	UINT32 shm_offset;
	unsigned int *dv1_head;
	unsigned int *dv2_head;
	unsigned int *dv3_head;
	int subID;
	UINT32 *phy_addr;
} BCMBUF;

/******************************************************************************
 * FUNCTION: allocate register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 *		 : size - size of the buffer to allocate
 *		 :		- (should be a multiple of 4)
 * RETURN:	1 - succeed
 *			0 - failed to initialize a BCM buffer
 ******************************************************************************/
int VPP_BCMBUF_Create(BCMBUF *pbcmbuf, int size);

/******************************************************************************
 * FUNCTION: free register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:	1 - succeed
 *			0 - failed to initialize a BCM buffer
 ******************************************************************************/
int VPP_BCMBUF_Destroy(BCMBUF *pbcmbuf);

/******************************************************************************
 * FUNCTION: reset a register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:	1 - succeed
 *			0 - failed to initialize a BCM buffer
 ******************************************************************************/
int VPP_BCMBUF_Reset(BCMBUF *pbcmbuf);

/******************************************************************************
 * FUNCTION: selest BCM sub-buffer to use
 * PARAMS: *buf - pointer to the buffer descriptor
 *		   subID - DV_1, DV_2, DV_3
 ******************************************************************************/
void VPP_BCMBUF_Select(BCMBUF *pbcmbuf, int subID);

/******************************************************************************
 * FUNCTION: write register address (4bytes) and value
 *			(4bytes) to the buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *			address - address of the register to be set
 *			value - the value to be written into the
 *					register
 * RETURN: 1 - succeed
 *		   0 - register programming buffer is full
 ******************************************************************************/
int VPP_BCMBUF_Write(BCMBUF *pbcmbuf, unsigned int address, unsigned int value);

/******************************************************************************
 * FUNCTION: write a block of data to BCM buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *			*pdata - pointer to the data
 *			length - the length of the data to be written
 *					to BCM buffer
 * RETURN: 1 - succeed
 *		   0 - register programming buffer is full
 ******************************************************************************/
int VPP_BCMBUF_WriteBlock(BCMBUF *pbcmbuf, unsigned int *pdata,
		unsigned int length);

/******************************************************************************
 * FUNCTION: do the hardware transmission
 * PARAMS: block - 0:return without waiting for
 *					 transactionfinishing
 *				   1:return after waiting for
 *					 transaction finishing
 ******************************************************************************/
void VPP_BCMBUF_HardwareTrans(BCMBUF *pbcmbuf, int block);

/******************************************************************************
 * FUNCTION: do the hardware transaction
 * PARAMS: pdata - pointer to the data block
 *				 length - data length for transaction
 ******************************************************************************/
void VPP_BCMBUF_HardwareTransBlock(const unsigned int *pdata,
		unsigned int length, int block);

/******************************************************************************
 * FUNCTION: commit cfgQ which contains BCM DHUB programming info to interrupt
 *			 service routine
 * PARAMS: *cfgQ - cfgQ
 *		   cpcbID - cpcb ID which this cmdQ belongs to
 *		   intrType - interrupt type which this cmdQ belongs to:
 *					  0-VBI, 1-VDE
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
int VPP_BCMDHUB_CFGQ_Commit(DHUB_CFGQ *cfgQ, int cpcbID, int intrType);

/******************************************************************************
 * FUNCTION: send a BCM BUF info info to a BCM cfgQ
 * PARAMS: *pbcmbuf - pointer to the BCMBUF
 *		   *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
int VPP_BCMBUF_To_CFGQ(BCMBUF *pbcmbuf, DHUB_CFGQ *cfgQ);

/******************************************************************************
 * FUNCTION: send a raw BCM BUF info to a BCM cfgQ
 * PARAMS: pdata - pointer to the data block
 *		   length - data length for transaction
 *		   *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
void VPP_BCMBUF_Raw_To_CFGQ(UINT32 *pdata, UINT32 length,
		DHUB_CFGQ *cfgQ);

/******************************************************************************
 * FUNCTION: send a BCM cfgQ info to a BCM cfgQ
 * PARAMS: src_cfgQ - pointer to the source BCM cfgQ
 *		   *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 *****************************************************************************/
void VPP_CFGQ_To_CFGQ(DHUB_CFGQ *src_cfgQ, DHUB_CFGQ *cfgQ);

/******************************************************************************
 * FUNCTION: Write Register block of array using DHUB or Direct REG Write
 * PARAMS: start - array pointer
 *		   size - array size
 *         use_dhub - flag to use dhub or not
 *****************************************************************************/
void VPP_REG_Block_Write(unsigned int *start, unsigned int size, int use_dhub);

#endif

