#include "v4l2enc_test.h"

uint8_t frame_buf[FRAME_BUF_SIZE]; // Buffer holds one FRAME, read from a file

void print_usage()
{
    printf("\nUsage: ./v4l2enc_test -i <input RAW(YUV) file> -o <output h264 file> -w <width> -h <height>\n");
    printf("Default Out File - out.h264\n");
    printf("Default wxh      - 1920x1080\n");
    printf("Default controls will be used\n");
    return;
}

HRESULT v4l2enc_test_streamon (void *handle, enum v4l2_buf_type buf_type)
{
    HRESULT ret = -1;
    ret = SynaV4L2_Ioctl(handle, VIDIOC_STREAMON, (void*) &buf_type);
    if (ret != 0)
    {
        printf ("VIDIOC_STREAMON failed = %d @ %d\n", ret, __LINE__);
    }
    return ret;
}

void *dqbufEnc_output_watcher(void *data)
{
    HRESULT ret = -1;
    struct v4l2_buffer dqbuf;
    struct v4l2_enc *v4l2enc = (struct v4l2_enc *)data;

    while (!v4l2enc->startSession)
    {
        // wait for the first data being sent to encoder
        usleep(10000);
    }

    while(!v4l2enc->eos)
    {
        memset (&dqbuf, 0, sizeof dqbuf);
        dqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        dqbuf.memory = V4L2_MEMORY_MMAP;

        ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_DQBUF, &dqbuf);
        if (ret != V4L2_SUCCESS)
        {
            if(ret != ERROR_EAGAIN)
            {
                printf("ERROR: VIDIOC_DQBUF Failed on output: %d\n", ret);
                break;
            }
            usleep(10000);
        }
        else
        {
            v4l2enc->bBufwithEnc[dqbuf.index] = 0;
        }
    }
}

HRESULT v4l2enc_setup_input(struct v4l2_enc *v4l2enc)
{
    struct v4l2_requestbuffers rqBuff;
    struct v4l2_plane qryPlanes[8];
    struct v4l2_buffer qryBuff, queBuff;
    int i;
    void *mmap_ptr = NULL;
    HRESULT ret = -1;

    memset (&rqBuff, 0, sizeof (rqBuff));
    rqBuff.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    rqBuff.count = NB_BUF_INPUT;
    rqBuff.memory = V4L2_MEMORY_MMAP;

    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_REQBUFS, (void*) &rqBuff);
    if (ret != 0)
    {
        printf ("VIDIOC_REQBUFS failed = %d @ %d\n", ret, __LINE__);
        return -1;
    }

    v4l2enc->mmap_virt_inp = malloc (sizeof (void *) * rqBuff.count);
    v4l2enc->mmap_size_inp = malloc (sizeof (void *) * rqBuff.count);

    memset (&qryBuff, 0, sizeof (qryBuff));
    memset (&qryPlanes, 0, sizeof (qryPlanes));
    qryBuff.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    qryBuff.memory = V4L2_MEMORY_MMAP;
    qryBuff.m.planes = qryPlanes;

    v4l2enc->session_active = 1;
    for(i = 0; i < rqBuff.count; i++){
        qryBuff.index = i;

        ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QUERYBUF, &qryBuff);
        if (ret != 0)
        {
            printf ("VIDIOC_QUERYBUF failed = %d @ %d\n", ret, __LINE__);
            return -1;
        }

        printf ("qryBuff.length = %d\t\tqryBuff.m.offset = %x\n", qryBuff.m.planes[0].length, qryBuff.m.planes[0].m.mem_offset);
        mmap_ptr = SynaV4L2_Mmap(v4l2enc->handle, NULL, qryBuff.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, qryBuff.m.planes[0].m.mem_offset);
        printf ("ret for SynaV4L2_Mmap = %p\n", mmap_ptr);
        if (mmap_ptr == NULL)
        {
            printf ("SynaV4L2_Mmap FAILED\n");
            return -1;
        }

        v4l2enc->mmap_virt_inp[i] = mmap_ptr;
        v4l2enc->mmap_size_inp[i] = qryBuff.m.planes[0].length;

        /* Queue empty V4L2 buffer */
        queBuff = qryBuff;
        queBuff.m.planes[0].bytesused = v4l2enc->frame_read_size;
        v4l2enc->encInpBuf[i] = queBuff;
        v4l2enc->bBufwithEnc[i] = 0;//means encoder doesn't have any initial buffers
