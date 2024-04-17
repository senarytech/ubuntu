#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "v4l2dec.h"
#include <linux/videodev2.h>
#include "v4l2_api.h"

#define ES_BUFFER_SIZE 3.5 * 1024 * 1024

void disable_port(V4L2_HANDLE handle)
{
	int type;
	int ret;

	/* Stop streaming on output */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = SynaV4L2_Ioctl(handle, VIDIOC_STREAMOFF, &type);

	if (ret != V4L2_SUCCESS)
	{
		printf("ERROR: VIDIOC_STREAMOFF Failed on CAPTURE\n");
		return;
	}
}

void *v4l2_event_watcher(void *data)
{
	v4l2dec *dec = (v4l2dec *) data;
	int ret = 0;
	struct v4l2_event dqevent;

	while(!dec->EndOfStream)
	{
		memset(&dqevent, 0, sizeof(struct v4l2_event));
		ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_DQEVENT, &dqevent);
		if (ret != V4L2_SUCCESS)
		{
			usleep(30000);
			continue;
		}

		switch (dqevent.type) {
			case V4L2_EVENT_SOURCE_CHANGE:
				printf("%s: %d src_change %d\n", __func__,__LINE__, dqevent.u.src_change.changes);
				if (dqevent.u.src_change.changes == V4L2_EVENT_SRC_CH_RESOLUTION) {
					dec->res_changes++;
					if (dec->res_changes > 1) {
						usleep(250000);
						dec->bH264DecoderConfigurationChanged = 1;
						disable_port(dec->handle);
						tsem_up(dec->videoH264DecoderEventSem);
					}
				}
			break;
		}
	}
}

int vidioc_streamon(V4L2_HANDLE handle, unsigned int type)
{
	int ret = V4L2_SUCCESS;
	ret = SynaV4L2_Ioctl(handle, VIDIOC_STREAMON, &type);
	if (ret != V4L2_SUCCESS)
	{
		printf("%s: %d ERROR: VIDIOC_STREAMON Failed\n", __func__,__LINE__);
		return -1;
	}
	return V4L2_SUCCESS;
}

int v4l2dec_set_formats(v4l2dec *dec, int width, int height)
{
	unsigned int ret = V4L2_SUCCESS;
	struct v4l2_format v4l2dec_s_fmt, v4l2dec_g_fmt;
	V4L2_HANDLE handle = dec->handle;

	//Set Format for input
	memset (&v4l2dec_s_fmt, 0, sizeof v4l2dec_s_fmt);
	v4l2dec_s_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	v4l2dec_s_fmt.fmt.pix_mp.field = V4L2_FIELD_INTERLACED;
	v4l2dec_s_fmt.fmt.pix_mp.colorspace = V4L2_COLORSPACE_REC709;
	v4l2dec_s_fmt.fmt.pix_mp.width = width;
	v4l2dec_s_fmt.fmt.pix_mp.height = height;
	v4l2dec_s_fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_H264;
	v4l2dec_s_fmt.fmt.pix_mp.num_planes = 1;
	v4l2dec_s_fmt.fmt.pix_mp.plane_fmt[0].sizeimage = ES_BUFFER_SIZE;
	v4l2dec_s_fmt.fmt.pix_mp.plane_fmt[0].bytesperline = 0;

	ret = SynaV4L2_Ioctl(handle, VIDIOC_S_FMT, &v4l2dec_s_fmt);
	if (ret != V4L2_SUCCESS)
	{
		printf ("%s: Output VIDIOC_S_FMT failed\n", __func__);
		return -1;
	}

	memset (&v4l2dec_g_fmt, 0, sizeof v4l2dec_g_fmt);
	v4l2dec_g_fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = SynaV4L2_Ioctl(handle, VIDIOC_G_FMT, &v4l2dec_g_fmt);
	if (ret != V4L2_SUCCESS)
	{
		printf ("%s: Output VIDIOC_G_FMT failed\n", __func__ );
		return -1;
	}

	//Set Format for output
	v4l2dec_s_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2dec_s_fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;

	ret = SynaV4L2_Ioctl(handle, VIDIOC_S_FMT, &v4l2dec_s_fmt);
	if (ret != V4L2_SUCCESS)
	{
		printf ("%s: Output VIDIOC_S_FMT failed\n", __func__);
		return -1;
	}

	memset (&v4l2dec_g_fmt, 0, sizeof v4l2dec_g_fmt);
	v4l2dec_g_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = SynaV4L2_Ioctl(handle, VIDIOC_G_FMT, &v4l2dec_g_fmt);
	if (ret != V4L2_SUCCESS)
	{
		printf ("%s: Output VIDIOC_G_FMT failed\n", __func__ );
		return -1;
	}

	//subscribe events to receive from libsynav4l2 layer
	struct v4l2_event_subscription sub;
	memset(&sub, 0, sizeof sub);
	sub.type = V4L2_EVENT_SOURCE_CHANGE;
	ret = SynaV4L2_Ioctl(handle, VIDIOC_SUBSCRIBE_EVENT, &sub);
	if (ret != V4L2_SUCCESS)
	{
		printf("%s: VIDIOC_SUBSCRIBE_EVENT failed\n", __func__ );
		return -1;
	}
	//create event watcher thread to look for event receive from libsynav4l2
	pthread_create(&dec->v4l2_event_watcher_id, NULL, v4l2_event_watcher, (void *)dec);

	return 0;
}

