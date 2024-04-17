

#ifndef __H264_PARSER_H
#define __H264_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


//FIXME:: set appropriate stream size.
#define AU_STREAM_BUF_SIZE	(1024*1024*2)

/* NAL unit types */
typedef enum __nal_unit_type
{
	NAL_SLICE           = 1,
	NAL_DPA             = 2,
	NAL_DPB             = 3,
	NAL_DPC             = 4,
	NAL_IDR_SLICE       = 5,
	NAL_SEI             = 6,
	NAL_SPS             = 7,
	NAL_PPS             = 8,
	NAL_AUD             = 9,
	NAL_END_SEQUENCE    = 10,
	NAL_END_STREAM      = 11,
	NAL_FILLER_DATA     = 12,
	NAL_SPS_EXT         = 13,
	NAL_AUXILIARY_SLICE = 19,
	NAL_FF_IGNORE       = 0xff0f001,
}nal_unit_type;

typedef struct __h264_au_parser
{
	uint8_t frame_start_found;
	int8_t	state;
	uint8_t *au_buf;
	uint8_t pending_bytes;
	int au_size;
}h264_au_parser;

int h264_auparser_init(h264_au_parser *parser);
int h264_auparser_getau(uint8_t *in_ptr, uint32_t *in_size, uint8_t *out_ptr, uint32_t *out_size, h264_au_parser *parser);
int h264_auparser_close(h264_au_parser *parser);

#endif

