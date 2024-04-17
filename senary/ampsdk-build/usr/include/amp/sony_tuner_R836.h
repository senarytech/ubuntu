#ifndef SONY_TUNER_R836_H_
#define SONY_TUNER_R836_H_

/*------------------------------------------------------------------------------
 Includes
------------------------------------------------------------------------------*/
#include "sony_tuner_terr_cable.h"
#include "sony_i2c.h"

/*------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

/**
 @brief Creates an instance of the R836 tuner driver

 @param pTuner The tuner driver instance to create. Memory
        must have been allocated for this instance before
        creation.
 @param i2cAddress The I2C address of the R836 device.
        Typically 0x74.
 @param pI2c The I2C driver that the tuner driver will use for
        communication.

 @return SONY_RESULT_OK if successful.
*/
sony_result_t sony_tuner_R836_Create (sony_tuner_terr_cable_t * pTuner,
                                        uint8_t i2cAddress,
                                        sony_i2c_t * pI2c);


#endif /* SONY_TUNER_R836_H_ */

