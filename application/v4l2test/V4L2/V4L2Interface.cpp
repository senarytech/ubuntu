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

#include "V4L2Interface.h"

#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>

#define LOG_TAG "SynaCameraV4L2"
#define UNUSED(x) (void)x;

unsigned char gMyFd = 0;
unsigned char gExternFd = 1;

V4L2Interface::V4L2Interface() {

    // Load symbols from the library
    if (initLibIntfc() == false) {
        std::cout << "Unable to load the interface library" << std::endl;
        goto error;
    }

    // Initialize the v4l2 interface
    if (0 != v4l2Intfc_.Init()) {
        std::cout << "Unable to init" << std::endl;
        goto error;
    }

    initialized_ = true;
    return;

    error: initialized_ = false;
}

V4L2Interface::~V4L2Interface() {
    v4l2Intfc_.Deinit();
}


void V4L2Interface::pollInterrupt(void *handle, int fd){
    while(true){
        bool pendingEvent = false;
        v4l2Intfc_.Poll(handle, true, &pendingEvent);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1E3;

        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(fd, &write_fds);

        if( 0 >= select(fd + 1, nullptr, &write_fds, nullptr, &timeout)){
            std::cout << "Failed to send the message" << std::endl;
            continue;
        }

        constexpr uint8_t kFrameReadyMsg = 0xBB;
        if (write(fd, &kFrameReadyMsg, sizeof(kFrameReadyMsg)) < 0) {
            std::cerr<< "Failed to send frame ready message\n";
            continue;
        }
    }
}

template<typename T> bool V4L2Interface::loadFunction(T &func,
        const char *name) {
    func = reinterpret_cast<T>(dlsym(libsynav4l2_, name));
    return true;
}

bool V4L2Interface::initLibIntfc(void) {
    // TODO: search for the libsynav4l2 instead of fixed path
    libsynav4l2_ = dlopen("/vendor/lib/libsynav4l2.so",
            RTLD_NOW | RTLD_GLOBAL | RTLD_NODELETE | RTLD_LAZY);
    if (libsynav4l2_ == nullptr){
        std::cout << "cannot find the libsynav4l2 library err = " << dlerror() << std::endl;
        return false;
    }

#define LOAD_FUNCTION_TO_LIBINTFC(name) \
    do{                                                             \
        bool ret = loadFunction(v4l2Intfc_.name, "SynaV4L2_" #name);\
        if(ret == false) return false;                              \
    } while(0)

    LOAD_FUNCTION_TO_LIBINTFC(Init);
    LOAD_FUNCTION_TO_LIBINTFC(Deinit);
    LOAD_FUNCTION_TO_LIBINTFC(Open);
    LOAD_FUNCTION_TO_LIBINTFC(Close);
    LOAD_FUNCTION_TO_LIBINTFC(Ioctl);
    LOAD_FUNCTION_TO_LIBINTFC(Poll);
    LOAD_FUNCTION_TO_LIBINTFC(SetDevicePollInterrupt);
    LOAD_FUNCTION_TO_LIBINTFC(ClearDevicePollInterrupt);
    LOAD_FUNCTION_TO_LIBINTFC(Mmap);
    LOAD_FUNCTION_TO_LIBINTFC(Munmap);
#undef LOAD_FUNCTION_TO_LIBINTFC

    return true;
}

int V4L2Interface::open(const char *pathname, int flags) {
    void *handle = nullptr;
    unsigned int pixFmt = 0;

    // TODO: fix unused warning.
    UNUSED(pathname);

    if (!initialized_)
        return -1;

    int ret = v4l2Intfc_.Open(&handle, Type::CAMERA, pixFmt, flags);

    if (ret != 0) {
        return -1;
    }

    MappedData *mData = (MappedData *)malloc(sizeof(MappedData));
    if (socketpair(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0, mData->fds.data()) < 0) {
        std::cout << "Failed to set up device fd socket pair" << std::endl;
        return -1;
    }
    std::cout << "Creating a socket pair = " << mData->fds[gMyFd] << " - " << mData->fds[gExternFd] << std::endl;
    mData->handle = handle;

    pollingThread = std::thread(&V4L2Interface::pollInterrupt, this, handle, mData->fds[gMyFd]);

    fdMap.insert(std::pair<int, MappedData*>(mData->fds[gExternFd], mData));
    return mData->fds[gExternFd];
}

int V4L2Interface::close(int fd) {
    if (!initialized_)
        return -1;

    MappedData *mData = fdMap.at(fd);
    close(mData->fds[gMyFd]);
    close(mData->fds[gExternFd]);

    v4l2Intfc_.Close(mData->handle);
    if(pollingThread.joinable())
        pollingThread.join();

    fdMap.erase(fd);

    return 0;
}

int V4L2Interface::poll(int fd) {
    if (!initialized_)
        return -1;

    MappedData *mData = fdMap.at(fd);

    bool pendingEvent = false;
    v4l2Intfc_.Poll(mData->handle, true, &pendingEvent);

    return 1;
}

int V4L2Interface::ioctl(int fd, unsigned long request, void *args) {
    if (!initialized_)
        return -1;

    MappedData *mData = fdMap.at(fd);

    if(0 != v4l2Intfc_.Ioctl(mData->handle, request, args))
        return -1;

    return 0;
}

void* V4L2Interface::mmap(void *addr, size_t length, int prot, int flags,
        int fd, off_t offset) {
    if (!initialized_)
        return nullptr;

    MappedData *mData = fdMap.at(fd);
    return v4l2Intfc_.Mmap(mData->handle, addr, length, prot, flags, offset);
}

int V4L2Interface::munmap(void *addr, size_t length, int fd) {
    if (!initialized_)
        return -1;

    MappedData *mData = fdMap.at(fd);
    return v4l2Intfc_.Munmap(mData->handle, addr, length);
}
