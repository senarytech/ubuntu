/*******************************************************************************
*                Copyright 2013, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

/** @file voicecapture.c
 *  @brief create/destroy ALSA input device whenever desired BT remote control
 *  connected/disconnected, send open/close microphone command to remote control
 *  when ALSA device is triggered, fetch voice data from hidraw and feed into
 *  the ALSA input device.
 */

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <pthread.h>
#include <time.h>

#define MRVL_VENDOR_ID 0x0048
#define MRVL_PRODUCT_ID 0x0064
#define MRVL_VOICE_REPORT_ID 0x07
static const unsigned char mrvl_mic_open[6] = {0x78, 0x04, 0x00, 0x00, 0x00, 0x01};
static const unsigned char mrvl_mic_close[6] = {0x78, 0x04, 0x00, 0x00, 0x00, 0x00};

#define HISENSE_VENDOR_ID 0x7045
#define HISENSE_PRODUCT_ID 0x2828
#define HISENSE_VOICE_REPORT_ID 0x09
static const unsigned char hisense_mic_open[9] = {0x5a, 0x01, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00};
static const unsigned char hisense_mic_close[9] = {0x5a, 0x02, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00};

static const unsigned int EPOLL_ID_INOTIFY = 0x80000001;
static int g_is_desired_rc_connected = 0;
static int g_vendor_id = MRVL_VENDOR_ID;
static int g_product_id = MRVL_PRODUCT_ID;
static unsigned char g_voice_report_id = MRVL_VOICE_REPORT_ID;

typedef struct hidraw_thread{
	int hidraw_num;
	int hidraw_fd;
	pthread_t hidraw_work;
	int is_event_thread_exit;
	int bt_alsa_dev;
	int is_alsa_started;
	unsigned char *mic_open_cmd;
	unsigned char *mic_close_cmd;
	int mic_cmd_length;
	timer_t timerid;
	unsigned char voice_report_id;
}hidraw_thread, *hidraw_thread_t;

static hidraw_thread g_hidraw_thread;

//#define __MRVL_DBG
#ifdef __MRVL_DBG
static void hexdump_1(char *prompt, unsigned char * buf, int len)
{
	int i;
	char dbgdumpbuf[4096];
	char *ptr = dbgdumpbuf;

	printf("%s: len=%d\n", prompt, len);
	for (i = 0; i < len; i++) {
		ptr += snprintf(ptr, 4, "%02x", *buf);
		buf++;
	}
	printf("buf content: %s\n", dbgdumpbuf);
}
#endif

static int setup_voice_capture_param(hidraw_thread_t p)
{
	struct hidraw_devinfo info;
	int rc = ioctl(p->hidraw_fd, HIDIOCGRAWINFO, &info);
	if (rc < 0) {
		printf("HIDIOCGRAWINFO error %s\n", strerror(errno));
		return -1;
	}

	if (info.vendor == HISENSE_VENDOR_ID && info.product == HISENSE_PRODUCT_ID){
		p->mic_open_cmd = hisense_mic_open;
		p->mic_close_cmd = hisense_mic_close;
		p->mic_cmd_length = 9;
		p->voice_report_id = HISENSE_VOICE_REPORT_ID;
	}
	else if (info.vendor == g_vendor_id && info.product == g_product_id){
		p->mic_open_cmd = mrvl_mic_open;
		p->mic_close_cmd = mrvl_mic_close;
		p->mic_cmd_length = 6;
		p->voice_report_id = g_voice_report_id;
	}
	else {
		printf("not desired hid device.\n");
		return -1;
	}

	return 0;
}

static int init_epoll_fd()
{
	int epoll_fd = epoll_create(1);
	if (epoll_fd < 0){
		printf("%s %d %s (%d)",__func__, __LINE__, strerror(errno), errno);
		return -1;
	}
	return epoll_fd;
}

static int init_inotify(char * dev_path, int epoll_fd)
{
	int inotify_fd = inotify_init();
	if(inotify_fd < 0){
		printf("inotify_init failed, errno %s",strerror(errno));
		return -1;
	}

	int result = inotify_add_watch(inotify_fd, dev_path, IN_DELETE | IN_CREATE);
	if(result < 0){
		printf("Could not register INotify for %s  errno %s", dev_path, strerror(errno));
		close(inotify_fd);
		return -1;
	}

	return inotify_fd;

}

