#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "watermark.h"
#include "ringbuf.h"

//#include "NDIlib_Send_Video.h"
#include "ndi_buf.h"

#define MAX_RINGBUF_NUM 32
#define MIN_RINGBUF_NUM 3
typedef struct ringbuf_t_ndi{
    unsigned char *pbuf[MAX_RINGBUF_NUM];
    int pos_read;
    int pos_write;
    int video_buf_size; //max alloc video size
    int buf_num;  // malloc buf num
    int data_num; // fill data num
	int audio_buf_size; //max alloc audio size
	int audio_size[MAX_RINGBUF_NUM]; //fill data audio size,

	//caculator fps
	double fps;
	double bitrate;
	int lost_packages;

} ringbuf_s;

typedef struct handle_ndi{
    void *handle;

}NDI_HANDLE;

NDI_HANDLE g_handle;
ringbuf_s g_ringbuf;
ringbuf_s g_ringbuf_audio;
static pthread_t tid_ndi_stream;
static pthread_mutex_t ndi_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t ndi_mutex_audio = PTHREAD_MUTEX_INITIALIZER;
int g_run = 0;
int g_time_cnt = 0;
int g_run_cnt = 0;
void *ndi_stream(void *hStream);
int ndi_buf_pull(void);
int ndi_buf_pull_audio(void);
struct  timeval   tv_begin,tv_end;


void show_fps()
{
	double msec;

    return ;// wdy add
	gettimeofday(&tv_end,NULL);
	msec = (tv_end.tv_sec - tv_begin.tv_sec) * 1000  +  (tv_end.tv_usec - tv_begin.tv_usec) / 1000;


	printf("video fps: %f, audio bit rate: %f; lost packages video %d, audio %d\n",  g_ringbuf.fps * 1000 / msec, g_ringbuf_audio.bitrate * 1000 / msec,g_ringbuf.lost_packages,g_ringbuf_audio.lost_packages);
	tv_begin.tv_sec = tv_end.tv_sec;
	tv_begin.tv_usec = tv_begin.tv_usec;

	//clear to zero
	g_ringbuf.fps = 0;
	g_ringbuf_audio.bitrate = 0;
	g_ringbuf.lost_packages = 0;
	g_ringbuf_audio.lost_packages = 0;
}

/*
void* ndi_stream(void *hStream)
{
    // get the usec, show no signal;
    g_time_cnt = 0;
    g_run = 1;

	gettimeofday(&tv_begin,NULL);
    while(g_run)
    {
        ndi_buf_pull();
		ndi_buf_pull_audio();
        usleep(2*1000);

    }
}
*/

void ndi_buf_task(void)
{
    void * tret;
    printf("ndi_buf_task ... \n");
    pthread_mutex_init(&ndi_mutex, NULL);
	pthread_mutex_init(&ndi_mutex_audio, NULL);
    //pthread_create(&tid_ndi_stream, NULL, (void*)ndi_stream, NULL);

    printf("ndi_buf_task done ... \n");

    //pthread_join(tid_ndi_stream, &tret);

    return;
}

