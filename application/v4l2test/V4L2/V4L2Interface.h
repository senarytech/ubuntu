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

#ifndef V4L2INTERFACE_H_
#define V4L2INTERFACE_H_

#include <iostream>
#include <iterator>
#include <map>
#include <thread>
#include <array>

#include <fcntl.h>
#include <linux/videodev2.h>

class V4L2Interface {
private:
    typedef int (*SynaV4L2Init)(void);
    typedef int (*SynaV4L2Deinit)(void);
    typedef int (*SynaV4L2Open)(void **handle, unsigned int type,
            unsigned int v4l2_pixfmt, unsigned int flag);
    typedef int (*SynaV4L2Close)(void *handle);
    typedef int (*SynaV4L2Ioctl)(void *handle, int cmd, void *arg);
    typedef int (*SynaV4L2Poll)(void *handle, bool poll_device,
            bool *event_pending);
    typedef int (*SynaV4L2SetDevicePollInterrupt)(void *handle);
    typedef int (*SynaV4L2ClearDevicePollInterrupt)(void *handle);
    typedef void* (*SynaV4L2Mmap)(void *handle, void *addr, size_t length,
            int prot, int flags, unsigned int offset);
    typedef int (*SynaV4L2Munmap)(void *handle, void *addr, size_t length);

#define SYNAV4L2_DECLARE(name) SynaV4L2##name name = nullptr
    struct SynaV4L2Interface {
        SYNAV4L2_DECLARE(Init);
        SYNAV4L2_DECLARE(Deinit);
        SYNAV4L2_DECLARE(Open);
        SYNAV4L2_DECLARE(Close);
        SYNAV4L2_DECLARE(Ioctl);
        SYNAV4L2_DECLARE(Poll);
        SYNAV4L2_DECLARE(SetDevicePollInterrupt);
        SYNAV4L2_DECLARE(ClearDevicePollInterrupt);
        SYNAV4L2_DECLARE(Mmap);
        SYNAV4L2_DECLARE(Munmap);
    };
#undef SYNAV4L2_DECLARE

public:

    enum Type {
        DECODER = 0, ENCODER, CAMERA
    };

    V4L2Interface();
    virtual ~V4L2Interface();

    int open(const char *pathname, int flags);
    int close(int fd);
    int ioctl(int fd, unsigned long request, void *args);
    void* mmap(void *addr, size_t length, int prot, int flags, int fd,
            off_t offset);
    int munmap(void *addr, size_t length, int fd);

    int poll(int fd);

private:
    bool initialized_;

    SynaV4L2Interface v4l2Intfc_;
    void *libsynav4l2_;
    template<typename T> bool loadFunction(T &func, const char *name);
    bool initLibIntfc();

    typedef struct _tag_MappedData {
        std::array<int, 2> fds;
        void *handle;
    }MappedData;
    std::map<int, MappedData*> fdMap;

    std::thread pollingThread;
    void pollInterrupt(void *handle, int fd);
};

#endif /* V4L2INTERFACE_H_ */