static int add_inotify_to_epoll(int epoll_fd, int inotify_fd)
{
	struct epoll_event eventItem;
	memset(&eventItem, 0, sizeof(eventItem));
	eventItem.events = EPOLLIN;
	eventItem.data.u32 = EPOLL_ID_INOTIFY;
	int result = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &eventItem);
	if (result != 0){
		printf("Could not add INotify to epoll instance.  errno %s", strerror(errno));
		return -1;
	}
	return 0;
}

static int write_to_dev_file(int fd, unsigned char *data, int data_len)
{
	int data_write = 0;

#ifdef __MRVL_DBG
	hexdump_1(__func__, data, data_len);
#endif

	data_write = TEMP_FAILURE_RETRY(write(fd, data, data_len));
	if (data_write < 0 || data_write != data_len){
		printf("%s failed:;len:%d %s (%d)",__func__,
				data_write, strerror(errno), errno);
		return -1;
	}

	return 0;
}
static void send_mic_open_cmd(union sigval sigev_val)
{
	hidraw_thread_t p = (hidraw_thread_t)sigev_val.sival_ptr;
	if (p->is_alsa_started){
		write_to_dev_file(p->hidraw_fd, p->mic_open_cmd, p->mic_cmd_length);
	}
	return;
}

static void start_timer_for_open_cmd(hidraw_thread_t p)
{
	struct sigevent sev;
	memset(&sev, 0, sizeof( struct sigevent));
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = send_mic_open_cmd;
	sev.sigev_value.sival_ptr = p;
	if (timer_create(CLOCK_REALTIME, &sev, &p->timerid) == -1){
		return;
	}

	printf("timer ID is 0x%x\n", p->timerid);

	struct itimerspec its;
	its.it_value.tv_sec = 10;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = 10;
	its.it_interval.tv_nsec = 0;
	if (timer_settime(p->timerid, 0, &its, NULL) == -1){
		printf("timer_settime failed, err %s\n", strerror(errno));
		timer_delete(p->timerid); p->timerid = 0;
		return;
	}

}
static int send_cmd_over_hidraw(hidraw_thread_t p)
{
	int rc = 0;
	if (p->is_alsa_started){
		rc = write_to_dev_file(p->hidraw_fd, p->mic_open_cmd, p->mic_cmd_length);
		start_timer_for_open_cmd(p);
	}
	else{
		if (p->timerid != 0){
			timer_delete(p->timerid); p->timerid = 0;
		}
		rc = write_to_dev_file(p->hidraw_fd, p->mic_close_cmd, p->mic_cmd_length);
	}
	return rc;
}

static int process_read_data(unsigned char * buffer,
		int data_len, int fd_num, hidraw_thread_t p)
{
	int rc = 0;

#ifdef __MRVL_DBG
	hexdump_1(__func__, buffer, data_len);
#endif
	if (fd_num == 1){
		p->is_alsa_started = *(int *)buffer;
		printf("bt alsa device start/stoped %d\n", p->is_alsa_started);
		rc = send_cmd_over_hidraw(p);
	}
	else if (fd_num == 0 && *buffer == p->voice_report_id && p->is_alsa_started){
		if (data_len == 2){
			printf("FIXME:skip voice start/stop report packet.\n");
			return 0;
		}
		rc = write_to_dev_file(p->bt_alsa_dev, buffer+1, data_len-1);
	}
	return rc;
}

static int check_poll_result(struct pollfd *poll_fd,
		int fd_num, hidraw_thread_t p)
{
	unsigned char   buffer[1024];

	if(poll_fd->revents == 0)
		return 0;

	if(poll_fd->revents != POLLIN){
		printf("Error! revents = %d\n", poll_fd->revents);
		return -1;
	}

	int rc = TEMP_FAILURE_RETRY(read(poll_fd->fd, buffer, sizeof(buffer)));
	if (rc < 0){
		printf("read() failed, error %s", strerror(errno));
		return -1;
	}

	if (rc == 0){
		printf("hidraw closed\n");
		return -1;
	}

	process_read_data(buffer, rc, fd_num, p);

	return 0;
}


