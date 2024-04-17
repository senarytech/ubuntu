/*
 * AC-4 parser prototypes
 * Copyright (c) 2021 Sam Chen
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_AC4_PARSER_H
#define AVCODEC_AC4_PARSER_H

#include <stddef.h>
#include <stdint.h>

/**
 * Extract the bitstream ID and the frame size from AC-4 data.
 */
int ff_aac_ac4_parse(AVCodecParserContext *s1,
                        AVCodecContext *avctx,
                        const uint8_t **poutbuf, int *poutbuf_size,
                        const uint8_t *buf, int buf_size);

#endif /* AVCODEC_AC4_PARSER_H */
