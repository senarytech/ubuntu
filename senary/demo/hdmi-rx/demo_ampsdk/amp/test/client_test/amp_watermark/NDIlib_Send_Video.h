#ifndef _NDI_VIDEO_H_
#define _NDI_VIDEO_H_

#ifdef __cplusplus
extern "C"{
#endif

int ndi_putbuf(unsigned char* pbuf);
int ndi_putbuf_audio(unsigned char* pbuf,int len);
void ndi_putbuf_nosignal();
int ndi_init();
int ndi_uint();

#ifdef __cplusplus
}
#endif

#endif