static void *read_hidraw_func(void *args)
{
	hidraw_thread_t p = (hidraw_thread_t) args;
	int i;
	struct pollfd poll_fd[2];

	printf("install bt alsa input device.\n");
	ioctl(p->bt_alsa_dev, 0x100, 1);
	while (p->is_event_thread_exit == 0){
		memset(poll_fd, 0 , sizeof(struct pollfd)*2);
		poll_fd[0].fd = p->hidraw_fd;
		poll_fd[0].events = POLLIN;
		poll_fd[1].fd = p->bt_alsa_dev;
		poll_fd[1].events = POLLIN;
		int rc = TEMP_FAILURE_RETRY(poll(poll_fd, 2, -1));
		if (rc < 0){
			printf("poll() failed, error %s", strerror(errno));
			break;
		}
		if (rc == 0){
			printf("poll() timed out. shouldn't happen\n");
			break;
		}

		for (i = 0; i < 2; i++){
			rc = check_poll_result(&poll_fd[i], i, p);
			if (rc < 0)
				goto exit;
		}
	}
exit:
	close(poll_fd[0].fd);
	if (p->timerid != 0){
		timer_delete(p->timerid); p->timerid = 0;
	}

	return NULL;
}

static int open_dev_file(char * dev_file_name)
{
	int retry_times = 0;
	int dev_fd = -1;
retry:
	dev_fd = open(dev_file_name, O_RDWR);
	if (dev_fd < 0) {
		int err = errno;
		if (err == ENOENT && retry_times < 10){
			printf("device file not created yet, try again later.\n");
			usleep(50*1000);
			retry_times++;
			goto retry;
		}
		printf("Unable to open device %s, %s",dev_file_name, strerror(err));
		return -1;
	}
	return dev_fd;
}


static void create_hidraw_thread(char *hidraw_file_name)
{
	int hidraw_num = atoi(&hidraw_file_name[11]);
	printf("%s, hidraw_num %d\n", __func__, hidraw_num);
	if (g_is_desired_rc_connected){
		printf("another remote control already connected, do not active microphone for this one.\n");
		return;
	}

	hidraw_thread_t p = &g_hidraw_thread;
	int retry_times = 0;
	while (p->hidraw_work != 0){
		printf("unfinished clean work for previous connect, wait.\n");
		sleep(1);
		if (retry_times++ > 10){
			exit(EXIT_FAILURE);//let system reboot this service.
		}
	}

	p->bt_alsa_dev = open_dev_file("/dev/snd_bt");
	if (p->bt_alsa_dev < 0) {
		goto err;
	}

	usleep(50*1000);//wait 50ms here.
	p->hidraw_fd = open_dev_file(hidraw_file_name);
	if (p->hidraw_fd < 0) {
		goto err;
	}

	if (setup_voice_capture_param(p) < 0){
		goto err;
	}

	p->is_event_thread_exit = 0;
	int ret = pthread_create(&p->hidraw_work, NULL, read_hidraw_func, (void*)p);
	if (ret != 0){
		printf("%s,%d\n",__FUNCTION__, __LINE__);
		goto err;
	}
	p->hidraw_num = hidraw_num;
	g_is_desired_rc_connected = 1;
	return;
err:
	if(p->hidraw_fd >=0)
		close(p->hidraw_fd);
	if(p->bt_alsa_dev >=0)
		close(p->bt_alsa_dev);
	memset(p, 0, sizeof(hidraw_thread));
	return;
}

static void feed_dummy_data(hidraw_thread_t p)
{
	unsigned char dummy_data[256];
	memset(dummy_data, 0x40, 256);

	struct timeval start;
	struct timeval now;
	gettimeofday(&start,NULL);

	do {
		if (write_to_dev_file(p->bt_alsa_dev, dummy_data, 256) < 0)
			return;
		usleep(8000);
		gettimeofday(&now,NULL);
	}while(now.tv_sec - start.tv_sec < 6 && p->is_alsa_started);
}