#if 0
        ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QBUF, &queBuff);
        if (ret != 0)
        {
            printf ("VIDIOC_QBUF failed = %d @ %d\n", ret, __LINE__);
            return -1;
        }
#endif
    }
    return ret;
}

int enc_get_input_handle(struct v4l2_enc *v4l2enc, int32_t *index)
{
    struct v4l2_buffer dqbuf;
    HRESULT ret = -1;
    int i;
    int j = v4l2enc->last_index;

    *index = -1;
    for (i = 0; i < NB_BUF_INPUT; i++)
    {
        if(++j == NB_BUF_INPUT)
            j=0;

        if (v4l2enc->bBufwithEnc[j] == 0)
        {
            *index = j;
            v4l2enc->last_index = j;
            ret = 0;
            return ret;
        }
    }
    return ret;
}

HRESULT v4l2enc_setup_output(struct v4l2_enc *v4l2enc)
{
    struct v4l2_requestbuffers rqBuff;
    int i;
    struct v4l2_plane qryPlanes[8];
    struct v4l2_buffer qryBuff, queBuff;
    void *mmap_ptr = NULL;
    HRESULT ret = -1;

    memset (&rqBuff, 0, sizeof (rqBuff));
    rqBuff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    rqBuff.count = NB_BUF_OUTPUT;
    rqBuff.memory = V4L2_MEMORY_MMAP;

    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_REQBUFS, (void*) &rqBuff);
    if (ret != 0)
    {
        printf ("VIDIOC_REQBUFS failed = %d @ %d\n", ret, __LINE__);
        return -1;
    }

    v4l2enc->mmap_virt_out = malloc (sizeof (void *) * rqBuff.count);
    v4l2enc->mmap_size_out = malloc (sizeof (void *) * rqBuff.count);

    memset (&qryBuff, 0, sizeof (qryBuff));
    memset (&qryPlanes, 0, sizeof (qryPlanes));
    qryBuff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    qryBuff.memory = V4L2_MEMORY_MMAP;
    qryBuff.m.planes = qryPlanes;

    for(i = 0; i < rqBuff.count; i++){
        qryBuff.index = i;

        ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QUERYBUF, &qryBuff);
        if (ret != 0)
        {
            printf ("VIDIOC_QUERYBUF failed = %d @ %d\n", ret, __LINE__);
            return -1;
        }

        printf ("qryBuff.length = %d\t\tqryBuff.m.offset = %x\n", qryBuff.m.planes[0].length, qryBuff.m.planes[0].m.mem_offset);
        mmap_ptr = SynaV4L2_Mmap(v4l2enc->handle, NULL, qryBuff.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, qryBuff.m.planes[0].m.mem_offset);
        printf ("ret for SynaV4L2_Mmap = %p\n", mmap_ptr);
        if (mmap_ptr == NULL)
        {
            printf ("SynaV4L2_Mmap FAILED\n");
            return -1;
        }

        v4l2enc->mmap_virt_out[i] = mmap_ptr;
        v4l2enc->mmap_size_out[i] = qryBuff.m.planes[0].length;

        // Queue empty buffers
        queBuff = qryBuff;
        queBuff.m.planes[0].bytesused = 0;
        queBuff.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QBUF, &queBuff);
        if (ret != 0)
        {
            printf ("VIDIOC_QBUF failed = %d @ %d\n", ret, __LINE__);
            return -1;
        }
    }

    return ret;
}

