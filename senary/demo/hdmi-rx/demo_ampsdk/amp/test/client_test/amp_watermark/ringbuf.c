#include "ringbuf.h"
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG
#ifdef DEBUG
#define INFO(...)  printf(__VA_ARGS__);
#else
#define INFO(...)  {};
#endif

void* memcpy(void *dest, const void *src, size_t bytes);

static inline int fls(int x)
{
	int position=0;
	int i;
	if(0 != x)
	{
		for (i = (x >> 1); i != 0; ++position)
           i >>= 1;
	}
	else
	{
        position = -1;
	}

	INFO("the position is %d\n",position +1 );
    return position+1;
}
static _Bool is_power_of_2(unsigned int x)
{
    return (x != 0) && ((x & (x - 1)) == 0);
}

static unsigned int rounddown_pow_of_two(unsigned int n) {
	n|=n>>1; n|=n>>2; n|=n>>4; n|=n>>8; n|=n>>16;
	return (n+1) >> 1;
}

static unsigned int roundup_pow_of_two(unsigned int n) {
	return  1UL << fls(n - 1);
}

//beter use this function
int ringbuf_alloc(struct ringbuf_t *self, unsigned int item_size, unsigned int bufsize) {

	bufsize = roundup_pow_of_two(bufsize); //need alloc more bytes then bufsize
    self->esize = item_size;

    self->in = 0;
	self->out = 0;

	if(bufsize < 2 )
	{
		self->data = NULL;
		printf("alloc size invalid\n");
		return -1;
	}
	self->data = malloc(bufsize*item_size); //item_size set to 1
	if(!self->data)
	{
		self->mask = 0;
		printf("alloc buf fail\n");
		return -1;
	}
	self->mask = bufsize -1 ;

	printf("alloc ringbuf sucess size %ld mask is % %lx\n",bufsize,self->mask);

	return 0;
}

//buf is pre-allocated
int ringbuf_init(struct ringbuf_t *self, unsigned int item_size, void *buf, unsigned int bufsize) {

    bufsize /= item_size;
	self->data = buf;
    self->esize = item_size;

	if (!is_power_of_2(bufsize))
		bufsize = rounddown_pow_of_two(bufsize);

    self->mask = bufsize - 1;
    self->in = self->out = 0;

	printf("the mask is %ld %lx\n",self->mask,self->mask);

	return 0;
}

// the used ringbuf size
unsigned int ringbuf_len(struct ringbuf_t *self) {
    return self->in - self->out;
};

// max bufsize of the ringbuf
unsigned int ringbuf_cap(struct ringbuf_t *self) {
    return self->mask + 1;
};

// free(not used) buf size
unsigned int ringbuf_avail(struct ringbuf_t *self) {
    return ringbuf_cap(self) - ringbuf_len(self);
};

int ringbuf_is_full(struct ringbuf_t *self) {
    return ringbuf_len(self) > self->mask;
};

int ringbuf_is_empty(struct ringbuf_t *self) {
    return self->in == self->out;
};

#define min(a, b) ((a)<(b)?(a):(b))

//write data to ringbuf
static void ringbuf_copy_in(struct ringbuf_t *self, const void *src, unsigned int len, unsigned int off) {

	unsigned int size = self->mask + 1;
	unsigned int esize = self->esize;
	unsigned int l;

	off &= self->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(self->data + off, src, l);
	memcpy(self->data, (const unsigned char*)src + l, len - l);
}

//len is data size for buf data
//len: return write data size
unsigned int ringbuf_in(struct ringbuf_t *self, const void *buf, unsigned int len) {

    unsigned int avail = ringbuf_avail(self);
    if (len > avail)
	{
		//len = avail;
		return 0;
	}


    ringbuf_copy_in(self, buf, len, self->in);

    self->in += len; //update ringbuf_t->in
    return len;
}


 static void ringbuf_copy_out(struct ringbuf_t *self, void *dst, unsigned int len, unsigned int off) {
	unsigned int size = self->mask + 1;
	unsigned int esize = self->esize;
	unsigned int l;

	off &= self->mask;
	if (esize != 1) {
		off *= esize;
		size *= esize;
		len *= esize;
	}
	l = min(len, size - off);

	memcpy(dst, self->data + off, l);
	memcpy((unsigned char*)dst + l, self->data, len - l);
}

//only peek data,but not update ringbuf out pointer
unsigned int ringbuf_out_peek(struct ringbuf_t *self, void *buf, unsigned int len) {
	unsigned int l;
	l = self->in - self->out;
	if (len > l)
	{
		len = l;
		return 0;
	}

	ringbuf_copy_out(self, buf, len, self->out);
	return len;
}

//
unsigned int ringbuf_out(struct ringbuf_t *self, void *buf, unsigned int item_count) {

    item_count = ringbuf_out_peek(self, buf, item_count);
    self->out += item_count;
    return item_count;
}

char * ringbuf_get_out_pointer(struct ringbuf_t *self)
{
	return self->data + self->out;
}

unsigned int ringbuf_update_out_pointer(struct ringbuf_t *self,unsigned int len)
{
	self->out += len;
	return 0;
}

char * ringbuf_get_in_pointer(struct ringbuf_t *self)
{
	return self->data + self->in;
}

unsigned int ringbuf_update_in_pointer(struct ringbuf_t *self,unsigned int len)
{
	self->in += len;
	return 0;
}