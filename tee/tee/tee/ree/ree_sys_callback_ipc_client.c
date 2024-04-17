#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tee_comm.h"
#include "ree_sys_callback_cmd.h"
#include "ree_sys_callback_ipc_client.h"
#include "tee_client_type.h"
#include "tee_client_const.h"

#define SOCK_NAME_LEN		32
#define SOCK_NAME		"@tee_clt"

#define gettid() 	syscall(__NR_gettid)

#ifdef DEBUG
#define dbg_printf(fmt, args...)      	printf(fmt, ##args)
#else
#define dbg_printf(fmt, ...)          	do { } while (0)
#endif

static int read_data_from_socket(int fd, void* out_buf, int cnt)
{
	int data_len;
	int read_bytes;
	char* buff = out_buf;

	data_len = cnt;
	while(1) {
		read_bytes = read(fd, buff, data_len);
		if (read_bytes < 0) {
			perror("read socket fail\n");
			return -1;
		}
		if (read_bytes == 0)
			break;
		data_len -= read_bytes;
		buff += read_bytes;
		if (data_len > 0)
			continue;
		else
			break;
	}

	return cnt - data_len;
}

int REE_ClientProcessIpcRequest(int ipc_cmd, void *in_param, int in_len,
		void *out_param, int out_len,
		int (*client_request_callback)(int, void*, int, void*, int))
{
	socklen_t clt_addr_len;
	char sock_name[SOCK_NAME_LEN];
	int ret = TEEC_ERROR_GENERIC;
	int proc_bytes;
	int data_len, len;
	struct sockaddr_un clt_addr;
	struct sockaddr_un srv_addr;
	unsigned char recv_header[8];
	unsigned char *ipc_buf, *tbuf;
	struct ipc_generic_param *ipc_param;
	int sock_fd;

	/* create named socket */
	sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock_fd<0) {
		perror("cannot create listening socket");
		return TEEC_ERROR_ACCESS_CONFLICT;
	}
	memset(&clt_addr, 0, sizeof(clt_addr));
	memset(sock_name, 0, sizeof(sock_name));
	clt_addr.sun_family = AF_UNIX;
	snprintf(sock_name, SOCK_NAME_LEN - 1, "%s%05d", SOCK_NAME, (int)gettid());
	dbg_printf("client %d sock name is %s\n", getpid(), sock_name);
	strncpy(clt_addr.sun_path, sock_name, sizeof(clt_addr.sun_path) - 1);
	clt_addr.sun_path[0] = '\0';
	clt_addr_len = strlen(sock_name) + offsetof(struct sockaddr_un, sun_path);
	ret = bind(sock_fd, (struct sockaddr*)&clt_addr, clt_addr_len);
	if(ret == -1) {
		perror("cannot bind socket");
		close(sock_fd);
		return TEEC_ERROR_ACCESS_CONFLICT;
	}

	/* connect to tee_daemon */
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	strncpy(srv_addr.sun_path, TEE_SOCKS_ADDR, sizeof(srv_addr.sun_path) - 1);
	srv_addr.sun_path[0] = '\0';
	len = strlen(TEE_SOCKS_ADDR) + offsetof(struct sockaddr_un, sun_path);
	dbg_printf("connect to srv name is %s, len is %d\n", &srv_addr.sun_path[1], len);
	ret=connect(sock_fd, (struct sockaddr*)&srv_addr, len);
	if(ret == -1) {
		close(sock_fd);
		perror("connect fail\n");
		return TEEC_ERROR_COMMUNICATION;
	}

	ipc_buf = malloc(sizeof(struct ipc_generic_param) + in_len);
	if (!ipc_buf) {
		close(sock_fd);
		perror("out of memory\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}

	/* send request data */
	ipc_param = (struct ipc_generic_param *)ipc_buf;
	ipc_param->cmd_id = ipc_cmd;
	ipc_param->param_len = in_len;
	memcpy(ipc_param->param, in_param, in_len);

	proc_bytes = write(sock_fd, ipc_param, sizeof(struct ipc_generic_param) + in_len);
	if (proc_bytes < 0) {
		perror("write socket fail\n");
		close(sock_fd);
		free(ipc_buf);
		return TEEC_ERROR_COMMUNICATION;
	}

	dbg_printf("client send %d bytes, cmd is %d, len is %d\n", proc_bytes, ipc_param->cmd_id, ipc_param->param_len);
	if (ipc_buf) {
		free(ipc_buf);
		ipc_buf = NULL;
	}
	/* read the header of response */
	memset(recv_header,  0, sizeof(recv_header));
	ret = read_data_from_socket(sock_fd, recv_header, sizeof(recv_header));
	if (ret < 0) {
		perror("read socket fail\n");
		close(sock_fd);
		return TEEC_ERROR_COMMUNICATION;
	}
	dbg_printf("client read header %d bytes\n", ret);

	ipc_param = (struct ipc_generic_param*)recv_header;
	if (ipc_param->param_len > 0) {
		/* read rest data */
		ipc_buf = malloc(ipc_param->param_len + sizeof(struct ipc_generic_param));
		if (!ipc_buf) {
			perror("out of memory\n");
			close(sock_fd);
			return TEEC_ERROR_OUT_OF_MEMORY;
		}

		memcpy(ipc_buf, recv_header, sizeof(struct ipc_generic_param));
		data_len = ipc_param->param_len;
		tbuf = ipc_buf + sizeof(struct ipc_generic_param);
		ret = read_data_from_socket(sock_fd, tbuf, data_len);
		if (ret < 0) {
			perror("read socket fail\n");
			close(sock_fd);
			free(ipc_buf);
			return TEEC_ERROR_COMMUNICATION;
		}

		dbg_printf("client read data %d bytes\n", ret);
		/* get all data */
		ipc_param = (struct ipc_generic_param*)ipc_buf;
	}
	/* now go to callback  */
	ret = (*client_request_callback)(ipc_param->cmd_id, ipc_param->param,
			ipc_param->param_len, out_param, out_len);
	if (ipc_buf)
		free(ipc_buf);
	close(sock_fd);
	return ret;
}