HRESULT v4l2enc_set_formats(struct v4l2_enc *v4l2enc, int width, int height)
{
    HRESULT ret = -1;
    struct v4l2_format v4l2enc_infmt;
    struct v4l2_format v4l2enc_ingfmt;
    struct v4l2_format v4l2enc_outfmt;
    struct v4l2_format v4l2enc_outgfmt;
    enum v4l2_buf_type buf_type;
    struct v4l2_streamparm param;

    //Output Set Format for Output
    memset (&v4l2enc_infmt, 0, sizeof v4l2enc_infmt);
    v4l2enc_infmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    v4l2enc_infmt.fmt.pix_mp.width = width;
    v4l2enc_infmt.fmt.pix_mp.height = height;
    v4l2enc_infmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_S_FMT, (void*) &v4l2enc_infmt);
    if (ret != 0)
    {
        printf ("SynaV4L2_Ioctl for Output VIDIOC_S_FMT failed = %d\n", ret);
        return -1;
    }

    // Set Frame-rate Param
    memset (&param, 0, sizeof param);
    param.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    param.parm.output.timeperframe.numerator = FRAME_RATE_NUM;
    param.parm.output.timeperframe.denominator = FRAME_RATE_DEN;
    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_S_PARM, (void*) &param);
    if (ret != 0)
    {
        printf ("SynaV4L2_Ioctl for Output VIDIOC_S_PARM failed = %d\n", ret);
        return -1;
    }

    memset (&v4l2enc_ingfmt, 0, sizeof v4l2enc_ingfmt);
    v4l2enc_ingfmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_G_FMT, (void*) &v4l2enc_ingfmt);
    if (ret != 0)
    {
        printf ("SynaV4L2_Ioctl for Output VIDIOC_G_FMT failed = %d\n", ret);
        return -1;
    }

    buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    ret = v4l2enc_test_streamon (v4l2enc->handle, buf_type);
    if (ret != 0)
    {
        printf ("v4l2enc_test_streamon failed = %d @ %d\n", ret, __LINE__);
        return -1;
    }

    //Output Set Format for Capture
    memset (&v4l2enc_outfmt, 0, sizeof v4l2enc_outfmt);
    v4l2enc_outfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    v4l2enc_outfmt.fmt.pix_mp.width = width;
    v4l2enc_outfmt.fmt.pix_mp.height = height;
    v4l2enc_outfmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_H264;
    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_S_FMT, (void*) &v4l2enc_outfmt);
    if (ret != 0)
    {
        printf ("SynaV4L2_Ioctl for Capture VIDIOC_S_FMT failed = %d\n", ret);
        return -1;
    }
    memset (&v4l2enc_outgfmt, 0, sizeof v4l2enc_outgfmt);
    v4l2enc_outgfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    ret = SynaV4L2_Ioctl(v4l2enc->handle, VIDIOC_G_FMT, (void*) &v4l2enc_outgfmt);
    if (ret != 0)
    {
        printf ("SynaV4L2_Ioctl for Capture VIDIOC_G_FMT failed = %d\n", ret);
        return -1;
    }

    buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    ret = v4l2enc_test_streamon (v4l2enc->handle, buf_type);
    if (ret != 0)
    {
        printf ("v4l2enc_test_streamon failed = %d @ %d\n", ret, __LINE__);
    }

    return ret;
}

#ifndef USE_THREADED_OUTPUT
HRESULT v4l2enc_data_out(struct v4l2_enc *v4l2enc)
#else
void v4l2enc_data_out(struct v4l2_enc *v4l2enc)
#endif
{
    struct v4l2_buffer dqbuf;
    struct v4l2_buffer qbuf;
    uint8_t *v4l2_data;
    uint32_t v4l2_size;
    HRESULT ret = -1;
#ifdef USE_THREADED_OUTPUT
    while (1)
#endif
    {
        //printf ("Inside %s @ %d\n", __func__, __LINE__);
#ifdef USE_THREADED_OUTPUT
        pthread_mutex_lock (&(v4l2enc->mutex_lock));
#endif
        memset (&dqbuf, 0, sizeof dqbuf);
        dqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        dqbuf.memory = V4L2_MEMORY_MMAP;
        //printf("waiting for output--->\n");
        ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_DQBUF, &dqbuf);
        if (ret != 0) {
            //printf("Error: VIDIOC_DQBUF Failed, while encoding %d\n", ret);
            if((!v4l2enc->session_active) && !(v4l2enc->eos))
            {
                printf ("Session is still not active\n");
                usleep(50*1000);
#ifdef USE_THREADED_OUTPUT
                pthread_mutex_unlock (&(v4l2enc->mutex_lock));
                continue;
#endif
            }
            else if (v4l2enc->eos)
            {
#ifndef USE_THREADED_OUTPUT
                return ret;
#else
                pthread_mutex_unlock (&(v4l2enc->mutex_lock));
                return;
#endif
            }
        }
        else {
            v4l2_data = v4l2enc->mmap_virt_out[dqbuf.index];
            v4l2_size = dqbuf.m.planes[0].bytesused;

            //printf("%s -> length %d -> bytesused %x -> mem_offset %x -> data_offset %x\n", __func__, dqbuf.m.planes[0].length, dqbuf.m.planes[0].bytesused, dqbuf.m.planes[0].m.mem_offset, dqbuf.m.planes[0].data_offset);
            // send access unit to application
            //printf("data of %d copied to output mmap memory\n", v4l2_size);
            v4l2enc->data_cb(v4l2_data, v4l2_size, v4l2enc->cb_data);

            qbuf = dqbuf;            /* index from querybuf */
            qbuf.m.planes[0].bytesused = 0;         /* enqueue it with no data */
            ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QBUF, &qbuf);
            if (ret != 0) {
                printf("ERROR: VIDIOC_QBUF Failed during encode process %d\n", ret);
            }
        }
