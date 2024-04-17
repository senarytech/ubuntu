/*******************************************************************************
 * Copyright (C) 2016, Marvell International Ltd. and its affiliates
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * alternative licensing terms.
 *
 *******************************************************************************
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell and you have entered into a commercial
 * license agreement (a "Commercial License") with Marvell, the File is licensed
 * to you under the terms of the applicable Commercial License.
 *
 *******************************************************************************
 */

#ifndef _TEE_VPP_CA_H_
#define _TEE_VPP_CA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "tee_ca_common.h"
#include "vpp_cmd.h"
#include "vpp_api.h"

typedef struct tagDOWNSTREAM_STAT {
    UINT8        devConnected;
    UINT8        devVersion;
    UINT8        devType;
    UINT8        authState;
    UINT8        hpdAsserted;
} DOWNSTREAM_DEVSTAT;

typedef struct tagDOWNSTREAM_TOPOLOGY {
    UINT8                dwnStrmDevId[5];
    UINT8                dwnStrmRxIdList[5*32];
    UINT16               RxInfo;
} DOWNSTREAM_TOPOLOGY;

int VppRegisterShm(unsigned int *PhyAddr, unsigned int Size);
int VppUnregisterShm(unsigned int *PhyAddr, unsigned int Size);
int VppInitialize(void);
void VppFinalize(void);

TAVPPPassShm *VppSelectPassShm();
int VPP_InvokePassShm_Helper(void *pBuffer, VPP_SHM_ID shmCmdId,
                         UINT32 sBufferSize);

int VppPassShm(unsigned int *PhyAddr, VPP_SHM_ID AddrId, unsigned int Size);
int VppPassVbufInfo(unsigned int *Vbuf, unsigned int VbufSize,
                         unsigned int *Clut, unsigned int ClutSize,
                         int PlaneID, int ClutValid, VPP_SHM_ID ShmID);
int VppInit(VPP_INIT_PARM *vpp_init_parm);
int VppAVIOReset(void);
int VppCreate(void);
int VppReset(void);
int VppConfig(void);
int VppSetOutRes(int CpcbId, int ResId, int BitDepth);
int VppSetRefWin(int PlaneId, int WinX, int WinY, int WinW, int WinH);
int VppOpenDispWin(int PlaneId, int WinX, int WinY, int WinW, int WinH, int BgClr, int Alpha, ENUM_GLOBAL_ALPHA_FLAG globalAlphaFlag);
int VppChangeDispWin(int PlaneId, int WinX, int WinY, int WinW, int WinH, int BgClr, int Alpha, ENUM_GLOBAL_ALPHA_FLAG globalAlphaFlag);
int VppSetDispMode(int PlaneId, int Mode);
int VppHdmiSetVidFmt(int ColorFmt, int BitDepth, int PixelRep);
int VppSetPlaneMute(int planeID, int mute);
int VppSetHdmiTxControl(int enable);

int Vppstop(void);
int VppDestroy(void);
int VppIsrHandler(unsigned int MsgId, unsigned int IntSts);
int MV_VPPOBJ_Stop(int handle);
int VppRecycleFrame(int PlaneId);
#ifdef __cplusplus
}
#endif

#endif /* _TEE_VPP_CA_H_ */
