#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ringbuf_t{
    unsigned char *pbuf[32];
	int data_len;
	int pos_read;
	int pos_write;
}ringbuf_s;
ringbuf_s g_ringbuf;

int main(int argc,char **argv)
{
	int i;
	printf("hello\n");
	
	g_ringbuf.pos_read = 0;

for(i = 0; i < 32; i++)
{
   g_ringbuf.pbuf[i]  = NULL;
   g_ringbuf.pbuf[i] = (unsigned char*)malloc(128);
   if(g_ringbuf.pbuf[i] == NULL)
     printf("malloc error\n");
}
printf("malloc ok\n");
unsigned char data[128];
int j;
unsigned char *pdata;
for(j = 0; j < 32; j ++)
{
	for(i = 0; i < 128; i ++)
	{
   		data[i] = i+j;
	}
	pdata = g_ringbuf.pbuf[j];
	memcpy(pdata,&data[0],128);
}
for(j = 0; j < 32; j ++)
{
	pdata = g_ringbuf.pbuf[6]+j;
	printf("%d ",*pdata);
}


return 0;
}