#ifdef USE_THREADED_OUTPUT
        pthread_mutex_unlock (&(v4l2enc->mutex_lock));
#endif
        usleep(10 * 1000);
    }

#ifndef USE_THREADED_OUTPUT
    return ret;
#endif
}

int write_out_data_to_file (void *data_ptr, int data_size, void *cb_data)
{
    FILE *fpOp = (FILE *)cb_data;

    //printf("Received data from encoder of size = %d\n", data_size);
    return fwrite(data_ptr, 1, data_size, fpOp);
}

int v4l2enc_test_encode(uint8_t *frame_ptr, uint32_t frame_size,
                        struct v4l2_enc *v4l2enc, int index)
{
    struct v4l2_buffer dqbuf;
    struct v4l2_buffer qbuf;
    uint8_t *v4l2_data;
    uint32_t v4l2_size;
    HRESULT ret = -1;

    //printf ("Inside %s at# %d\n", __func__, __LINE__);

#ifdef USE_THREADED_OUTPUT
    pthread_mutex_lock (&(v4l2enc->mutex_lock));
#endif
#ifndef USE_THREADED_OUTPUT
    ret = v4l2enc_data_out(v4l2enc);
#endif

#if 0
    memset (&dqbuf, 0, sizeof dqbuf);
    dqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    dqbuf.memory = V4L2_MEMORY_MMAP;

    ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_DQBUF, &dqbuf);
    if (ret != 0) {
        printf("Error: VIDIOC_DQBUF Failed, while submitting input to encoder %d\n", ret);
        goto err;
    }
#endif
    //printf("%s -> length %d -> bytesused %x -> mem_offset %x -> data_offset %x\n", __func__, dqbuf.m.planes[0].length, dqbuf.m.planes[0].bytesused, dqbuf.m.planes[0].m.mem_offset, dqbuf.m.planes[0].data_offset);

    qbuf = v4l2enc->encInpBuf[index];
    v4l2_data = v4l2enc->mmap_virt_inp[index];
    v4l2_size = qbuf.m.planes[0].length;

    if(frame_size > v4l2_size) {
        printf("%s:%d Error - frame_size(%d) > v4l2_size(%d)\n", __func__ ,__LINE__,frame_size, v4l2_size);
        goto err;
    }

    // Copy to mmap-ed memory
    //printf("data of %d copied to input mmap memory\n", frame_size);
    memcpy(v4l2_data, frame_ptr, frame_size);

    /* Queue V4L2 buffer */
    v4l2enc->bBufwithEnc[index] = 1;
    qbuf.m.planes[0].bytesused = frame_size;
    ret = SynaV4L2_Ioctl (v4l2enc->handle, VIDIOC_QBUF, &qbuf);
    if (ret != 0) {
        printf("ERROR: VIDIOC_QBUF Failed %d\n", ret);
    }
err:
#ifdef USE_THREADED_OUTPUT
    pthread_mutex_unlock (&(v4l2enc->mutex_lock));
#endif
    //printf ("returning from %s with ret = %d\n", __func__, ret);
    return ret;
}

