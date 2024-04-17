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

#ifndef _MINUI_H_
#define _MINUI_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int width;
    int height;
    int row_bytes;
    int pixel_bytes;
    int rotation;
    unsigned char* data;
} GRSurface;

typedef GRSurface* gr_surface;

#ifdef CONFIG_FB
int gr_init(void);
#else
int gr_init(int width, int height, int logo_rotate);
#endif /* CONFIG_FB */

void gr_exit(void);

int gr_fb_width(void);
int gr_fb_height(void);

void gr_flip(void);
void gr_fb_blank(bool blank);

void gr_clear();  // clear entire surface to current color
void gr_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void gr_fill(int x1, int y1, int x2, int y2);
void gr_text(int x, int y, const char *s, int bold);
void gr_texticon(int x, int y, gr_surface icon);
int gr_measure(const char *s);
void gr_font_size(int *x, int *y);

void gr_blit(gr_surface source, int sx, int sy, int w, int h, int dx, int dy);
unsigned int gr_get_width(gr_surface surface);
unsigned int gr_get_height(gr_surface surface);

#ifdef __cplusplus
}
#endif

#endif

