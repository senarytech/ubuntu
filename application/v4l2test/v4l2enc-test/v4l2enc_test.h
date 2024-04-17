#ifndef V4L2_ENC_TEST_H
#define V4L2_ENC_TEST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <getopt.h>
#include <linux/videodev2.h>
#include "v4l2_api.h"
#include "v4l2_types.h"

// Number of input buffers
#define NB_BUF_INPUT    8
// Number of output buffers
#define NB_BUF_OUTPUT   16
#define USE_THREADED_OUTPUT
#define DECODER 0
#define ENCODER 1
#define CAMERA 2
#define FRAME_RATE_NUM 30
#define FRAME_RATE_DEN 1

#define FRAME_BUF_SIZE (1920*1080*4)

struct v4l2_enc {
    // Global handle for the application to interact with SynaV4l2 library
    V4L2_HANDLE *handle;
    void **mmap_virt_inp;    /* Pointer to input frames pointers */
    int *mmap_size_inp;

    void **mmap_virt_out;    /* Pointer to output frame pointers */
    int *mmap_size_out;

#ifdef USE_THREADED_OUTPUT
    pthread_t thread_id;
    pthread_mutex_t mutex_lock;
    pthread_t thread_id_dqbuf;
#endif
    void *cb_data;
    int (*data_cb)(void *data_ptr, int data_size, void *cb_data);

    int eos;
    int session_active;
    int frame_read_size;
    int last_index;
    struct v4l2_buffer encInpBuf[NB_BUF_INPUT];
    int bBufwithEnc[NB_BUF_INPUT];
    int startSession;

}v4l2_enc;

#endif