int v4l2dec_setup_input(v4l2dec *dec, uint8_t *au_ptr, uint32_t au_size, int index)
{
	struct v4l2_requestbuffers reqbuf;
	struct v4l2_plane queryplanes[8];
	struct v4l2_buffer querybuf, qbuf;
	unsigned int type;
	uint8_t *v4l2_data;
	uint32_t v4l2_size;
	int ret = V4L2_SUCCESS;
	void *m_input_ptr;

	if (!dec->input_setup) {
		//Set stream ON for input
		ret = vidioc_streamon(dec->handle, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
		if (ret != V4L2_SUCCESS)
		{
			printf ("%s: input vidioc_streamon failed\n", __func__ );
			return -1;
		}
		printf("Started stream on OUTPUT..\n");

		/* Memory mapping for input buffers in V4L2 */
		memset (&reqbuf, 0, sizeof reqbuf);
		reqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		reqbuf.count = NB_BUF_INPUT;
		reqbuf.memory = V4L2_MEMORY_MMAP;

		ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_REQBUFS, &reqbuf);
		if (ret != V4L2_SUCCESS)
		{
			printf("%s: %d ERROR: VIDIOC_REQBUFS Failed\n", __func__,__LINE__);
			goto err;
		}

		dec->mmap_virtual_input = malloc (sizeof (void *) * reqbuf.count);
		dec->mmap_size_input = malloc (sizeof (void *) * reqbuf.count);
	}

	memset(&querybuf, 0, sizeof(querybuf));
	memset(&queryplanes, 0, sizeof(queryplanes));
	querybuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	querybuf.memory = V4L2_MEMORY_MMAP;
	querybuf.m.planes = queryplanes;

	querybuf.index = index;
	dec->current_nb_buf_input++;

	/* Memory mapping for input buffers */
	ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_QUERYBUF, &querybuf);
	if (ret != V4L2_SUCCESS)
	{
		printf("%s: %d ERROR: VIDIOC_QUERYBUF Failed\n", __func__,__LINE__);
		goto err;
	}

	m_input_ptr = SynaV4L2_Mmap(dec->handle, NULL, querybuf.m.planes[0].length,
								PROT_READ | PROT_WRITE, MAP_SHARED,
								querybuf.m.planes[0].m.mem_offset);
	if (m_input_ptr == NULL)
	{
		printf("%s: %d ERROR: SynaV4L2_Mmap Failed\n", __func__,__LINE__);
		goto err;
	} else
		printf("%s: input memory mapped VA: %p\n", __func__, m_input_ptr);

	/* Queue V4L2 buffer */
	dec->mmap_virtual_input[index] = m_input_ptr;
	dec->mmap_size_input[index] = querybuf.m.planes[0].length;

	qbuf = querybuf;            /* index from querybuf */

	v4l2_data = dec->mmap_virtual_input[index];
	v4l2_size = dec->mmap_size_input[index];

	if(au_size > v4l2_size) {
		printf("%s:%d Error - au_size > v4l2_size\n", __FUNCTION__,__LINE__);
		goto err;
	}
	// Copy to mmap-ed memory
	memcpy(v4l2_data, au_ptr, au_size);
	qbuf.m.planes[0].bytesused = au_size;         /* enqueue it with data */

	ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_QBUF, &qbuf);
	if (ret != V4L2_SUCCESS)
	{
		printf("%s: %d ERROR: VIDIOC_QBUF Failed\n", __func__,__LINE__);
		goto err;
	}

	dec->input_setup = 1;

	return 0;

err:
	return -1;
}