static char *gBuf = NULL;
int ndi_buf_create(int video_size, int video_num,int audio_size, int audio_num)
{
    int i;
	printf("ndi video buf 20230607-v1.0.8 \n");
    int len=0;
    // creat video buffer
#if 0
    for(i = 0; i < video_num; i++)
    {
        g_ringbuf.pbuf[i]  = NULL;
        g_ringbuf.pbuf[i] = (unsigned char*)malloc(video_size);
        printf("ndi_buf_create video ... %d \n",i);
        if(g_ringbuf.pbuf[i] == NULL)
        {
            printf("malloc error\n");
            return -1;
        }

    }
    g_ringbuf.buf_num = video_num;
    g_ringbuf.pos_read = 0;
    g_ringbuf.pos_write = 0;
    g_ringbuf.data_num = 0;
    g_ringbuf.video_buf_size = video_size;
	g_ringbuf.fps = 0;
	g_ringbuf.bitrate = 0;
	g_ringbuf.lost_packages = 0;
	printf("ndi video buf create: size: %d, num : %d\n",video_size,video_num);
#endif

    len = 1920*1080*2; //yuv resolution
    gBuf = malloc (len);
    if(gBuf == NULL)
        printf("fatal error,malloc fail\n");

	//create audio buffer
	//clear buffer size
	for(i = 0; i < MAX_RINGBUF_NUM; i ++)
	{
		g_ringbuf_audio.audio_size[i] = 0;

	}
    for(i = 0; i < audio_num; i++)
    {
        g_ringbuf_audio.pbuf[i]  = NULL;
        g_ringbuf_audio.pbuf[i] = (unsigned char*)malloc(audio_size);
        printf("ndi_buf_create audio ... %d \n",i);
        if(g_ringbuf_audio.pbuf[i] == NULL)
        {
            printf("malloc error\n");
            return -1;
        }

    }
    g_ringbuf_audio.buf_num = audio_num;
    g_ringbuf_audio.pos_read = 0;
    g_ringbuf_audio.pos_write = 0;
    g_ringbuf_audio.data_num = 0;
    g_ringbuf_audio.audio_buf_size = audio_size;
	g_ringbuf_audio.fps = 0;
	g_ringbuf_audio.bitrate = 0;
	g_ringbuf_audio.lost_packages = 0;
    printf("ndi buf create: size: %d, num : %d\n",audio_size,audio_num);
    ndi_buf_task();
	g_run = 1;

    return 0;
}

int ndi_buf_push(unsigned char *pbuf, int size)
{
    int cpy_size = size;
    unsigned char *pdata;

    if(g_run == 0)
    {
        printf("ndi_buf_push wait buf task\n");
        return -1;
    }

    if(g_ringbuf.data_num >= g_ringbuf.buf_num - 2){
        //printf("too full, wait ring buffer pull  %d ... \n",g_ringbuf.data_num);
		g_ringbuf.lost_packages ++;
        return -1;
    }


	g_ringbuf.fps ++;
    //printf("--- ndi push buf: size: %d , data num: %d\n",size,g_ringbuf.data_num);
    pdata = g_ringbuf.pbuf[g_ringbuf.pos_write];
    if(cpy_size > g_ringbuf.video_buf_size)
    {
        cpy_size = g_ringbuf.video_buf_size;
    }
    memcpy(pdata,pbuf,cpy_size);

    pthread_mutex_lock(&ndi_mutex);
    g_ringbuf.data_num ++;
    g_ringbuf.pos_write ++;
    if(g_ringbuf.data_num > g_ringbuf.buf_num)
    {
        g_ringbuf.data_num = g_ringbuf.buf_num;
    }

    if(g_ringbuf.pos_write >= g_ringbuf.buf_num)
    {
        g_ringbuf.pos_write = 0;
    }

    pthread_mutex_unlock(&ndi_mutex);

    return 0;
}

void ndi_putbuf_nosignal(unsigned char * p_buf)
{
	get_bmp_buf(p_buf,1920*1080*2);
	return;

}

#if 0 //origin demo
int ndi_buf_pull_hdmi(unsigned char * p_buf)
{
    unsigned char *pdata;

    usleep(10*1000);
    return -1;

	g_time_cnt ++;
	g_run_cnt ++;
	if(g_run_cnt % 1000 == 0)
	{
		show_fps();
	}

    if(g_ringbuf.data_num < MIN_RINGBUF_NUM){
        //printf("wait ring buffer fill  %d ... \n",g_ringbuf.data_num);
        //10s empty buffer , show no signal;
         if(g_time_cnt > 100)
		 {
			ndi_putbuf_nosignal(p_buf);
			g_time_cnt = 0;
			return 0;
		 }

        return -1;
    }

    //data exist, timer cnt set to zero
    g_time_cnt = 0;

    pdata = g_ringbuf.pbuf[g_ringbuf.pos_read];


    // do something
	//memcpy(p_buf,pdata,1920*1080*2);

	//if(g_run_cnt % 100 == 0)
	{
		//printf("opencv demo, addwatermark\n");
		addwatermark(pdata,p_buf,1920*1080*2);
	}



    pthread_mutex_lock(&ndi_mutex);
    g_ringbuf.data_num --;
    if(g_ringbuf.data_num < 0)
    {
        g_ringbuf.data_num = 0;
    }
    g_ringbuf.pos_read ++;
    if(g_ringbuf.pos_read >= g_ringbuf.buf_num)
    {
        g_ringbuf.pos_read = 0;
    }
    pthread_mutex_unlock(&ndi_mutex);
    return 0;
}
#else
extern struct ringbuf_t gRingbuf ;
#define ONE_FRAME_SIZE  1920*1080*2
int ndi_buf_pull_hdmi(unsigned char * p_buf)
{
    unsigned char *pdata;
    int ret =0;

#if 0
    //pdata = g_ringbuf.pbuf[g_ringbuf.pos_read];

    if(ringbuf_len(&gRingbuf) < ONE_FRAME_SIZE){
        printf("rb empty,need fill rb\n");
        return -1;
    }

    ret = ringbuf_out(&gRingbuf, gBuf, ONE_FRAME_SIZE);
    //printf("rb out %d %d\n",ret,ringbuf_len(&gRingbuf));
    if(ret < ONE_FRAME_SIZE)
        printf("rb out less than one frame %d\n",ret);

	//addwatermark(gBuf,p_buf,ONE_FRAME_SIZE);
    memcpy(p_buf,gBuf,ONE_FRAME_SIZE);
#endif
    return 0;
}

