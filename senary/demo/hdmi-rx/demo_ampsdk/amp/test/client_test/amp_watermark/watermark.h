#ifndef _WATER_MARK_H_
#define _WATER_MARK_H_

#ifdef __cplusplus
extern "C"{
#endif

void addwatermark(unsigned char *pbuf_in, unsigned char *pbuf_out, int size);
int get_bmp_buf(unsigned char *pbuf, int size);

#ifdef __cplusplus
}
#endif

#endif