static void destroy_hid_thread(int hidraw_num)
{

	printf("%s, hidraw_num %d\n", __func__, hidraw_num);
	hidraw_thread_t p = &g_hidraw_thread;
	if (p->hidraw_num != hidraw_num){
		printf("hidraw_num %d not initiated.\n",hidraw_num);
		return;
	}

	if (p->is_alsa_started){
		printf("bt device disconnected before user close the ALSA input device.\n");
		feed_dummy_data(p);
	}

	if (p->timerid != 0){
		timer_delete(p->timerid); p->timerid = 0;
	}

	if(p->is_event_thread_exit == 0){
		p->is_event_thread_exit = 1;
		pthread_join(p->hidraw_work, NULL);
	}
	if (p->hidraw_fd >= 0){
		close(p->hidraw_fd);
	}

	if (p->bt_alsa_dev >= 0){
		printf("remove bt alsa input device.\n");
		ioctl(p->bt_alsa_dev, 0x110, 1);
		close(p->bt_alsa_dev);
	}

	memset(p, 0, sizeof(hidraw_thread));
	g_is_desired_rc_connected = 0;
}

static void process_add_event(char *hidraw_file_name)
{
	if (strlen(hidraw_file_name) > 12){
		printf("exceed maximum hidraw device number.%s\n", __func__);
		return;
	}
	create_hidraw_thread(hidraw_file_name);
}

static void process_remove_event(char *hidraw_file_name)
{
	if (strlen(hidraw_file_name) > 12){
		printf("exceed maximum hidraw device number.%s\n", __func__);
		return;
	}
	int hidraw_num = atoi(hidraw_file_name+11);
	destroy_hid_thread(hidraw_num);
}

static int read_inotify(int inotify_fd) {
    int rc;
    char devname[128];
    char *filename;
    char event_buf[512];
    int event_size;
    int event_pos = 0;
    struct inotify_event *event;

    printf("read_inotify fd: %d\n", inotify_fd);
    rc = TEMP_FAILURE_RETRY(read(inotify_fd, event_buf, sizeof(event_buf)));
    if(rc < (int)sizeof(*event)) {
        printf("could not get event, %s\n", strerror(errno));
        return -1;
    }

    strcpy(devname, "/dev");
    filename = devname + strlen(devname);
    *filename++ = '/';

    while(rc >= (int)sizeof(*event)) {
        event = (struct inotify_event *)(event_buf + event_pos);
        printf("%d: %08x \"%s\"\n", event->wd, event->mask, event->len ? event->name : "");
        if(event->len && !strncmp(event->name, "hidraw", strlen("hidraw"))) {
            strcpy(filename, event->name);
            if(event->mask & IN_CREATE) {
                process_add_event(devname);
            } else {
                printf("Removing device '%s' due to inotify event\n", devname);
                process_remove_event(devname);
            }
        }
        event_size = sizeof(*event) + event->len;
        rc -= event_size;
        event_pos += event_size;
    }
    return 0;
}

static void process_inode_event(struct epoll_event *events,
		int event_counts, int inotify_fd)
{
	int i;
	for (i = 0; i < event_counts; i++){
		if (events->data.u32 == EPOLL_ID_INOTIFY){
			if (events->events & EPOLLIN) {
				read_inotify(inotify_fd);
			} else {
				printf("Received unexpected epoll event 0x%08x for inotify.", events->events);
			}
		}
		events++;
	}
}

int main(int argc, char **argv)
{
	if (argc == 4){
		g_vendor_id = atoi(argv[1]);
		g_product_id = atoi(argv[2]);
		g_voice_report_id = atoi(argv[3]);
		printf("vendor_id %04x, product_id %04x, voice report id %02x\n",
				g_vendor_id, g_product_id, g_voice_report_id);
	}
	else
		printf("use default config for MRVL remote control");

	int epoll_fd = init_epoll_fd();
	if (epoll_fd < 0)
		return 1;

	int inotify_fd = init_inotify("/dev", epoll_fd);
	if (inotify_fd < 0){
		close(epoll_fd);
		return -1;
	}

	int rc = add_inotify_to_epoll(epoll_fd, inotify_fd);
	if (rc < 0){
		close(epoll_fd);close(inotify_fd);
		return 1;
	}

	static const int EPOLL_MAX_EVENTS = 16;
	struct epoll_event epoll_events[EPOLL_MAX_EVENTS];
	while(1){
		int rc = TEMP_FAILURE_RETRY(epoll_wait(epoll_fd, epoll_events, EPOLL_MAX_EVENTS, -1));
		if (rc <= 0) {
			printf("poll failed errno=%s\n", strerror(errno));
			return 1;
		} else {
			process_inode_event(epoll_events, rc, inotify_fd);
		}
	}

	close(inotify_fd);
	close(epoll_fd);

	return 0;
}
