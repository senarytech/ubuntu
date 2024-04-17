#ifndef RINGBUF_H
#define RINGBUF_H

struct ringbuf_t {
    unsigned char *data;  /* the buffer holding the data */
    unsigned int in;      /* data is added at offset (in % size) */
    unsigned int out;     /* data is extracted from off. (out % size) */
    unsigned int mask;
    unsigned int esize;    /* size of one item */
};

#ifdef __cplusplus
extern "C" {
#endif

int ringbuf_alloc(struct ringbuf_t *self, unsigned int item_size, unsigned int bufsize);
int ringbuf_init(struct ringbuf_t *self, unsigned int item_size, void *buf, unsigned int bufsize);


// item count in buf
unsigned int ringbuf_len(struct ringbuf_t *self);

// max item count in buf
unsigned int ringbuf_cap(struct ringbuf_t *self);

// avail item count
unsigned int ringbuf_avail(struct ringbuf_t *self);

int ringbuf_is_full(struct ringbuf_t *self);
int ringbuf_is_empty(struct ringbuf_t *self);

unsigned int ringbuf_in(struct ringbuf_t *self, const void *buf, unsigned int item_count);
unsigned int ringbuf_out(struct ringbuf_t *self, void *buf, unsigned int item_count);
unsigned int ringbuf_out_peek(struct ringbuf_t *self, void *buf, unsigned int len);

char * ringbuf_get_out_pointer(struct ringbuf_t *self);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RINGBUF_H