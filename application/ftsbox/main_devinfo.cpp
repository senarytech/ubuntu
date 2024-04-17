/*
 * Copyright (C) 2018 Synaptics Incorporated. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
//#include <android-base/unique_fd.h>
#include <fcntl.h>
#include "flash_ts.h"
#include "log.h"
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "devinfo"
#endif

#define DEVINFO_DEVICE "/dev/mmcblk0p14"
#define MAX_KEY_LEN 16
#define MAX_VALUE_LEN 128
#define FTS_KEY_MACADDR "macaddr"
#define MAC_STR_LEN 17
#define MAC_HEX_LEN 6
#define DEVINFO_MACADDR_SIZE 32
#define DEVINFO_SERIALNO_SIZE 64

#define TEST_AND_RETURN_FALSE(_x)              \
    do {                                       \
        bool _success = static_cast<bool>(_x); \
        if (!_success) {                       \
            INFO(" %d failed.",_x);            \
            return false;                      \
        }                                      \
    } while (0)

//using namespace android::base;

static void usage(char* name) {
     printf("Usage: %s [-h] [-get] [-set] \n"
                    "\t sn : sn number, for get and set command, set value as 1254585845212511254\n"
                    "\t mac : ethernet mac address,for get and set command,set value as 02:11:22:33:44:55 \n",name);
	 printf("Example: %s -get sn\n", name);
	 printf("Example: %s -set sn  1254585845212511254\n", name);
}


static bool is_hex(char hex) {
    if (hex >= '0' && hex <= '9') {
        return true;
    } else if (hex >= 'a' && hex <= 'f') {
        return true;
    } else if (hex >= 'A' && hex <= 'F') {
        return true;
    }

    return false;
}

static bool is_valid_macaddr(const char* ethaddr) {
    int index = 0;

    while (index < MAC_STR_LEN) {
        if (index % 3 == 2) {
            if (ethaddr[index] != ':') {
                return false;
            }
        } else if (!is_hex(ethaddr[index])) {
            return false;
        }
        index++;
    }

    return true;
}


static unsigned int str_to_hex(unsigned char ch) {
    if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    }

    if (ch >= 'a' && ch <= 'f') {
        return (ch - 'a' + 10);
    }

    if (ch >= 'A' && ch <= 'F') {
        return (ch - 'A' + 10);
    }

    return 0;
}

static unsigned char hex_to_str(unsigned int h) {
    h &= 0xf;
    if (h <= 9) {
        return (h + '0');
    }

    if (h >= 0xa && h <= 0xf) {
        return (h + 'a' - 10);
    }

    return 0;
}

static int set_macaddr_to_dev(char* ethaddr) {
    struct ifreq ifr;
    int skfd;

    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
    memcpy(ifr.ifr_hwaddr.sa_data, ethaddr, MAC_HEX_LEN);

    skfd = socket(PF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
        ERROR("%s, socket error", __FUNCTION__);
        return -1;
    }

    if (ioctl(skfd, SIOCSIFHWADDR, &ifr) < 0) {
        ERROR("%s, ioctl error", __FUNCTION__);
        close(skfd);
        return -1;
    }
    close(skfd);

    return 0;
}


static int get_macaddr_from_dev(char* ethaddr) {
    struct ifreq ifr;
    int skfd;

    snprintf(ifr.ifr_name, IFNAMSIZ, "eth0");
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    skfd = socket(PF_INET, SOCK_STREAM, 0);
    if (skfd < 0) {
        ERROR("%s, socket error", __FUNCTION__);
        return -1;
    }

    if (ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
        ERROR("%s, ioctl error", __FUNCTION__);
        close(skfd);
        return -1;
    }

    memcpy(ethaddr, ifr.ifr_hwaddr.sa_data, MAC_HEX_LEN);
    close(skfd);

    return 0;
}

static int get_macaddr_from_file(const char* filename, char* macaddr) {
    if (filename == NULL) {
        return -1;
    }

    FILE* pfd = fopen(filename, "r");
    if (pfd == NULL) {
        return -1;
    }

    if (fread(macaddr, MAC_STR_LEN, 1, pfd) != 1) {
        fclose(pfd);
        return -1;
    }
    fclose(pfd);

    if (!is_valid_macaddr(macaddr)) {
        return -1;
    }

    macaddr[MAC_STR_LEN] = '\0';

    return 0;
}

static void macaddr_str_to_val(const char* str, char* val) {
    int index = 0;

    while (index < MAC_HEX_LEN) {
        val[index] = (str_to_hex(str[index * 3]) << 4) + str_to_hex(str[index * 3 + 1]);
        index++;
    }
}

static void macaddr_val_to_str(const char* val, char* str) {
    int index = 0;

    while (index < MAC_STR_LEN) {
        if (index % 3 == 0) {
            str[index] = hex_to_str(val[index / 3] >> 4);
        } else if (index % 3 == 1) {
            str[index] = hex_to_str(val[index / 3] & 0xf);
        } else {
            str[index] = ':';
        }
        index++;
    }

    str[MAC_STR_LEN] = '\0';
}


bool write_devinfo(const void* p, size_t size, const std::string& blk_device, size_t offset) {
    int  fd = open(blk_device.c_str(), O_WRONLY);
    if (fd == -1) {
        ERROR("%s(): failed to open %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        return false;
    }
    if (lseek(fd, static_cast<off_t>(offset), SEEK_SET) != static_cast<off_t>(offset)) {
        ERROR("%s(): failed to lseek %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        close(fd);
        return false;
    }
    if (!write(fd, p, size)) {
        ERROR("%s(): failed to write %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        close(fd);
        return false;
    }
    if (fsync(fd) == -1) {
        ERROR("%s(): failed to fsync %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

bool read_devinfo(void* p, size_t size, const std::string& blk_device, size_t offset) {
    int fd = open(blk_device.c_str(), O_RDONLY);
    if (fd == -1) {
        ERROR("%s(): failed to open %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        return false;
    }
    if (lseek(fd, static_cast<off_t>(offset), SEEK_SET) != static_cast<off_t>(offset)) {
        ERROR("%s(): failed to lseek %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        close(fd);
        return false;
    }
    if (!read(fd, p, size)) {
        ERROR("%s(): failed to read %s: %s", __FUNCTION__, blk_device.c_str(), strerror(errno));
        close(fd);
        return false;
    }
    close(fd);
    return true;
}

bool read_macaddr_from_devinfo(char* p) {
    return read_devinfo(p, MAC_STR_LEN + 1, DEVINFO_DEVICE, 0);
}

bool write_macaddr_to_devinfo(const char* p) {
    return write_devinfo(p, DEVINFO_MACADDR_SIZE, DEVINFO_DEVICE, 0);
}

bool read_serialno_from_devinfo(char* p) {
    return read_devinfo(p, DEVINFO_SERIALNO_SIZE, DEVINFO_DEVICE, 32);
}

bool write_serialno_to_devinfo(const char* p) {
    return write_devinfo(p, DEVINFO_SERIALNO_SIZE, DEVINFO_DEVICE, 32);
}

bool ethconfig_devinfo(char* macaddr) {
    char storedMacaddr[DEVINFO_MACADDR_SIZE] = {0};
    INFO("%s(): mac is [%s]", __FUNCTION__, macaddr);
    TEST_AND_RETURN_FALSE(read_macaddr_from_devinfo(storedMacaddr));
    if (0 == memcmp(macaddr, storedMacaddr, sizeof(storedMacaddr))) {
        INFO("%s(): macaddr already stored", __FUNCTION__);
        return true;
    }
    TEST_AND_RETURN_FALSE(write_macaddr_to_devinfo(macaddr));
    INFO("%s(): stored mac address %s to devinfo", __FUNCTION__, macaddr);
    return true;
}

bool update_serialno(char* snData) {
    char storedSnData[DEVINFO_SERIALNO_SIZE] = {0};
    INFO("%s(): sn is [%s]", __FUNCTION__, snData);
    TEST_AND_RETURN_FALSE(read_serialno_from_devinfo(storedSnData));
    if (0 == memcmp(snData, storedSnData, DEVINFO_SERIALNO_SIZE)) {
        INFO("%s(): sn already stored", __FUNCTION__);
        return true;
    }
    TEST_AND_RETURN_FALSE(write_serialno_to_devinfo(snData));
    INFO("%s(): stored sn to devinfo", __FUNCTION__);
    return true;
}

int main(int argc, char** argv) {
    bool bRet=false;
    char storedSnData[DEVINFO_SERIALNO_SIZE] = {0};
    char storedMacaddr[DEVINFO_MACADDR_SIZE] = {0};
    if(argc < 2) {
        usage(argv[0]);
        return -1;
    }
    if (access(DEVINFO_DEVICE, R_OK | W_OK) != 0) {
        printf("no devinfo, can not perform device info set/get: %s \n",strerror(errno));
        return -1;
    }
    argv += 1;
    if(0 == strcmp(*argv, "-get")) {
        if(argc == 3) {
            argv++;
            if(*argv) {
                if(0 == strcmp(*argv, "sn")) {
                    bRet=read_serialno_from_devinfo(storedSnData);
                    printf("%s\n",storedSnData);
                }else if(0 == strcmp(*argv, "mac")){
                    bRet=read_macaddr_from_devinfo(storedMacaddr);
                    printf("%s\n",storedMacaddr);
                }
                else {
                    argv -= 2;
                    usage(argv[0]);
                    return -1;
                }
            }else{
                argv -= 2;
                usage(argv[0]);
                return -1;
            }
        }else {
            argv -= 1;
            usage(argv[0]);
            return -1;
        }

    }else if(0 == strcmp(*argv, "-set")) {

        if(argc == 4) {
            argv++;
            if(*argv) {
                if(0 == strcmp(*argv, "sn")) {
                    argv++;
                    bRet=update_serialno((char*)*argv);
                    printf("[%s][line:%d]  SET_SN = %s,ret=%d	\n", __func__, __LINE__, *argv,bRet);
                 }else if(0 == strcmp(*argv, "mac")) {
                    argv++;
                    bRet=ethconfig_devinfo((char*)*argv);
                    printf("[%s][line:%d]  SET_MAC = %s,ret=%d	\n", __func__, __LINE__, *argv,bRet);
                 }else {
                    argv -= 2;
                    usage(argv[0]);
                    return -1;
                 }
            }else {
                argv -= 2;
                usage(argv[0]);
                return -1;
            }
        }else {
            argv -= 1;
            usage(argv[0]);
            return -1;
        }

    }else {
        argv -= 1;
        usage(argv[0]);
        return -1;
    }

    return 0;
}
