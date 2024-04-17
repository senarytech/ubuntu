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
#include "ree_sys_callback_ipc_server.h"
#include "rpmb.h"
#include "ree_ca_rpmb_op.h"
#include "ree_ca_file_op.h"

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

static TEEC_Result server_process_ipc_request(struct ipc_generic_param *ipc_param, int fd, char* root_dir)
{
	int ret;

	dbg_printf("recv tee request id is %d, len is %x\n", ipc_param->cmd_id, ipc_param->param_len);
	switch (ipc_param->cmd_id) {
		case REE_CMD_FILE_OPEN:
			ipc_param->cmd_id = Do_FileOpen(ipc_param->param, root_dir);
			break;
		case REE_CMD_FILE_CLOSE:
			ipc_param->cmd_id = Do_FileClose(ipc_param->param);
			ipc_param->param_len = 0;
			break;
		case REE_CMD_FILE_READ:
			ipc_param->cmd_id = Do_FileRead(ipc_param->param);
			break;
		case REE_CMD_FILE_WRITE:
			ipc_param->cmd_id = Do_FileWrite(ipc_param->param,
					&ipc_param->param_len);
			break;
		case REE_CMD_FILE_SEEK:
			ipc_param->cmd_id = Do_FileSeek(ipc_param->param);
			break;
		case REE_CMD_FILE_DELETE:
			ipc_param->cmd_id = Do_FileDelete(ipc_param->param, root_dir);
			ipc_param->param_len = 0;
			break;
		case REE_CMD_FOLDER_MAKE:
			ipc_param->cmd_id = Do_FolderMake(ipc_param->param, root_dir);
			ipc_param->param_len = 0;
			break;
		case REE_CMD_FOLDER_OPEN:
			ipc_param->cmd_id = Do_FolderOpen(ipc_param->param, root_dir);
			break;
		case REE_CMD_FOLDER_CLOSE:
			ipc_param->cmd_id = Do_FolderClose(ipc_param->param);
			ipc_param->param_len = 0;
			break;
		case REE_CMD_FOLDER_READ:
			ipc_param->cmd_id = Do_FolderRead(ipc_param->param, root_dir);
			break;
		case REE_CMD_RPMB_READ_COUNTER:
		case REE_CMD_RPMB_READ_BLOCK:
			ipc_param->cmd_id = Do_RPMBReadBlock(ipc_param->param);
			break;
		case REE_CMD_RPMB_READ_SIZE:
			ipc_param->cmd_id = Do_RPMBReadSize(ipc_param->param);
			break;
		case REE_CMD_RPMB_WRITE_KEY:
		case REE_CMD_RPMB_WRITE_BLOCK:
			ipc_param->cmd_id = Do_RPMBWriteBlock(ipc_param->param);
			break;

		default:
			break;
	}

	/* response to client */
	ret = write(fd, ipc_param, sizeof(struct ipc_generic_param) + ipc_param->param_len);
	if (ret < 0)
		return TEEC_ERROR_COMMUNICATION;

	return TEEC_SUCCESS;
}

int REE_CreateIpcServer(const char* tee_data_dir)
{
	socklen_t srv_addr_len;
	int listen_fd;
	int com_fd;
	int ret = 0;
	int data_len, len;
	struct sockaddr_un clt_addr;
	struct sockaddr_un srv_addr;
	unsigned char recv_header[sizeof(struct ipc_generic_param)];
	unsigned char *recv_buf;
	struct ipc_generic_param *ipc_param;
	const char const_tee_dir[] = "/data/tee";
	char *tee_dir;

	if (tee_data_dir == NULL)
		tee_dir = (char*)const_tee_dir;
	else
		tee_dir = (char*)tee_data_dir;

	while(1) {
		listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if(listen_fd<0) {
			perror("cannot create listening socket");
			sleep(5);
			continue;
		}
		while(1) {
			srv_addr.sun_family = AF_UNIX;
			strncpy(srv_addr.sun_path, TEE_SOCKS_ADDR, sizeof(srv_addr.sun_path)-1);
			srv_addr.sun_path[0] = '\0';
			srv_addr_len = strlen(TEE_SOCKS_ADDR) + offsetof(struct sockaddr_un, sun_path);
			dbg_printf("srv name is %s, len is %d\n", &srv_addr.sun_path[1], srv_addr_len);
			ret = bind(listen_fd, (struct sockaddr*)&srv_addr, srv_addr_len);
			if(ret == -1) {
				perror("cannot bind server socket");
				close(listen_fd);
				break;
			}
			ret = listen(listen_fd, 1);
			if(ret == -1) {
				perror("cannot listen the client connect request");
				close(listen_fd);
				break;
			}
			while(1) {
				len = sizeof(clt_addr);
				com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, (socklen_t*)&len);
				if(com_fd<0) {
					perror("cannot accept client connect request");
					close(listen_fd);
					break;
				}
				/* here comes a connect... read the header first */
				memset(recv_header,  0, sizeof(recv_header));
				data_len = sizeof(recv_header);
				ret = read_data_from_socket(com_fd, recv_header, sizeof(recv_header));
				if (ret < 0) {
					perror("read socket fail\n");
					close(com_fd);
					continue;
				}

				/* read all data */
				ipc_param = (struct ipc_generic_param*)recv_header;
				dbg_printf("srv read header %d bytes, cmd id %d, len %d\n", ret, ipc_param->cmd_id, ipc_param->param_len);
				recv_buf = malloc(sizeof(struct ipc_generic_param) + ipc_param->param_len);
				if (!recv_buf) {
					perror("out of memory\n");
					close(com_fd);
					continue;
				}
				memcpy(recv_buf, recv_header, sizeof(struct ipc_generic_param));
				data_len =  ipc_param->param_len;
				ret = read_data_from_socket(com_fd,
						recv_buf + sizeof(struct ipc_generic_param),
						data_len);
				if (ret < 0) {
					perror("read socket fail\n");
					free(recv_buf);
					recv_buf = NULL;
					close(com_fd);
					continue;
				}
				dbg_printf("srv read data %d bytes\n", ret);

				/* now we get all data and can process the request */
				ipc_param = (struct ipc_generic_param*)recv_buf;
				server_process_ipc_request(ipc_param, com_fd, tee_dir);

				/* close the connection and wait for next request */
				if (recv_buf) {
					free(recv_buf);
					recv_buf = NULL;
				}
				close(com_fd);

			}

		}

	}

	return 0;
}