int v4l2dec_setup_output(v4l2dec *dec)
{
	struct v4l2_buffer qbuf, querybuf, dqbuf;
	struct v4l2_plane queryplanes[8];
	struct v4l2_requestbuffers reqbuf;
	uint32_t v4l2_size, type;
	uint8_t *v4l2_data;
	int i, ret = V4L2_SUCCESS;

	//Set stream ON for output
	ret = vidioc_streamon(dec->handle, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
	if (ret != V4L2_SUCCESS)
	{
		printf ("%s: output vidioc_streamon failed\n", __func__ );
		return -1;
	}

	//Request for n count buffers
	memset (&reqbuf, 0, sizeof reqbuf);
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	reqbuf.count = NB_BUF_OUTPUT;
	reqbuf.memory = V4L2_MEMORY_MMAP;

	ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_REQBUFS, &reqbuf);
	if (ret != V4L2_SUCCESS)
	{
		printf("%s: %d ERROR: VIDIOC_REQBUFS Failed\n", __func__, __LINE__);
		goto err;
	}

	dec->mmap_virtual_output = malloc(sizeof (void *) * reqbuf.count);
	dec->mmap_size_output = malloc(sizeof (void *) * reqbuf.count);

	//query and memory mapping for input buffers
	memset(&querybuf, 0, sizeof querybuf);
	memset(&queryplanes, 0, sizeof(queryplanes));
	querybuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	querybuf.memory = V4L2_MEMORY_MMAP;
	querybuf.m.planes = queryplanes;

	for(i = 0; i < reqbuf.count; i++)
	{
		void *m_ptr;
		dec->current_nb_buf_output++;
		querybuf.index = i;

		/* Memory mapping for input buffers */
		ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_QUERYBUF, &querybuf);
		if (ret != V4L2_SUCCESS)
		{
			printf("%s: %d ERROR: VIDIOC_QUERYBUF Failed\n",__func__, __LINE__);
			goto err;
		}

		m_ptr = SynaV4L2_Mmap(dec->handle, NULL, querybuf.m.planes[0].length,
							  PROT_READ | PROT_WRITE, MAP_SHARED,
							  querybuf.m.planes[0].m.mem_offset);
		if (m_ptr == NULL)
		{
			printf("%s: %d ERROR: SynaV4L2_Mmap Failed\n",__func__, __LINE__);
			goto err;
		} else
			printf("%s: output memory mapped VA: %p\n", __func__, m_ptr);

		dec->mmap_virtual_output[i] = m_ptr;
		dec->mmap_size_output[i] = querybuf.m.planes[0].length;

		qbuf = querybuf;            /* index from querybuf */
		qbuf.m.planes[0].bytesused = 0;         /* enqueue it with no data */
		ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_QBUF, &qbuf);
		if (ret != V4L2_SUCCESS)
		{
			printf("%s: %d ERROR: VIDIOC_QBUF Failed\n",__func__, __LINE__);
			goto err;
		}
	}

	dec->output_setup = 1;

	return 0;
err:
	return -1;
}

void v4l2dec_frame_out(void *ptr)
{
	struct v4l2_buffer dqbuf, qbuf;
	uint8_t *v4l2_data;
	uint32_t v4l2_size;
	int ret = V4L2_SUCCESS;
	v4l2dec *dec = (v4l2dec *)ptr;

	if (!dec->session_active) {
		printf("decoder session is not active, returning\n");
		return;
	}

	if (!dec->output_setup && v4l2dec_setup_output(dec) < 0)
	{
		printf("v4l2dec_setup_output failed\n");
		return;
	}

	do
	{
		// we already submitted few frames, check if we have a output ready
		memset (&dqbuf, 0, sizeof dqbuf);
		dqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		dqbuf.memory = V4L2_MEMORY_MMAP;

		ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_DQBUF, &dqbuf);
		if (ret != V4L2_SUCCESS)
		{
			printf("ERROR: VIDIOC_DQBUF Failed while decode\n");
			//return;
		} else {
			v4l2_data = dec->mmap_virtual_output[dqbuf.index];
			v4l2_size = dqbuf.m.planes[0].bytesused;

			if (!v4l2_size || (dqbuf.flags & V4L2_BUF_FLAG_LAST)) {
				printf("stream closed\n");
				dec->EndOfStream = 1;
				return;
			}

			// send frame to application
			dec->frame_cb(v4l2_data, v4l2_size, dec->cb_data);

			qbuf = dqbuf;            /* index from querybuf */
			qbuf.m.planes[0].bytesused = 0;         /* enqueue it with no data */
			ret = SynaV4L2_Ioctl(dec->handle,VIDIOC_QBUF, &qbuf);
			if (ret != V4L2_SUCCESS)
			{
				printf("ERROR: VIDIOC_QBUF Failed\n");
			}
			dec->no_of_frames++;
			usleep(10000); //Added delay to sync so that buffers are available after decoder processing done
		}
	} while (dec->decoder_mode == DECODER_IN_STREAM_MODE);

	return;
}

