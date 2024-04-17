#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "NDIlib_Send_Video.h"

extern "C" {
#include <Processing.NDI.Lib.h>
}

#define MAX_AUDIO_BUFF 64*1024

NDIlib_video_frame_v2_t NDI_video_frame = {};
NDIlib_audio_frame_v2_t NDI_audio_frame = {};
NDIlib_send_instance_t pNDI_send = NULL;
int NDI_done = 0;

void gen_color_bar(uint8_t *data)
{
	int i;
	int j;
	for(i = 0; i < 360; i ++)
	{
		for(j = 0; j < 1920; j ++)
		{
			data[(i * 1080 + j) * 4 ] = 255;
			data[(i * 1080 + j) * 4 + 1] = 0;
			data[(i * 1080 + j) * 4 + 2] = 0;
			data[(i * 1080 + j) * 4 + 3] = 255;
		}
		
	}
	for(i = 360; i < 720; i ++)
	{
		for(j = 0; j < 1920; j ++)
		{
			data[(i * 1080 + j) * 4 ] = 0;
			data[(i * 1080 + j) * 4 + 1] = 255;
			data[(i * 1080 + j) * 4 + 2] = 0;
			data[(i * 1080 + j) * 4 + 3] = 255;
		}
		
	}
	for(i = 720; i < 1080; i ++)
	{
		for(j = 0; j < 1920; j ++)
		{
			data[(i * 1080 + j) * 4 ] = 0;
			data[(i * 1080 + j) * 4 + 1] = 0;
			data[(i * 1080 + j) * 4 + 2] = 255;
			data[(i * 1080 + j) * 4 + 3] = 255;
		}
		
	}

}
int ndi_init()
{
	printf("Enter ndi main\n");
	// Not required, but "correct" (see the SDK documentation).
	if (!NDIlib_initialize())
	{
		printf("NDIlib_initialize error\n");
		return 0;
	}
		

	// We create the NDI sender
	NDIlib_send_create_t NDI_send_create_desc;
	NDI_send_create_desc.p_ndi_name = "SENARY NDI";

	pNDI_send = NDIlib_send_create(&NDI_send_create_desc);

	if (!pNDI_send)
	{
		printf("NDIlib_send_create error\n");
		return 0;
	}
	
	NDI_video_frame.xres = 1920;
	NDI_video_frame.yres = 1080;
	NDI_video_frame.FourCC = NDIlib_FourCC_video_type_UYVY; //NDIlib_FourCC_type_BGRX;
	NDI_video_frame.p_data = (uint8_t*)malloc(NDI_video_frame.xres * NDI_video_frame.yres * 4);
    gen_color_bar(NDI_video_frame.p_data);
	NDI_done = 1;
	//audio
	// Because 48kHz audio actually involves 1601.6 samples per frame, we make a basic sequence that we follow.
	//static const int audio_no_samples[] = { 1602, 1601, 1602, 1601, 1602 };
	int audio_buf_len = MAX_AUDIO_BUFF; //sizeof(float) * 1602 * 2 * 10;
	NDI_audio_frame.sample_rate = 48000;
	NDI_audio_frame.no_channels = 2;
	NDI_audio_frame.no_samples = 1024; // Will be changed on the fly
	NDI_audio_frame.p_data = (float*)malloc(audio_buf_len);
	NDI_audio_frame.channel_stride_in_bytes = sizeof(float) * 1024;
	//NDI_audio_frame.p_metadata = NULL;
	//NDI_audio_frame.timecode = 0;
	memset(NDI_audio_frame.p_data,0,audio_buf_len);
	
	return 0;
}
void ndi_putbuf_nosignal()
{
	if(NDI_done == 0)
		return;

	gen_color_bar(NDI_video_frame.p_data);
	NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);

}
int ndi_uint()
{
	NDI_done = 0;
	// Free the video frame
	if(NDI_video_frame.p_data)
		free(NDI_video_frame.p_data);
	
	if(NDI_audio_frame.p_data)
		free(NDI_audio_frame.p_data);
	// Destroy the NDI sender
	NDIlib_send_destroy(pNDI_send);

	// Not required, but nice
	NDIlib_destroy();
	return 0;
}
int ndi_putbuf(uint8_t* pbuf)
{
	
	if(NDI_done == 0)
		return -1;
	//printf("ndi put buff ... \n");
	memcpy(NDI_video_frame.p_data,pbuf,NDI_video_frame.xres * NDI_video_frame.yres * 4);
    // We now submit the frame. Note that this call will be clocked so that we end up submitting at exactly 29.97fps.
	NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);

	// Success
	return 0;
}


