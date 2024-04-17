/*
 *
 *	H264 Access Unit parser, based on FFMPEG h264_parser
 *
*/

#include "h264_auparser.h"

//TODO: look for further improvements
// Buffer is expected in 4 byte boundary
int h264_find_start_code(const uint8_t *buf, int size)
{
	int i = 0;

	while (i < size && !((~*(const uint32_t *)(buf + i) & (*(const uint32_t *)(buf + i) - 0x01010101U)) & 0x80808080U))
		i += 4;

	for(; i < size; i++)
		if(!buf[i])
			break;

	return i;
}


int h264_find_start_code1(uint8_t *buf, int buf_size)
{
	uint8_t *p = buf;
	uint8_t *end = &buf[buf_size];
	uint8_t start_code_size;

	assert(p <= end);
	if (p >= end)
		return buf_size;

	p += 3;
	start_code_size = 3;

	while(p < end) {
		if(p[-1] > 1)
			p += 3;
		else if(p[-2])
			p += 2;
		else if(p[-3] | (p[-1] - 1))
			p++;
		else
			break;
	}

	if(p[-4] == 0 && ((p-4) >= buf))
		start_code_size = 4;

	if(p >= end)
		return (int) (end - buf);
	else
		return (int) (p - buf) - start_code_size;
}

static int h264_auparser_find_frame_end(uint8_t *buf, uint32_t buf_size, int *end_pos, h264_au_parser *parser)
{
	uint32_t i = 0;
	int32_t state = parser->state;

	if (state > NAL_SPS_EXT)
		state = NAL_SPS;

	for(i = 0; i < buf_size; i++) {
		if (state == NAL_SPS) {
			i += h264_find_start_code(buf + i, buf_size - i);
			if (i < buf_size)
				state = NAL_DPA;
		} else if (state <= NAL_DPA) {
			if (buf[i] == 1)
				state ^= NAL_IDR_SLICE; // 2->7, 1->4, 0->5
			else if (buf[i])
				state = NAL_SPS;
			else
				state >>= 1;           	// 2->1, 1->0, 0->0
		} else if (state <= NAL_IDR_SLICE) {
			int nalu_type = buf[i] & 0x1F;
			if (	nalu_type == NAL_SEI || nalu_type == NAL_SPS ||
				nalu_type == NAL_PPS || nalu_type == NAL_AUD || nalu_type == NAL_SLICE) {
				if (parser->frame_start_found) {
					i++;
					goto found;
				}
			}

			if (	nalu_type == NAL_SLICE || nalu_type == NAL_DPA ||
					nalu_type == NAL_IDR_SLICE) {
				if (parser->frame_start_found) {
					state += NAL_PPS;
					continue;
				} else
					parser->frame_start_found = 1;
			}
			state = NAL_SPS;
		} else {
			if (buf[i] & 0x80)
				goto found;
			state = NAL_SPS;
		}
	}

	parser->state = state;
	return 0;

found:
	parser->state = NAL_SPS;
	parser->frame_start_found = 0;

	*end_pos = i - 5;

	return 1;
}


/*
 * h264_auparser_getau - get byte stream from user and return one AU when found
 *
 * 	When input buffer is not fully consumed, *in_size is set
 * to number of unused bytes in the buffer. It's the callers responsibility
 * to move the unused bytes and re-fill the input buffer accordingly. This design simulates
 * the BufferMgmtCallback function of the Bellagio OMX component
 *
*/

int h264_auparser_getau(uint8_t *in_ptr, uint32_t *in_size, uint8_t *out_ptr, uint32_t *out_size, h264_au_parser *parser)
{
	int end_pos = 0;
	int found_au = 0;
	uint8_t *au_buf = parser->au_buf;
	uint32_t au_size = parser->au_size;

	if(!*in_size) {
		// EOF reached, give away the last AU
		parser->state = NAL_SPS;
		parser->frame_start_found = 0;

		goto found_au;
	}


	if(parser->pending_bytes) {
		switch(parser->pending_bytes) {
			case 1:
				parser->state = NAL_DPA;
				break;
			case 2:
				parser->state = NAL_SLICE;
				break;
			case 3:
				parser->state = 0;
				break;
			case 4:
				parser->state = NAL_IDR_SLICE;
				break;
			default:
				printf("Warning: Invalid pending bytes\n");
				break;
		}

		parser->pending_bytes = 0;
	}

	*out_size = 0;

	found_au = h264_auparser_find_frame_end(in_ptr, *in_size, &end_pos, parser);

	if(found_au) {
		// "end_pos < 0" means that the NAL code is split between previous and current buffer
		if(end_pos < 0)
			goto found_au;

		memcpy(au_buf+au_size, in_ptr, end_pos);
		au_size += end_pos;
		*in_size -= end_pos;

		goto found_au;

	} else {
		memcpy(au_buf+au_size, in_ptr, *in_size);
		au_size += *in_size;
		*in_size = 0;
	}

	parser->au_size = au_size;
	return 0;

found_au:

	parser->pending_bytes = 0;
	if(end_pos < 0) {
		parser->pending_bytes = abs(end_pos);
		au_size -= parser->pending_bytes;
	}

	*out_size = au_size;
	memcpy(out_ptr, au_buf, au_size);

	au_size = 0;

	if(parser->pending_bytes) {
		memmove(au_buf, &au_buf[au_size], parser->pending_bytes);
		au_size = parser->pending_bytes;
	}

	parser->au_size = au_size;
	return 1;
}


int h264_auparser_init(h264_au_parser *parser)
{
	parser->state = NAL_SPS;
	parser->frame_start_found = 0;
	parser->pending_bytes = 0;

	parser->au_buf = (uint8_t *)malloc(AU_STREAM_BUF_SIZE);

	if(!parser->au_buf) {
		printf("%s:%d malloc failed..\n", __FUNCTION__, __LINE__);
		return -1;
	}

	parser->au_size = 0;

	return 0;
}


int h264_auparser_close(h264_au_parser *parser)
{

	if(parser->au_buf)
		free(parser->au_buf);

	printf("closing parser.. exit \n");

	return 0;
}



