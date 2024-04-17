/*
 * Copyright (C) 2007 The Android Open Source Project
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
#ifdef CONFIG_FASTLOGO
//#include <stdbool.h>
//#include <stdlib.h>
//#include <unistd.h>
#include "io.h"

#include "font_10x18.h"
#include "minui.h"
#include "string.h"
#include "debug.h"
#include "vpp_api.h"
#include "debug.h"
#include "timer.h"
#include "boot_mode.h"

#define OVERSCAN_PERCENT 0
#define FB_WIDTH     1280
#define FB_HEIGHT    720
#define NUM_BUFFER 4

#define align(x, y, delta, rotate) if(rotate == ROTATION_270D) \
                                       x += delta; \
                                   else \
                                       y += delta;

typedef struct {
    GRSurface* texture;
    int cwidth;
    int cheight;
} GRFont;

static GRFont* gr_font = NULL;
//static minui_backend* gr_backend = NULL;

static int overscan_percent = OVERSCAN_PERCENT;
static int overscan_offset_x = 0;
static int overscan_offset_y = 0;


static unsigned char gr_current_r = 255;
static unsigned char gr_current_g = 255;
static unsigned char gr_current_b = 255;
static unsigned char gr_current_a = 255;

static GRSurface* gr_draw = NULL;
static int gr_bufcount =0;
static unsigned char* gr_base = NULL;


static bool outside(int x, int y)
{
    return x < 0 || x >= gr_draw->width || y < 0 || y >= gr_draw->height;
}

int gr_measure(const char *s)
{
    return gr_font->cwidth * strlen(s);
}

void gr_font_size(int *x, int *y)
{
    *x = gr_font->cwidth;
    *y = gr_font->cheight;
}

static void text_blend(unsigned char* src_p, int src_row_bytes,
                       unsigned char* dst_p, int dst_row_bytes,
                       int width, int height)
{
    int i, j;
    for (j = 0; j < height; ++j) {
        unsigned char* sx = src_p;
        unsigned char* px = dst_p;
        for (i = 0; i < width; ++i) {
            unsigned char a = *sx++;
            if (gr_current_a < 255) a = ((int)a * gr_current_a) / 255;
            if (a == 255) {
                *px++ = gr_current_r;
                *px++ = gr_current_g;
                *px++ = gr_current_b;
                *px++ = gr_current_a;
                //px++;
            } else if (a > 0) {
                *px = (*px * (255-a) + gr_current_r * a) / 255;
                ++px;
                *px = (*px * (255-a) + gr_current_g * a) / 255;
                ++px;
                *px = (*px * (255-a) + gr_current_b * a) / 255;
                ++px;
                ++px;
            } else {
                px += 4;
            }

            if(gr_draw->rotation == ROTATION_270D)
                px = px - gr_draw->row_bytes - gr_draw->pixel_bytes;
        }

        src_p += src_row_bytes;
        if(gr_draw->rotation == ROTATION_270D)
            dst_p += gr_draw->pixel_bytes;
        else
            dst_p += dst_row_bytes;
    }
}


void gr_text(int x, int y, const char *s, int bold)
{
    GRFont *font = gr_font;
    unsigned off;

    if (!gr_draw) return;
    if (!font->texture) return;
    if (gr_current_a == 0) return;

    bold = bold && (font->texture->height != font->cheight);

    x += overscan_offset_x;
    y += overscan_offset_y;

    while((off = *s++)) {
        off -= 32;
        if (outside(x, y) || outside(x+font->cwidth-1, y+font->cheight-1)) break;
        if (off < 96) {

            unsigned char* src_p = font->texture->data + (off * font->cwidth) +
                (bold ? font->cheight * font->texture->row_bytes : 0);
            unsigned char* dst_p = gr_draw->data + y*gr_draw->row_bytes + x*gr_draw->pixel_bytes;

            text_blend(src_p, font->texture->row_bytes,
                       dst_p, gr_draw->row_bytes,
                       font->cwidth, font->cheight);

        }
    if(gr_draw->rotation == ROTATION_270D)
        y -= font->cwidth;
    else
        x += font->cwidth;
    }
}

void gr_texticon(int x, int y, GRSurface* icon) {
    if (!gr_draw) return;
    if (icon == NULL) return;

    if (icon->pixel_bytes != 1) {
        NOTICE("gr_texticon: source has wrong format\n");
        return;
    }

    x += overscan_offset_x;
    y += overscan_offset_y;

    if (outside(x, y) || outside(x+icon->width-1, y+icon->height-1)) return;

    unsigned char* src_p = icon->data;
    unsigned char* dst_p = gr_draw->data + y*gr_draw->row_bytes + x*gr_draw->pixel_bytes;

    text_blend(src_p, icon->row_bytes,
               dst_p, gr_draw->row_bytes,
               icon->width, icon->height);
}

void gr_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    gr_current_r = r;
    gr_current_g = g;
    gr_current_b = b;
    gr_current_a = a;
}

void gr_clear()
{
    if (!gr_draw) return;

    if (gr_current_r == gr_current_g &&
        gr_current_r == gr_current_b) {
        memset(gr_draw->data, gr_current_r, gr_draw->height * gr_draw->row_bytes);
    } else {
        int x, y;
        unsigned char* px = gr_draw->data;
        for (y = 0; y < gr_draw->height; ++y) {
            for (x = 0; x < gr_draw->width; ++x) {
                *px++ = gr_current_r;
                *px++ = gr_current_g;
                *px++ = gr_current_b;
                px++;
            }
            px += gr_draw->row_bytes - (gr_draw->width * gr_draw->pixel_bytes);
        }
    }
}

void gr_fill(int x1, int y1, int x2, int y2)
{
    if (!gr_draw) return;

    x1 += overscan_offset_x;
    y1 += overscan_offset_y;

    x2 += overscan_offset_x;
    y2 += overscan_offset_y;

    if (outside(x1, y1) || outside(x2-1, y2-1)) return;

    unsigned char* p = gr_draw->data + y1 * gr_draw->row_bytes + x1 * gr_draw->pixel_bytes;
    if (gr_current_a == 255) {
        int x, y;
        for (y = y1; y < y2; ++y) {
            unsigned char* px = p;
            for (x = x1; x < x2; ++x) {
                *px++ = gr_current_r;
                *px++ = gr_current_g;
                *px++ = gr_current_b;
                 *px++ = gr_current_a;
//                px++;
            }
            p += gr_draw->row_bytes;
        }
    } else if (gr_current_a > 0) {
        int x, y;
        for (y = y1; y < y2; ++y) {
            unsigned char* px = p;
            for (x = x1; x < x2; ++x) {
                *px = (*px * (255-gr_current_a) + gr_current_r * gr_current_a) / 255;
                ++px;
                *px = (*px * (255-gr_current_a) + gr_current_g * gr_current_a) / 255;
                ++px;
                *px = (*px * (255-gr_current_a) + gr_current_b * gr_current_a) / 255;
                ++px;
                ++px;
            }
            p += gr_draw->row_bytes;
        }
    }
}

void gr_blit(GRSurface* source, int sx, int sy, int w, int h, int dx, int dy) {
    if (!gr_draw) return;
    if (source == NULL) return;

    if (gr_draw->pixel_bytes != source->pixel_bytes) {
        NOTICE("gr_blit: source has wrong format\n");
        return;
    }

    dx += overscan_offset_x;
    dy += overscan_offset_y;

    if (outside(dx, dy) || outside(dx+w-1, dy+h-1)) return;

    unsigned char* src_p = source->data + sy*source->row_bytes + sx*source->pixel_bytes;
    unsigned char* dst_p = gr_draw->data + dy*gr_draw->row_bytes + dx*gr_draw->pixel_bytes;

    int i;
    for (i = 0; i < h; ++i) {
        memcpy(dst_p, src_p, w * source->pixel_bytes);
        src_p += source->row_bytes;
        dst_p += gr_draw->row_bytes;
    }
}

unsigned int gr_get_width(GRSurface* surface) {
    if (surface == NULL) {
        return 0;
    }
    return surface->width;
}

unsigned int gr_get_height(GRSurface* surface) {
    if (surface == NULL) {
        return 0;
    }
    return surface->height;
}

extern void * GaloisMalloc_Cacheable(unsigned int size);
static void gr_init_font(void)
{
    gr_font = (GRFont *)(uintptr_t)GaloisMalloc_Cacheable(sizeof(*gr_font));

    // fall back to the compiled-in font.
    gr_font->texture = (GRSurface *)(uintptr_t)GaloisMalloc_Cacheable(sizeof(*gr_font->texture));
    gr_font->texture->width = font.width;
    gr_font->texture->height = font.height;
    gr_font->texture->row_bytes = font.width;
    gr_font->texture->pixel_bytes = 1;

    unsigned char* bits =(unsigned char *)(uintptr_t)GaloisMalloc_Cacheable(font.width * font.height);
    gr_font->texture->data = (void*) bits;

    unsigned char data;
    unsigned char* in = font.rundata;
    while((data = *in++)) {
        memset(bits, (data & 0x80) ? 255 : 0, data & 0x7f);
        bits += (data & 0x7f);
    }

    gr_font->cwidth = font.cwidth;
    gr_font->cheight = font.cheight;

}

void gr_getbuffer(unsigned char **buffer)
{
    if (!gr_draw) return;
    gr_bufcount = gr_bufcount%NUM_BUFFER;
    *buffer= (gr_base + (gr_draw->row_bytes*gr_draw->height)*gr_bufcount);
    gr_bufcount++;
}

void gr_buffer_init()
{
    if (!gr_draw) return;
	gr_getbuffer(&gr_draw->data);
}

extern void MV_DISP_MuteDisplay(int mute);
extern void MV_DISP_Display_Frame(void *buf,int width,int height);

void gr_flip(void) {
    if (!gr_draw) return;
    MV_DISP_MuteDisplay(0);
    MV_DISP_Display_Frame(gr_draw->data,gr_draw->width,gr_draw->height);
}
int gr_init(int width, int height, int logo_rotate)
{

    gr_init_font();
    gr_draw= (GRSurface *)(uintptr_t)GaloisMalloc_Cacheable(sizeof(GRSurface));
    if(gr_draw)
    {
       gr_base=gr_draw->data=(unsigned char *)(uintptr_t)GaloisMalloc_Cacheable(width*height*4*NUM_BUFFER);//ARGB
       gr_draw->width=width;
       gr_draw->height=height;
       gr_draw->row_bytes=width*4;
       gr_draw->pixel_bytes=4;
       gr_draw->rotation=logo_rotate;
       if(gr_base) {
            //Push black frame with full transperancy
            //Note GFX/error plane will be shown only during error scenario
            gr_color(10, 10, 10, 0);
            MV_DISP_Display_Frame(gr_base, width, height);
       }
    }
    overscan_offset_x = gr_draw->width * overscan_percent / 100;
    overscan_offset_y = gr_draw->height * overscan_percent / 100;
    return 0;
 }

void gr_exit(void)
{
}

int gr_fb_width(void)
{
    return gr_draw->width - 2*overscan_offset_x;
}

int gr_fb_height(void)
{
    return gr_draw->height - 2*overscan_offset_y;
}

void gr_fb_blank(bool blank)
{
}

#if 0
void gr_warning_message(int color){

unsigned int start_x, start_y;
int r,g,b;
char message[100];
    switch (color) {
    case 0: //yellow
        r=255;b=255;g=0;
        strcpy(message,"Your device has loaded a different operating system.");
        break;
    case 1: //orange
        r=255;b=128;g=0;
        strcpy(message,"Your device has been unlocked and can't be trusted.");
        break;
    case 2: //red
        r=255;b=0;g=0;
        strcpy(message,"Your device is in an unexpected state and may not work properly.");
        break;
    default:
        r=255;b=255;g=0;
        break;
    }

    start_x = (gr_fb_width() - 650)/2;
    start_y = (gr_fb_height() - 400)/2;
    gr_getbuffer(&gr_draw->data);
    gr_color(0, 0, 0, 255);
    gr_clear();
    gr_color(10, 10, 10, 255);
    gr_fill(start_x,start_y,start_x+650, start_y+400);
    start_x+=25;
    start_y+=25;
    gr_color(g, b, r, 255);
    gr_text(start_x,start_y,"<!>",1);
    start_y+=25;
    gr_color(255, 255, 255, 255);
    gr_text(start_x,start_y,message,1);
    start_y+=50;
    gr_text(start_x,start_y,"Visit this website for more info:",1);
    start_y+=25;
    gr_color(g, b, r, 255);
    gr_text(start_x,start_y,"google.com/help",1);
    start_y+=50;
    start_y+=25;
    gr_color(200, 200, 200, 255);
    gr_text(start_x,start_y,"ID: PUT-SNO-HERE",0);
    start_y+=50;
    gr_color(255, 255, 255, 255);
    gr_text(start_x,start_y,"Your device will boot in 5 seconds.",0);
    gr_flip();
    TEE_Wait(5000);

}
#endif

#define SHOW_WARNING_TIME_LENGTH		9000

void show_warning_message(int color){
	unsigned int start_x, start_y;
	int r,g,b;
	char message[100];
	char serilno_buf[128] = "\0";

    if (!gr_draw) return;

	start_x = (gr_fb_width() - 650)/2;
	start_y = (gr_fb_height() - 400)/2;
	gr_getbuffer(&gr_draw->data);
	gr_color(0, 0, 0, 255);
	gr_clear();
	gr_color(10, 10, 10, 255);
	gr_fill(start_x,start_y,start_x+650, start_y+400);

	get_serialno_from_flash(serilno_buf);
	if(!(strlen(serilno_buf) > 0)) {
		strcat(serilno_buf, "< ID Unknow >");
	}

	if(gr_draw->rotation == ROTATION_270D)
            start_y += 400;

	switch (color) {
		case 0: //yellow
			r=255;b=255;g=0;
			strcpy(message,"Your device has been altered from its factory configuration.");

			start_x+=25;
			start_y+=25;
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,"< WARNING !>",1);
			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,message,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"The Device was unable to verify the keystore with ID:",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,serilno_buf,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"Visit this website for more info:",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,"source.android.com/security",0);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"If not responsible for the changes,security of your device may be at risk.",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"You can choose \" No \" to recovery your device.",0);
			gr_flip();
			mdelay(SHOW_WARNING_TIME_LENGTH);
			break;
		case 1: //orange
			r=255;b=128;g=0;
			//strcpy(message,"Your device has been altered from its factory configuration.");
			strcpy(message,"Your device has been unlocked and can't be trusted.");

			start_x+=25;
			start_y+=25;
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,"< WARNING !>",1);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,message,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"Device has been altered from factory setting with ID:",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,serilno_buf,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"If not responsible for the changes,security of your device may be at risk.",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"You can choose \" No \" to recovery your device.",0);
			gr_flip();
			mdelay(SHOW_WARNING_TIME_LENGTH);
			break;
		case 2: //red
			r=255;b=0;g=0;
			strcpy(message,"Your device has failed verification and may not work properly.");

			start_x+=25;
			start_y+=25;
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,"< WARNING !>",1);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,message,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"Device has been altered from factory setting with ID:",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,serilno_buf,1);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"Visit this website for more info:",0);

			align(start_x, start_y, 25, gr_draw->rotation);
			gr_color(g, b, r, 255);
			gr_text(start_x,start_y,"source.android.com/security",0);

			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"You may attempt to power off the device.",0);
			//start_y+=25;
			//gr_color(255, 255, 255, 255);
			//gr_text(start_x,start_y,"You can choose \" No \" to recovery your device.",0);
			align(start_x, start_y, 50, gr_draw->rotation);
			gr_color(255, 255, 255, 255);
			gr_text(start_x,start_y,"Your device will be power off in 5 seconds.",1);
			gr_flip();
			mdelay(SHOW_WARNING_TIME_LENGTH);
			mdelay(5000);
			break;
		case 3: //green
			r=0;b=0;g=255;
		break;
		case 0xFF:
		default:
			r=0;b=0;g=0;
			strcpy(message,"\0");
			MV_DISP_MuteDisplay(1);
			//MV_DISP_DestroyDisplay();
		break;
	}
	return ;
}
#endif
