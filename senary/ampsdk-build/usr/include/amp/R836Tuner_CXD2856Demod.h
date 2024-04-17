#ifndef R836TUNER_CXD2856DEMOD_H
#define R836TUNER_CXD2856DEMOD_H

/*------------------------------------------------------------------------------
  Includes
------------------------------------------------------------------------------*/
#include "sony_demod.h"
#include "sony_integ.h"
#include "sony_i2c.h"
#include "sony_i2c_i2cdev.h"
#include "sony_tuner_terr_cable.h"

/*------------------------------------------------------------------------------
 Enums
------------------------------------------------------------------------------*/
enum {
    DEMOD_MONITOR_ALL = 0,
    DEMOD_MONITOR_LITE
};

/*------------------------------------------------------------------------------
  Structs
------------------------------------------------------------------------------*/
/**
 @brief Necessary struct instances to control reference code.
*/
typedef struct {
    sony_demod_t             demod;          /**< Demod IC driver struct instance */
    sony_integ_t             integ;          /**< Integ struct instance */
    sony_i2c_t               i2c;            /**< I2C struct instance */
    sony_i2c_i2cdev_t        i2cdev;         /**< Synaptics Linux I2C dev fd */
#ifdef SONY_DEMOD_SUPPORT_TERR_OR_CABLE
    sony_tuner_terr_cable_t  tunerTerrCable; /**< Terrestrial/Cable tuner driver instance */
#endif
} sony_driver_ctx;

/*------------------------------------------------------------------------------
  Variables
------------------------------------------------------------------------------*/
extern const char *Sony_Common_Result[13];       /**< Table to convert sony_result_t value to character */
extern const char *Sony_Common_System[13];       /**< Table to convert sony_dtv_system_t value to character */
extern const char *Sony_Common_Bandwidth[11];    /**< Table to convert sony_dtv_bandwidth_t value to character */

/*------------------------------------------------------------------------------
  Functions
------------------------------------------------------------------------------*/

sony_result_t R836Tuner_CXD2856Demod_Init (sony_driver_ctx* driver);

sony_result_t R836Tuner_CXD2856Demod_Tune (sony_driver_ctx* driver, uint32_t centerFreqKHz);

sony_result_t R836Tuner_CXD2856Demod_Monitor (sony_driver_ctx* driver, int count, int mode);

sony_result_t R836Tuner_CXD2856Demod_AllinOne(sony_driver_ctx* driver, uint32_t centerFreqKHz, int count);

sony_result_t R836Tuner_CXD2856Demod_Sleep_Finalize (sony_driver_ctx* driver);

#endif /* R836TUNER_CXD2856DEMOD_H */

