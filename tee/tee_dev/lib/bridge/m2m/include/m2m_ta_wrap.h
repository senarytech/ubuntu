#ifndef __M2M_TA_WRAP_H__
#define __M2M_TA_WRAP_H__
#include "drm_types.h"
#include "amp_crypto_types.h"
#include "amp_m2mcrypto_api.h"
#include "tee_internal_api.h"
#ifdef __cplusplus
extern "C"
{
#endif

TEE_Result tz_session_open(AMP_M2MCRYPTO_SESSION_TYPE eType, uint32_t uFigoSysId,void  **phSession, uint32_t *uSessionId);
TEE_Result tz_session_close(void  *phSession);
TEE_Result tz_session_setscheme(void  *phSession, AMP_M2MCRYPTO_TYPE eScheme);
TEE_Result tz_session_config(void   *phSession, \
                            AMP_M2MCRYPTO_KEY_OPTION  KeyOption, \
                            uint32_t          KeyTblIndex, \
                            uint8_t           *pKeyData, \
                            uint32_t          KeyLength, \
                            AMP_M2MCRYPTO_KEY_OPTION  IVOption, \
                            uint32_t          IVTblIndex,  \
                            uint8_t           *pIVData, \
                            uint32_t          IVLength, \
                            AMP_CRYPTO_ST   *pCryptInfo);
TEE_Result tz_session_update(void *phSession, void * pInputPhyAddr,  uint32_t uInOffset, uint32_t uInSize,  \
                                           void * pOutputPhyAddr, uint32_t uOutOffset, uint32_t uOutSize);

TEE_Result tz_session_update_residue(void *phSession, void * pInputPhyAddr,  uint32_t uInOffset, uint32_t uInSize, \
                                  void * pOutputPhyAddr, uint32_t uOutOffset, uint32_t uOutSize, uint32_t ResidueType);
TEE_Result tz_session_routine(void *phSession, uint8_t *uIsDone);

#ifdef __cplusplus
}
#endif

#endif /* __M2M_TA_WRAP_H__ */