int main (int argc, char *argv[])
{
    HRESULT ret = -1;
    FILE *fpi = NULL;
    FILE *fpo = NULL;
    char f_in_location[256];// = "/data/raw_file_example.raw";
    char f_out_location[256];// = "/data/encoded_file.h264";
    int width = 0;
    int height = 0;
    // Input File Read Pointer
    uint8_t *read_ptr = NULL;
    long long int bytes_read = 0;
    uint32_t in_size = 0;
    enum v4l2_buf_type buf_type;
    uint32_t frame_size = 0;
    int8_t option;
    int index;
    struct v4l2_enc v4l2enc;
    memset (&v4l2enc, 0, sizeof v4l2enc);
    memset(f_in_location, 0, sizeof (f_in_location));
    memset(f_out_location, 0, sizeof (f_out_location));
    // TODO: Use strncpy instead of strcpy
    while ((option = getopt(argc, argv,"i:o:w:h:")) != -1) {
        switch (option) {
            case 'i': strcpy(f_in_location, optarg);
                      break;
            case 'o': strcpy(f_out_location, optarg);
                      break;
            case 'w': width = atoi(optarg);
                      break;
            case 'h': height = atoi(optarg);
                      break;
            default:
                      print_usage();
                      exit(EXIT_FAILURE);
        }
    }
    v4l2enc.frame_read_size = (width * height * 3 / 2);
    int bytes_to_read = 0;
    int read_flag = 1;
    int inIndex = 0;
    //printf ("Input args are\n");
    //printf ("Input file %s\n", f_in_location);
    //printf ("Output file %s\n", f_out_location);
    //printf ("width %d\n", width);
    //printf ("height %d\n", height);
    if ((width == 0) || (height == 0))
    {
        printf ("Invalid resolution\n");
        exit(-1);
    }
    fpi = fopen (f_in_location,"rb");
    if (NULL == fpi)
    {
        printf ("Invalid input file pointer\n");
        return -1;
    }
    fpo = fopen (f_out_location,"wb");
    if (NULL == fpo)
    {
        printf ("Invalid output file pointer\n");
        return -1;
    }
    v4l2enc.eos = 0;
    v4l2enc.last_index = -1;
    v4l2enc.startSession = 0;

    //Set the callback pointers so as to write the encoded data to a file
    v4l2enc.data_cb =  write_out_data_to_file;
    v4l2enc.cb_data = (void *) fpo;


    ret = SynaV4L2_Init();
    if (ret != 0)
    {
        printf ("SynaV4L2_Init failed = %d\n", ret);
        return -1;
    }

    ret = SynaV4L2_Open((void *)&(v4l2enc.handle), ENCODER, V4L2_PIX_FMT_H264, 0);
    if (ret != 0)
    {
        printf ("SynaV4L2_Open failed = %d\n", ret);
        return -1;
    }
    //printf ("SynaV4L2_Open with handle = %p\n", handle);

    ret = v4l2enc_set_formats(&v4l2enc, width, height);
    if (ret != 0)
    {
        printf ("v4l2enc_set_formats failed = %d\n", ret);
        return -1;
    }

    ret = v4l2enc_setup_input(&v4l2enc);
    printf ("v4l2enc_setup_input rets = %d\n", ret);
    ret = v4l2enc_setup_output(&v4l2enc);
    printf ("v4l2enc_setup_output rets = %d\n", ret);
#ifdef USE_THREADED_OUTPUT
    if (pthread_mutex_init(&(v4l2enc.mutex_lock), NULL) != 0)
    {
        printf("\n Mutex Initialization Failed\n");
        return 1;
    }
    pthread_create(&(v4l2enc.thread_id_dqbuf), NULL, (void *)dqbufEnc_output_watcher, (void *) (&v4l2enc));
    pthread_create(&(v4l2enc.thread_id), NULL, (void *)v4l2enc_data_out, (void *) (&v4l2enc));
#endif
    v4l2enc.startSession = 1;
    read_ptr = frame_buf;
    usleep(10*1000);
    while (1)
    {
        bytes_to_read = v4l2enc.frame_read_size;
        if ((!v4l2enc.eos) && read_flag)
        {
            bytes_read = fread(read_ptr, 1, bytes_to_read, fpi);
            if (bytes_read != bytes_to_read)
            {
                //printf ("fread read less then requested read=%lld req=%d\n", bytes_read, bytes_to_read);
                in_size = v4l2enc.frame_read_size;
                v4l2enc.eos = 1;
            }
            else
                in_size = bytes_read;
        }

        // just encode a single frame for now
        frame_size = in_size;

        if (frame_size) {
            // get free handle to feed the data in
            ret = enc_get_input_handle(&v4l2enc, &inIndex);
            if (ret != 0 && ret != ERROR_EAGAIN) {
                printf("enc_get_input_handle failed..\n");
                break;
            }
            // Got one frame, feed it to encoder
            ret = v4l2enc_test_encode(read_ptr, frame_size, &v4l2enc, inIndex);
            if (ret != 0 && ret != ERROR_EAGAIN) {
                printf("Enccoding failed..\n");
                break;
            }
            else if (ret == ERROR_EAGAIN) {
                // Input injection should be tried again.
                // So don't read a new buffer.
                read_flag = 0;
            }
            else {
                // Set to read a new buffer
                read_flag = 1;
            }

            // Halt for some time so that encoder is not overloaded.
            // Considering 30fps, it should take 33ms to encode per frame.
            usleep(10*1000);
        }
        if(v4l2enc.eos)
            break; // EOF reached

    }

#ifndef USE_THREADED_OUTPUT
    // process remaining buffers
    ret = v4l2enc_data_out((&v4l2enc));
    if (ret != 0 && ret != ERROR_EAGAIN) {
        printf("Enccoding failed at EOS..\n");
    }
#endif

    //wait for encoder to output last data
    //printf ("wait for the encode process comp for pending buffers\n");
    usleep (1*1000*1000);

    //Start the Clean-up Activity
    printf("Starting the clean up activity for v4l2enc_test\n");

    //set the session to be closed
    v4l2enc.session_active = 0;

    //close the thread
#ifdef USE_THREADED_OUTPUT
    pthread_join(v4l2enc.thread_id, NULL);
    pthread_join(v4l2enc.thread_id_dqbuf, NULL);
#endif

    //Stream OFF for output and capture streams
    buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    ret = SynaV4L2_Ioctl(v4l2enc.handle, VIDIOC_STREAMOFF, (void*) &buf_type);
    if (ret != 0)
    {
        printf ("VIDIOC_STREAMOFF failed = %d @ %d\n", ret, __LINE__);
        return -1;
    }

    buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    ret = SynaV4L2_Ioctl(v4l2enc.handle, VIDIOC_STREAMOFF, (void*) &buf_type);
    if (ret != 0)
    {
        printf ("VIDIOC_STREAMOFF failed = %d @ %d\n", ret, __LINE__);
        return -1;
    }

    //munmap for input
    for (index = 0; index < NB_BUF_INPUT; index++)
    {
        if (0 != SynaV4L2_Munmap(v4l2enc.handle, v4l2enc.mmap_virt_inp[index], v4l2enc.mmap_size_inp[index]))
        {
            printf("Input Munmap failed for index: %d\n", index);
            return -1;
        }
    }

    //munmap for output
    for (index = 0; index < NB_BUF_OUTPUT; index++)
    {
        if (0 != SynaV4L2_Munmap(v4l2enc.handle, v4l2enc.mmap_virt_out[index], v4l2enc.mmap_size_out[index]))
        {
            printf("Output Munmap failed for index: %d\n", index);
            return -1;
        }
    }

    //close the file pointers
    if (fpi != NULL)
    {
        fclose (fpi);
    }
    if (fpo != NULL)
    {
        fclose (fpo);
    }
    free (v4l2enc.mmap_virt_inp);
    v4l2enc.mmap_virt_inp = NULL;
    free (v4l2enc.mmap_size_inp);
    v4l2enc.mmap_size_inp = NULL;
    free (v4l2enc.mmap_virt_out);
    v4l2enc.mmap_virt_out = NULL;
    free (v4l2enc.mmap_size_out);
    v4l2enc.mmap_size_out = NULL;

#ifdef USE_THREADED_OUTPUT
    pthread_mutex_destroy(&(v4l2enc.mutex_lock));
#endif

    //Close SynaV4L2
    ret = SynaV4L2_Close(v4l2enc.handle);
    printf ("SynaV4L2_Close returned = %d\n", ret);

    //Deinit SynaV4L2
    ret = SynaV4L2_Deinit();

    printf ("SynaV4L2_Deinit returned = %d\n", ret);
    return 0;
}

