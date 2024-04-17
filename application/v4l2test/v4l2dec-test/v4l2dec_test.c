#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#include "v4l2dec.h"
#include "h264_auparser.h"
#include <linux/videodev2.h>
#include "v4l2_api.h"

#define INPUT_bytes_to_read 1024
uint8_t in_buf[INPUT_bytes_to_read]; // buffer to read from file and feed data to parser

#define AU_BUFF_SIZE (1920*1080*4)
uint8_t au_buf[AU_BUFF_SIZE]; // Buffer holds one AU, populated by the parser

#define FRAME_BUF_SIZE (1920*1080*4)
uint8_t frame_buf[FRAME_BUF_SIZE]; // Buffer holds one decoded frame, returned by the decoder

int write_frame_to_file(void *frame_ptr, int frame_size, void *cb_data)
{
	FILE *fp = (FILE *)cb_data;

	printf("Threaded: Received frame from decoder, size = %d\n", frame_size);
	return fwrite(frame_ptr, 1, frame_size, fp);
}

void print_usage()
{
	printf("\nUsage: ./v4l2dec-test -i <input h264 file> -o <output raw file> -w <width> -h <height> -m <decoder mode: stream-in(0) or frame-in(1)\n");
	return;
}

int main(int argc, char *argv[])
{
	FILE *ifp = NULL, *ofp = NULL;
	int ret = V4L2_SUCCESS;
	unsigned int type;
	v4l2dec *dec;

	h264_au_parser parser;
	uint32_t au_size = 0;
	uint32_t in_size = 0;
	uint32_t bytes_consumed = 0, bytes_read = 0;
	uint32_t bytes_to_read = 0;
	uint32_t total_au_size = 0, eof = 0;
	uint8_t *in_ptr;
	uint32_t bytes_to_move = 0;
	uint32_t size_temp = 0;
	char *in_file = NULL;
	char *out_file = "out.raw";
	uint32_t frame_size = 0;
	uint8_t *frame_ptr;
	int opt;
	uint32_t width = 1920;
	uint32_t height = 1080;
	uint32_t mode = DECODER_IN_FRAME_MODE;

	while((opt = getopt(argc, argv, ":o:i:w:h:m:")) != -1)
	{
		switch(opt)
		{
			case 'o':
				printf("output filename: %s\n", optarg);
				out_file = optarg;
			break;

			case 'i':
				printf("input filename: %s\n", optarg);
				in_file = optarg;
			break;

			case 'w':
				width = atoi(optarg);
				printf("width %d\n", width);
			break;

			case 'h':
				height = atoi(optarg);
				printf("height %d\n", height);
			break;

			case 'm':
				mode = atoi(optarg);
				break;

			default:
				printf("unknown option: %c \n", opt);
				print_usage();
				exit(0);
		}
	}

	if(!in_file) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	ifp = fopen(in_file, "rb");
	if(!ifp) {
		printf("error: Unable to open input file\n");
		goto err;
	}

	ofp = fopen(out_file, "wb");
	if(!ofp) {
		printf("error: Unable to open output file\n");
		goto err;
	}

	//init decoder session
	dec = (v4l2dec *)malloc(sizeof(v4l2dec));
	memset(dec, 0, sizeof(v4l2dec));
	dec->session_active = 1;
	dec->decoder_mode = mode;
	dec->cb_data = (void *)ofp;
	dec->frame_cb = write_frame_to_file;
	dec->videoH264DecoderEventSem = (tsem_t *) malloc(sizeof(tsem_t));
	tsem_init(dec->videoH264DecoderEventSem, 0);

	// Initialize V4l2 library
	ret = SynaV4L2_Init();
	if (ret != V4L2_SUCCESS) {
		printf("Error in v4l2 init\n");
		goto err;
	}

	//init and get decoder component handle
	ret = SynaV4L2_Open(&dec->handle, DECODER, V4L2_PIX_FMT_H264, O_NONBLOCK);
	if (ret == V4L2_SUCCESS)
		printf("v4l2dec open done handle = 0x%x\n", (int)dec->handle);
	else {
		printf("Error in SynaV4L2_Open\n");
		goto err;
	}

	//set input/output pixal format, width, height, numOfPlane, image size
	if (v4l2dec_set_formats(dec, width, height) < 0)
	{
		printf("v4l2dec_set_formats failed\n");
		goto err;
	}

	if (dec->decoder_mode == DECODER_IN_STREAM_MODE) {
		bytes_to_read = sizeof(au_buf);
		//Read input from file and feed it to decoder and get decoded frame then write it to file
		bytes_read = fread(au_buf, 1, bytes_to_read, ifp);
		if (bytes_read < bytes_to_read)
			printf("total bytes read: %d\n", bytes_read);

		if(v4l2dec_decode(au_buf, bytes_read, dec) < 0)
			printf("Decoding failed..\n");
	} else {
		// Initialize H264 AU parser
		memset(&parser, 0, sizeof(parser));
		h264_auparser_init(&parser);

		bytes_to_read = sizeof(in_buf);
		in_ptr = in_buf;

		//Read input from file, parse the data and feed it to decoder and get decoded frame then write it to file
		while(1) {
			if(!eof) {
				bytes_read = fread(in_ptr, 1, bytes_to_read, ifp);
				if(bytes_read != bytes_to_read) {
					in_size += bytes_read;
					eof = 1;
				} else
					in_size = sizeof(in_buf);
			}
			size_temp = in_size;

			//get the AU
			h264_auparser_getau(in_buf, &in_size, au_buf, &au_size, &parser);

			if(in_size) {
				// remaining bytes
				bytes_consumed = size_temp - in_size;
				bytes_to_move = in_size;

				if(bytes_consumed) {
					memmove(in_buf, in_buf+bytes_consumed, bytes_to_move);
					in_ptr = in_buf + bytes_to_move;
				} else
				in_ptr = in_buf;

				bytes_to_read = bytes_consumed;
			} else {
				bytes_to_read = sizeof(in_buf);
				in_ptr = in_buf;
			}

			if(au_size) {
				// Got one AU, feed it to decoder
				if(v4l2dec_decode(au_buf, au_size, dec) < 0) {
					printf("Decoding failed..\n");
					break;
				}
				total_au_size += au_size;

				if(eof && !in_size)
					break; // EOF reached
				usleep(30000);
			}
		}

		//submit end of stream buff with zero byte
		memset(au_buf, 0, sizeof(au_buf));
		if (v4l2dec_decode(au_buf, 0, dec) < 0)
			printf("Decoding failed for end of stream buff\n");
		printf("Total AU size = %d\n", total_au_size);

		//close h264_auparser
		h264_auparser_close(&parser);
	}

	//get remaining decoded frame before closing session
	while (!dec->EndOfStream)
		v4l2dec_frame_out((void *)dec);

	printf("Total No of Decoded Frames = %d\n", dec->no_of_frames);

	//Closing and clean up of the decoder session
	v4l2dec_close(dec);

	ret = SynaV4L2_Close(dec->handle);
	if (ret != V4L2_SUCCESS)
	{
		printf("SynaV4L2_Close Failed\n");
	}

	ret = SynaV4L2_Deinit();
	if (ret != V4L2_SUCCESS)
	{
        printf("SynaV4L2_Deinit Failed\n");
    }
err:
	if(ifp) fclose(ifp);
	if(ofp) fclose(ofp);
	tsem_deinit(dec->videoH264DecoderEventSem);
	free(dec->videoH264DecoderEventSem);
	free(dec);

	printf("*** closed V4l2 Decoder session..\n");

	return 0;
}