int v4l2dec_decode(uint8_t *au_ptr, uint32_t au_size, v4l2dec *dec)
{
	struct v4l2_buffer dqbuf, qbuf;
	uint8_t *v4l2_data;
	uint32_t v4l2_size;
	int ret = V4L2_SUCCESS;

	if (dec->bH264DecoderConfigurationChanged)
	{
		printf("%s: bH264DecoderConfigurationChanged: %d\n", __func__, dec->bH264DecoderConfigurationChanged );
		// Port settings changed
		tsem_down(dec->videoH264DecoderEventSem);

		if(dec->mmap_virtual_output) {
			free(dec->mmap_virtual_output);
			free(dec->mmap_size_output);
		}

		v4l2dec_setup_output(dec);
		dec->bH264DecoderConfigurationChanged = 0;
	}

	if (dec->decoder_mode == DECODER_IN_STREAM_MODE) {
		//input setup such as request,query buff and feed input data
		if (v4l2dec_setup_input(dec, au_ptr, au_size, dec->current_nb_buf_input) < 0)
		{
			printf("v4l2dec_setup_input failed\n");
			goto err;
		}
		usleep(30000); //Delay added so that libsynav4l2 process resolution change event before setting up output interface
		//get the decoded frame and dump to a file
		v4l2dec_frame_out((void *)dec);
	} else {
		//input setup such as request,query buff and feed input data
		if (dec->current_nb_buf_input < NB_BUF_INPUT) {
			if (v4l2dec_setup_input(dec, au_ptr, au_size, dec->current_nb_buf_input) < 0) {
				printf("v4l2dec_setup_input failed\n");
				goto err;
			}
		} else {
			//get the decoded frame and dump to a file
			v4l2dec_frame_out((void *)dec);
decode:
			//Feed input to decoder
			memset (&dqbuf, 0, sizeof dqbuf);
			dqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
			dqbuf.memory = V4L2_MEMORY_MMAP;

			ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_DQBUF, &dqbuf);
			if (ret != V4L2_SUCCESS)
			{
				printf(" %s: %d ERROR: VIDIOC_DQBUF Failed, while subbmiting input\n", __func__, __LINE__);
				goto decode;
			}

			v4l2_data = dec->mmap_virtual_input[dqbuf.index];
			v4l2_size = dqbuf.m.planes[0].length;

			if(au_size > v4l2_size) {
				printf("%s:%d Error - au_size > v4l2_size\n", __FUNCTION__,__LINE__);
				goto err;
			}

			// Copy to mmap-ed memory
			memcpy(v4l2_data, au_ptr, au_size);

			/* Queue V4L2 buffer */
			qbuf = dqbuf;                 /* index from dqbuf */
			qbuf.m.planes[0].bytesused = au_size;

			ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_QBUF, &qbuf);
			if (ret != V4L2_SUCCESS)
			{
				printf("%s: %d ERROR: VIDIOC_QBUF Failed\n", __func__, __LINE__);
				goto err;
			}
		}
	}

	return 0;
err:
	return -1;
}

static void unmap_input_buf(v4l2dec *dec)
{
	int i;

	if (dec->mmap_virtual_input) {
		for (i = 0; i < dec->current_nb_buf_input; i++)
			SynaV4L2_Munmap(dec->handle, dec->mmap_virtual_input[i], dec->mmap_size_input[i]);
	}
}

static void unmap_output_buf(v4l2dec *dec)
{
	int i;

	if (dec->mmap_virtual_output) {
		for (i = 0; i < dec->current_nb_buf_output; i++)
			SynaV4L2_Munmap(dec->handle, dec->mmap_virtual_output[i], dec->mmap_size_output[i]);
	}
}

int v4l2dec_close(v4l2dec *dec)
{
	int type;
	int ret = V4L2_SUCCESS;

	dec->session_active = 0;
	dec->EndOfStream = 1;
	pthread_join(dec->v4l2_event_watcher_id, NULL);

	/* Stop streaming on output */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_STREAMOFF, &type);
	if (ret != V4L2_SUCCESS)
	{
		printf("ERROR: VIDIOC_STREAMOFF Failed on CAPTURE\n");
		return -1;
	}

	unmap_input_buf(dec);

	/* Stop streaming on input */
	type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = SynaV4L2_Ioctl(dec->handle, VIDIOC_STREAMOFF, &type);
	if (ret != V4L2_SUCCESS)
	{
		printf("ERROR: VIDIOC_STREAMOFF Failed on OUTPUT\n");
		return -1;
	}

	unmap_output_buf(dec);

	if(dec->mmap_virtual_output) {
		free(dec->mmap_virtual_output);
		free(dec->mmap_size_output);
	}

	if(dec->mmap_virtual_input) {
		free(dec->mmap_virtual_input);
		free(dec->mmap_size_input);
	}

	return 0;
}
