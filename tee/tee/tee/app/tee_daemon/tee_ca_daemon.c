#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
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
#include "ree_sys_callback_ipc_server.h"

int main(int argc, const char *argv[])
{
	REE_CreateIpcServer(argv[1]);
	return 0;
}

