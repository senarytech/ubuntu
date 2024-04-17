#ifndef __V4L2DEC_H
#define __V4L2DEC_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <pthread.h>    /* POSIX Threads */
#include <semaphore.h>
#include "v4l2_types.h"
#include "tsemaphore.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define MAX_DEVICES 20
#define FALSE	0
#define TRUE 	1

#define NB_BUF_INPUT 8
#define NB_BUF_OUTPUT 8         /* nb frames necessary for display pipeline */

#define DECODER 0
#define DECODER_IN_STREAM_MODE    0
#define DECODER_IN_FRAME_MODE     1

//TODO:: remove the macro USE_THREADED_OUTPUT once approach is finalized
//#define USE_THREADED_OUTPUT 1

typedef struct __v4l2dec
{
	V4L2_HANDLE handle;      //handle of decoder component

	void **mmap_virtual_input;    /* Pointer tab of input AUs */
	int *mmap_size_input;

	void **mmap_virtual_output;    /* Pointer tab of output AUs */
	int *mmap_size_output;

	int current_nb_buf_input;
	int current_nb_buf_output;
	int session_active;
	int output_setup;
	int input_setup;
	int no_of_frames;
	int EndOfStream;

	// To be filled by the user
	int (*frame_cb)(void *frame_ptr, int frame_size, void *cb_data);
	void *cb_data;
	int decoder_mode;
	pthread_t v4l2_event_watcher_id;
	tsem_t *videoH264DecoderEventSem;
	int bH264DecoderConfigurationChanged;
	int res_changes;

}v4l2dec;

// params supplied by application to open v4l2 device
typedef struct __v4l2dec_params
{
	uint32_t fmt;
	uint32_t interlace_mode;
	uint32_t width;
	uint32_t height;
	void *cb_data;
	int (*frame_cb)(void *v4l2_data, int v4l2_size, void *cb_data);
}v4l2dec_params;

typedef struct __pixel_format
{
	uint32_t pixel_fmt_nb;
	char *pixel_fmt_str;
}pixel_format;

v4l2dec *v4l2dec_init(v4l2dec_params *params);
void v4l2dec_frame_out(void *ptr);
int v4l2dec_set_formats(v4l2dec *dec, int width, int height);
int v4l2dec_decode(uint8_t *au_ptr, uint32_t au_size, v4l2dec *dec);
int v4l2dec_close(v4l2dec *dec);

#endif

