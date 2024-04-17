#ifndef _NDI_BUF_H_
#define _NDI_BUF_H_

#ifdef __cplusplus
extern "C"{
#endif

int ndi_buf_create(int video_size, int video_num,int audio_size, int audio_num);
int ndi_buf_push(unsigned char *pbuf, int size);
int ndi_buf_pull_hdmi(unsigned char * p_buf);
int ndi_buf_push_audio(unsigned char *pbuf, int size);
void ndi_buf_stop();

#ifdef __cplusplus
}
#endif

#endif