int ndi_putbuf_audio3(uint8_t* pbuf, int len)
{
	static int idx = 0;
	static const int audio_no_samples[] = { 1602, 1601, 1602, 1601, 1602 };
	int send_len = len;
	int audio_buf_len = 64*1024;
	int i;
	
	if(NDI_done == 0)
		return -1;
	
	if(len > audio_buf_len)
	{
		send_len = audio_buf_len;	
	}
	
	memcpy(NDI_audio_frame.p_data,pbuf,send_len);
	printf("ndi audio send len :%d\n",len);
	float* p_ch = (float*)((uint8_t*)NDI_audio_frame.p_data + 0 * NDI_audio_frame.channel_stride_in_bytes);
	for(i = 0; i < 32; i ++)
	{
		printf("%f ",p_ch[i]);
	}
	printf("\n");

	NDI_audio_frame.sample_rate = 48000;
	NDI_audio_frame.no_channels = 2;
	NDI_audio_frame.no_samples = 1602; // Will be changed on the fly
	//NDI_audio_frame.p_data = (float*)malloc(sizeof(float) * 1602 * 2);
	NDI_audio_frame.channel_stride_in_bytes = sizeof(float) * 1602;
	idx ++;
	NDI_audio_frame.no_samples = audio_no_samples[idx % 5];
	// When not black, insert noise into the buffer. This is a horrible noise, but its just
	// for illustration.
	// Fill in the buffer with silence. It is likely that you would do something much smarter than this.
	
	/*
	for (int ch = 0; ch < 2; ch++) 
	{
	// Get the pointer to the start of this channel
		float* p_ch = (float*)((uint8_t*)NDI_audio_frame.p_data + ch * NDI_audio_frame.channel_stride_in_bytes);

		// Fill it with silence
		for (int sample_no = 0; sample_no < 1602; sample_no++)
			p_ch[sample_no] = ((float)rand() / (float)RAND_MAX - 0.5f) * (2.0f);
	}
	*/	
	NDIlib_send_send_audio_v2(pNDI_send, &NDI_audio_frame);
	return 0;
}

int ndi_putbuf_audio(uint8_t* pbuf, int len)
{
	static int idx = 0;
	int send_len = len;
	int audio_buf_len = MAX_AUDIO_BUFF;
	int i,ch;
	
	if(NDI_done == 0)
		return -1;
	
	//signed 16 S16_LE to float data[-1~0~1], divides by 32768(-1~1)
	if(len > audio_buf_len /2)
	{
		send_len = audio_buf_len /2;	
	}
	
	// unsigned chat to short(buffer 2 data), 2 channels 
	send_len = send_len / 4;

	NDI_audio_frame.no_samples = send_len;
	NDI_audio_frame.channel_stride_in_bytes = sizeof(float)*send_len;

	short *pdata = (short *)pbuf;
	for (ch = 0; ch < 2; ch++) 
	{
		// Get the pointer to the start of this channel
		float* p_ch = (float*)((uint8_t*)NDI_audio_frame.p_data + ch * NDI_audio_frame.channel_stride_in_bytes);
		for (i = 0; i < send_len; i++)
			p_ch[i] = pdata[2*i+ch]/(double)32768;
	}
	
	NDIlib_send_send_audio_v2(pNDI_send, &NDI_audio_frame);
	return 0;
}
int ndi_putbuf_audio_test(uint8_t* pbuf, int len)
{
	static int idx = 0;
	int send_len = len;
	int audio_buf_len = MAX_AUDIO_BUFF;
	int i,ch;
	
	if(NDI_done == 0)
		return -1;
	
	//signed 16 S16_LE to float data[-1~0~1], divides by 32768(-1~1)
	if(len > audio_buf_len /2)
	{
		send_len = audio_buf_len /2;	
	}
	
	// unsigned chat to short(buffer 2 data), 2 channels 
	send_len = 1024;
	NDI_audio_frame.no_channels = 2;
	NDI_audio_frame.no_samples = 1024;
	NDI_audio_frame.channel_stride_in_bytes = 4;

	short *pdata = (short *)pbuf;
	float* p_ch = (float*)((uint8_t*)NDI_audio_frame.p_data);
	for (i = 0; i < send_len; i++)
	{
		p_ch[2*i] = pdata[2*i]/(double)32768;
		p_ch[2*i + 1] = pdata[2*i + 1]/(double)32768;
	}
	
	NDIlib_send_send_audio_v2(pNDI_send, &NDI_audio_frame);
	return 0;
}