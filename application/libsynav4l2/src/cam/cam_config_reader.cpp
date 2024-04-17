/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2020 Synaptics Incorporated. All rights reserved.
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

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#include "cam_config_parser.h"
#include "v4l2_log.h"

#define DISABLE_I2C_LOG

typedef struct _tag_sensor_info {
    char         name[100];
    unsigned int outWidth;
    unsigned int outHeight;
    unsigned int bitDepth;
    unsigned int orientation;
    unsigned int i2cDev;
    unsigned int i2cAddr;
}cam_sensor_info;

typedef struct _tag_cam_config {
    int             sensorCount;
    cam_sensor_info sensorInfo[AMP_ISP_SENSOR_MAX];
}cam_hw_config;

cam_hw_config gCamHwConfig;

static bool readConfig = false;

using namespace tinyxml2;

#ifdef __cplusplus
extern "C" {
#endif

int cam_read_hardware_config(){
    const char *sConfigFileHW;
    XMLDocument *pConfigFileHW;
    XMLElement *pEle;

    if(readConfig)
        return 0;

    sConfigFileHW = getenv("BERLIN_CONFIG_HW");
    if(sConfigFileHW == NULL){
        sConfigFileHW = "/etc/berlin_config_hw.xml";
    }
    pConfigFileHW = new XMLDocument(true, COLLAPSE_WHITESPACE);

    const char *psElePathHW[] = {"Board", "Devices", "ISPSS", "ISP"};
    const char *psElePathHWSensor[] = {"Board",
                                       "Devices",
                                       "ISPSS",
                                       "ISP",
                                       "Sensors"};
    const char *psElePathHWSensorConfig[AMP_ISP_SENSOR_MAX][6] = {
                                       {"Board",
                                        "Devices",
                                        "ISPSS",
                                        "ISP",
                                        "Sensors",
                                        "Sensor1"},
                                       {"Board",
                                        "Devices",
                                        "ISPSS",
                                        "ISP",
                                        "Sensors",
                                        "Sensor2"}};

    if (cam_configparser_initialize(pConfigFileHW, sConfigFileHW)) {
        AMPLOGF("Failed to initialize HW config file passer, force exit!");
        //exit(1);
        delete pConfigFileHW;
        pConfigFileHW = NULL;
        return 0;
    }

    if (SUCCESS == cam_getelementbypath(pConfigFileHW, psElePathHW, 4, &pEle)) {
        if (SUCCESS == cam_getelementbypath(pConfigFileHW, psElePathHWSensor, 5, &pEle)) {

            UINT32 uiMaxSensorNum = AMP_ISP_SENSOR_MAX;

            if (SUCCESS != cam_getelementattribuinteger(pEle, (char *)"MaxSensorNum",
                                                        &uiMaxSensorNum)) {
                uiMaxSensorNum = 0;
            }
            if (uiMaxSensorNum > AMP_ISP_SENSOR_MAX) {
                uiMaxSensorNum = AMP_ISP_SENSOR_MAX;
            }
            gCamHwConfig.sensorCount = uiMaxSensorNum;

            for (int i = 0; i < uiMaxSensorNum; ++i) {
                if (SUCCESS == cam_getelementbypath(pConfigFileHW, psElePathHWSensorConfig[i],
                                                    6, &pEle)) {

                    const char *string;
                    UINT32 val;

                    if (SUCCESS == cam_getelementattribstring(pEle, (char *)"Name",
                                                              &string)) {
                       strcpy(gCamHwConfig.sensorInfo[i].name, string);
                    }
                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"OutputWidth",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].outWidth = val;
                    }
                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"OutputHeight",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].outHeight = val;
                    }
                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"Bitdepth",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].bitDepth = val;
                    }
                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"Orientation",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].orientation = val;
                    }

                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"I2CDev",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].i2cDev = val;
                    }

                    if (SUCCESS == cam_getelementattribuinteger(pEle, (char *)"I2CAddr",
                                                                &val)) {
                        gCamHwConfig.sensorInfo[i].i2cAddr = val;
                    }
                }
            }

        }
    }

    delete pConfigFileHW;
    pConfigFileHW = NULL;
    return 0;
}

int cam_get_total_sensors(){
    return gCamHwConfig.sensorCount;
}


int cam_get_i2c_addr(int id, int *pDev, int *pAddr){
    if(id >= AMP_ISP_SENSOR_MAX || pDev==NULL || pAddr==NULL)
        return -1;

    *pDev = gCamHwConfig.sensorInfo[id].i2cDev;
    *pAddr = gCamHwConfig.sensorInfo[id].i2cAddr;

    return 0;
}

int cam_get_orientation(int id, int *pOrientation){
    if(id >= AMP_ISP_SENSOR_MAX || pOrientation==NULL)
        return -1;

    *pOrientation = gCamHwConfig.sensorInfo[id].orientation;

    return 0;
}

int cam_get_resolution(int id, int *pWidth, int *pHeight, int *pBitDepth) {
    if(id >= AMP_ISP_SENSOR_MAX || pWidth==NULL || pHeight==NULL || pBitDepth==NULL)
        return -1;

    *pWidth = gCamHwConfig.sensorInfo[id].outWidth;
    *pHeight = gCamHwConfig.sensorInfo[id].outHeight;
    *pBitDepth = gCamHwConfig.sensorInfo[id].bitDepth;

    return 0;
}

bool cam_is_connected(unsigned int i2cDev, unsigned int i2cAddr) {
    int fd;
    char filename[25];
    bool is_connected = true;
#if 0
    sprintf(filename, "/dev/i2c-%d", i2cDev);

    if ((fd = open(filename, O_RDWR)) < 0) {
        V4L2LOGE("Failed to open the i2c bus %s\n", filename);
        return false;
    }

    if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
        V4L2LOGE("Failed to acquire bus access to slave.\n");
        close(fd);
        return false;
    }


    union i2c_smbus_data data;
    struct i2c_smbus_ioctl_data args;
    args.read_write = I2C_SMBUS_READ;
    args.command = 0x00;
    args.size = 1;
    args.data = &data;
    if(ioctl(fd, I2C_SMBUS, &args) < 0) {
#if !defined(DISABLE_I2C_LOG)
        V4L2LOGE("Failed to get data from i2c %x %x.\n", i2cDev, i2cAddr);
#endif
        is_connected = false;
    }

#if !defined(DISABLE_I2C_LOG)
    V4L2LOGD("camera connection status = %s\n", is_connected?"true":"false");
#endif

    close(fd);
#endif
    return is_connected;
}

#ifdef __cplusplus
}
#endif
