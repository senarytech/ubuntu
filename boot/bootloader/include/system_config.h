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
#ifndef	__SYSTEM_CONFIG__
#define	__SYSTEM_CONFIG__

#include "com_type.h"

typedef	struct	__User_Video_Param{
	INT8	Video_Output_Resolution ;	//The output resolution that CPU 1 boot-up with.
	INT8	QDEO_Brightness ; 		//Brightness adjustment
	INT8	QDEO_Contrast ;			//Contrast adjustment
	INT8	QDEO_Hue ; 			//Hue adjustment
	INT8	QDEO_Saturation ; 		//Saturation adjustment
	INT16	QDEO_Color_Temperature ; 	//Color temperature adjustment
	INT8	QDEO_Brightness_Level ; 	//Brightness enhancement level
	INT8	QDEO_EE_Gain ; 			//Edge enhancement gain
	INT8	QDEO_LTI_Gain ; 		//Large edge transient improvement gain for luma
	INT8	QDEO_CTI_Gain ; 		//Large edge transient improvement gain for chroma
	INT8	QDEO_ACE ; 			//Adaptive Contrast Enhancement mode
	INT8	QDEO_ICR ; 			//Color remap
	INT8	QDEO_FTDC ; 			//Flesh tone correction
	INT8	QDEO_Gamma ; 			//Gamma correction
	INT8	QDEO_VNR ; 			//Noise reduction
	INT8	QDEO_VNR_Profile ; 		//Noise reduction profile
}User_Video_Param ;

typedef	struct	__Box_Video_Param{
	INT8	HDMI_Color_fmt ;
	INT8	HDMI_Bit_Depth ;
	INT8	HDMI_Pixel_Rept ;
}Box_Video_Param ;

typedef	struct __Video_Param{
	UINT32			uiVideo_Param_Customization_Type ;
	Box_Video_Param		sBox_Video_Param ;
	User_Video_Param	sUser_Video_Param ;
}Video_Param ;

typedef	struct __Audio_Param{
	UINT32	uiAudio_Customization_Type ;
	INT8	MultiCh_Enable ;
	INT8	Stereo_Enable ;
	INT8	Spdif_Enable ;
	INT8	HDMI_Enable ;
	INT8	Max_MultiCh_Num ;
	INT32	Max_Sample_Rate ;
}Audio_Param;


typedef	struct __Customization_Module{
	INT8	bModule_Enable;
	UINT32	uiData_Offset;
	UINT32	uiData_Length;
}Customization_Module ;

typedef struct __System_Customization{
	UINT32	uiBegin_Magic_Number;
	Customization_Module	sAV_Logo ;
	Customization_Module	sStill_Logo ;
	Customization_Module	sPinmux_Setting ;
	Customization_Module	sVideo_Param ;
	Customization_Module	sAudio_Param ;
	UINT32	uiEnd_Magic_Number ;
}System_Customization;

#endif