#endif

int ndi_buf_push_audio(unsigned char *pbuf, int size)
{
    int cpy_size = size;
    unsigned char *pdata;

    if(g_run == 0)
    {
        printf("ndi_buf_push wait buf task\n");
        return -1;
    }
    if(g_ringbuf_audio.data_num >= g_ringbuf_audio.buf_num - 2){
        //printf("too full, wait ring buffer pull  %d ... \n",g_ringbuf_audio.data_num);
		g_ringbuf_audio.lost_packages ++;
        return -1;
    }

	g_ringbuf_audio.bitrate +=size;
    //printf("--- ndi push audio buf: size: %d ,cur data num: %d\n",size,g_ringbuf_audio.data_num);
    pdata = g_ringbuf_audio.pbuf[g_ringbuf_audio.pos_write];
    if(cpy_size > g_ringbuf_audio.audio_buf_size)
    {
        cpy_size = g_ringbuf_audio.audio_buf_size;
    }
    memcpy(pdata,pbuf,cpy_size);
	g_ringbuf_audio.audio_size[g_ringbuf_audio.pos_write] = cpy_size;

    pthread_mutex_lock(&ndi_mutex_audio);
    g_ringbuf_audio.data_num ++;
    g_ringbuf_audio.pos_write ++;
    if(g_ringbuf_audio.data_num > g_ringbuf_audio.buf_num)
    {
        g_ringbuf_audio.data_num = g_ringbuf_audio.buf_num;
    }

    if(g_ringbuf_audio.pos_write >= g_ringbuf_audio.buf_num)
    {
        g_ringbuf_audio.pos_write = 0;
    }

    pthread_mutex_unlock(&ndi_mutex_audio);

    return 0;
}
int ndi_buf_pull_audio(void)
{
    unsigned char *pdata;

    if(g_ringbuf_audio.data_num < MIN_RINGBUF_NUM){
		//wait buffer fill
        return -1;
    }

    pdata = g_ringbuf_audio.pbuf[g_ringbuf_audio.pos_read];
	int audio_read_len = g_ringbuf_audio.audio_size[g_ringbuf_audio.pos_read];
    // do something
    //ndi_putbuf_audio(pdata,audio_read_len);
    //printf("pull ring buffer ... g_ringbuf_audio.data_num %d\n",g_ringbuf_audio.data_num);

    pthread_mutex_lock(&ndi_mutex_audio);
    g_ringbuf_audio.data_num --;
    if(g_ringbuf_audio.data_num < 0)
    {
        g_ringbuf_audio.data_num = 0;
    }
    g_ringbuf_audio.pos_read ++;
    if(g_ringbuf_audio.pos_read >= g_ringbuf_audio.buf_num)
    {
        g_ringbuf_audio.pos_read = 0;
    }
    pthread_mutex_unlock(&ndi_mutex_audio);
    return 0;
}

void ndi_buf_stop()
{
    g_run = 0;
    pthread_mutex_destroy(&ndi_mutex);
	pthread_mutex_destroy(&ndi_mutex_audio);

}


