/*
 * AC-4 parser
 * Copyright (c) 2003 Fabrice Bellard
 * Copyright (c) 2003 Michael Niedermayer
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

#include "config.h"

#include "libavutil/channel_layout.h"
#include "parser.h"
#include "get_bits.h"
#include "ac4.h"
#include "ac4_parser.h"
#include "libavutil/mem.h"

int ff_aac_ac4_parse(AVCodecParserContext *s1,
                       AVCodecContext *avctx,
                       const uint8_t **poutbuf, int *poutbuf_size,
                       const uint8_t *buf, int buf_size)
{
    *poutbuf = buf;
    *poutbuf_size = buf_size;
    return buf_size;
}

static av_cold int ac4_parse_init(AVCodecParserContext *s1)
{
    return 0;
}

AVCodecParser ff_ac4_parser = {
    .codec_ids      = { AV_CODEC_ID_AC4 },
    .priv_data_size = sizeof(AC4ParseContext),
    .parser_init    = ac4_parse_init,
    .parser_parse   = ff_aac_ac4_parse,
    .parser_close   = ff_parse_close,
};

