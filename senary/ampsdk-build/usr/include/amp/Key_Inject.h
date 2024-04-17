////////////////////////////////////////////////////////////////////////////////
//! \file Key_Inject.h
//! \brief Header file declare the FIRSTBOOT CLIENT APIs
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRM_KEY_INJECT__
#define __DRM_KEY_INJECT__

#include "DRM_FIRSTBOOT_AGENT_Api.h"

int Prepare_Key_Inject(Key_Injection_Image KeyInjectData);
int WriteBack_Key_Inject_Data(void);
int Write_NAGRA_EFPK_eMMC(unsigned char *efpk);


#endif
