/*
 * Copyright (c) 1996, 1997, 1998, 1999 The University of Utah and
 * the Computer Systems Laboratory at the University of Utah (CSL).
 * All rights reserved.
 *
 * Permission to use, copy, modify and distribute this software is hereby
 * granted provided that (1) source code retains these copyright, permission,
 * and disclaimer notices, and (2) redistributions including binaries
 * reproduce the notices in supporting documentation.
 *
 * THE UNIVERSITY OF UTAH AND CSL ALLOW FREE USE OF THIS SOFTWARE IN ITS "AS
 * IS" CONDITION.  THE UNIVERSITY OF UTAH AND CSL DISCLAIM ANY LIABILITY OF
 * ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * CSL requests users of this software to return to csl-dist@cs.utah.edu any
 * improvements that they make and grant CSL redistribution rights.
 */

#ifndef __flick_link_iiop_h
#define __flick_link_iiop_h

#include <sys/types.h>
#include <flick/link/all.h>

#include "OSAL_api.h"

extern int flick_seqnum;

typedef struct {
    MV_CC_HANDLE_CBufCtrl_t ccbuf;
    MV_CC_HANDLE_MsgQ_t msgq;
    UINT32 msgqId;
    MV_OSAL_HANDLE_MUTEX_t lock;
} flick_mvcc_channel_t;

typedef struct FLICK_BUFFER {
    void *buf_start;    /* start of current message */
    void *buf_current;   /* position in current message */
    void *buf_end;       /* end of current message */

    /* These not used for encoding and sending !!! */
    void *real_buf_start; /* start of allocated block w/multiple messages*/
    void *buf_read;       /* unfilled part of allocated block */
    void *real_buf_end;   /* end of allocated block */
    unsigned int fds[FLICK_MAX_FD]; /* fds associated with this flick buffer */
    unsigned int fd_used;           /* Number of fds that have already been used so far while parsing the
                                     * flick_buffer. This starts at 0 and is incremented each time a
                                     * native file_descriptors is parsed. It is equal to fd_count when the
                                     * parsing is successfully completed.
                                     */
    unsigned int fd_count;          /* count of fds associated with this flick buffer */
} *flick_marshal_stream_t, FLICK_BUFFER;

typedef struct flick_request_state {
    int request_nr;
    int seqnum;
    int boa_locked;
} flick_request_state_t;

#define _buf_start (_stream->buf_start)
#define _buf_current (_stream->buf_current)
#define _buf_end (_stream->buf_end)

typedef struct flick_msg_struct_t * flick_msg_t;

#define flick_assert(arg) assert(arg)

#define flick_check_null_pointer(_param, _onerror)                      \
    do {                                                                \
        if ((void *)(_param) == NULL) {                                 \
            fprintf(stderr, "Pointer %s may not be NULL in call to %s\n", #_param, __FUNCTION__); \
            flick_stub_error(FLICK_ERROR_NULL_POINTER, _onerror);       \
        }                                                               \
    } while (0)

#define flick_iiop_check_span(_size, _onerror) {            \
    if ((((char *) _buf_current) + (_size)) > ((char *) _buf_end))  \
        flick_stub_error(FLICK_ERROR_COMMUNICATION, _onerror);  \
}

/* stream <==> msg conversion macros */
#define MSG_HDR_PAD 64

#define flick_iiop_encode_stream_to_msg(_msg) {                  \
    (_msg) = (flick_msg_t) calloc(sizeof(struct flick_msg_struct_t), 1); \
    (_msg)->buf = _buf_start;                        \
    (_msg)->hdr = (_msg)->msg = ((char *) (_msg)->buf) + MSG_HDR_PAD;    \
    (_msg)->msg_len                              \
        = (unsigned int)                         \
          ((char *) _buf_current - (char *) (_msg)->msg);        \
}

#define flick_iiop_encode_msg_to_stream(_msg) {         \
    _buf_start = (_msg)->hdr;               \
    _buf_end = ((char *) _buf_start) + (_msg)->msg_len; \
    _buf_current = (_msg)->msg;             \
}

#define flick_iiop_decode_stream_to_msg(_msg) {                  \
    (_msg) = (flick_msg_t) calloc(sizeof(struct flick_msg_struct_t), 1); \
    (_msg)->buf = (_msg)->hdr = _buf_start;                  \
    (_msg)->msg = _buf_current;                      \
    (_msg)->msg_len                              \
        = (unsigned int)                         \
          ((char *) _buf_end - (char *) _buf_start);             \
    (_msg)->response = ((char *) _buf_start)[20];                \
}

#define flick_iiop_decode_msg_to_stream(_msg) {         \
    _buf_start = (_msg)->hdr;               \
    _buf_end = ((char *) _buf_start) + (_msg)->msg_len; \
    _buf_current = (_msg)->msg;             \
}

/* Used for both client request & server reply... */
#define flick_iiop_build_message_header(loc, msg_type) {    \
    ((char *) (loc))[0] = 'G';              \
    ((char *) (loc))[1] = 'I';              \
    ((char *) (loc))[2] = 'O';              \
    ((char *) (loc))[3] = 'P';              \
    ((char *) (loc))[4] = 1;                \
    ((char *) (loc))[5] = 0;                \
    /* little endian flag... */             \
    ((short *) (loc))[3] = 1;               \
    ((char *) (loc))[7] = (msg_type);           \
    /* ((int *) (loc))[2] = size of rest of message */  \
}

#ifdef VERIFY
/* Used for both client reply & server request... */
#define flick_iiop_verify_message_header(loc, msg_type) {   \
    if (((char *) (loc))[0] != 'G' ||           \
        ((char *) (loc))[1] != 'I' ||           \
        ((char *) (loc))[2] != 'O' ||           \
        ((char *) (loc))[3] != 'P' ||           \
        ((char *) (loc))[4] != 1 ||             \
        ((char *) (loc))[5] != 0 ||             \
        ((char *) (loc))[7] != (msg_type))          \
        fprintf(stderr, "Malformed Message Header!\n"); \
}
#else
#define flick_iiop_verify_message_header(loc, msg_type) {}
#endif

#define flick_iiop_discard_service_context_list() {               \
    char *_buf_cursor = _buf_current;                     \
                                          \
    unsigned int _service_context_seq_len;                    \
    unsigned int _service_context_len;                    \
    unsigned int _i;                              \
                                          \
    _service_context_seq_len = *(unsigned int *) _buf_cursor;         \
    _buf_cursor += 4;                             \
                                          \
    if (_service_context_seq_len == 0) {                      \
        /* Zero is zero even when it's byteswapped. */            \
    } else {                                  \
        int _swap = flick_cdr_swap();                     \
                                          \
        if (_swap)                            \
            _service_context_seq_len                  \
                = swap_unsigned32(_service_context_seq_len);  \
                                          \
        for (_i = 0; _i < _service_context_seq_len; ++_i) {       \
            /* Skip the `context_id', an unsigned int. */         \
            _buf_cursor += 4;                     \
            _service_context_len                      \
                = *(unsigned int *) _buf_cursor;          \
            if (_swap)                        \
                _service_context_len                  \
                    = swap_unsigned32(            \
                        _service_context_len);        \
            _buf_cursor += 4;                     \
            /*                            \
             * Note: It is always correct to 4-byte align here,   \
             * because every service context is followed by an    \
             * unsigned int.  Even the last is followed by an    \
             * unsigned int in the GIOP request or reply header. \
             */                           \
            _buf_cursor += (_service_context_len + 3) & ~3;       \
        }                                 \
    }                                     \
                                          \
    _buf_current = _buf_cursor;                       \
}

#define flick_iiop_discard_request_id() {       \
    _buf_current = ((char *) _buf_current) + 4; \
}

/* Initialization / Exit code */
#define flick_iiop_client_start_encode() {                              \
        _stream = _obj->boa->in;                                        \
        flick_iiop_client_start_encode_internal(_obj, _stream, &_request_state); \
    }

#define flick_iiop_client_end_encode() /* Nothing to do. */

#define flick_iiop_client_set_response_expected(val) {          \
    /*                              \
     * 20 ==  12 `GIOP::MessageHeader' bytes            \
     *       + 4 `GIOP::RequestHeader.service_context' bytes    \
     *       + 4 `GIOP::RequestHeader.request_id' bytes     \
     *                              \
     * XXX --- This assumes that the service context list is a  \
     * zero-element sequence.                   \
     */                             \
    ((char *) _buf_start)[20] = (val);              \
}

#define flick_iiop_client_start_decode() {              \
        _stream = _obj->boa->out;                        \
        flick_iiop_client_start_decode_internal(_obj, _stream);  \
    }

#define flick_iiop_client_end_decode() {}

#define flick_iiop_server_start_decode() {               \
    _stream = InHeadP;                       \
    /* Verify the request GIOP MessageHeader (note Request == 0). */ \
    flick_iiop_verify_message_header(_buf_start, 0);         \
    /*                               \
     * NOTE: The server dispatch loop sets the stream pointer to     \
     * point to the start of the operation name --- i.e., we've  \
     * already parsed up to the `operation' field of the GIOP    \
     * RequestHeader.  See the function `find_implementation'.   \
     */                              \
}

#define flick_iiop_server_end_decode() /* Nothing to do. */

#define flick_iiop_server_start_encode() {              \
    _stream = OutHeadP;                     \
    flick_iiop_server_start_encode_internal(_stream, _request_id);  \
}

#define flick_iiop_server_restart_encode()      \
    _buf_current = &(((int *) _buf_start)[5])

#define flick_iiop_server_end_encode() {}

/* Message marshal stub initialization/exit macros */
#define flick_iiop_msg_start_encode(msg_type) {             \
    /*                              \
     * Normally, buffers are allocated when the BOA was created.    \
     * A msg marshal stub allows the message to persist, and    \
     * thus needs its own buffer allocated.             \
     */                             \
    _buf_start = malloc(4096 /* XXX */);                \
    _buf_current = ((char *) _buf_start) + MSG_HDR_PAD;     \
    _buf_end = ((char *) _buf_start) + 4096;            \
    /*                              \
     * From here, we need to put the object key, operation string,  \
     * plus the 'Principal' opaque interface.           \
     */                             \
}

#define flick_iiop_msg_end_encode() /* Do nothing */

#define flick_iiop_msg_start_decode(msg_type) {             \
    /* Verify the reply GIOP MessageHeader (note Reply == 1). */    \
    flick_iiop_verify_message_header(_buf_start, msg_type);     \
}

#define flick_iiop_msg_end_decode() /* Do nothing */

/* Send stub macros */
#define flick_iiop_send_start_encode() {                \
    /* Reset the hdr pointer since we are encoding a new one. */    \
    (_msg)->hdr = (_msg)->msg;                  \
}

#define flick_iiop_send_end_encode(reqrep) {                \
    (_msg)->hdr = ((char *) (_msg)->hdr) - 24;          \
    flick_iiop_build_message_header((_msg)->hdr, reqrep);       \
    ((unsigned int *) (_msg)->hdr)[3] = 0; /* service context */    \
    ((unsigned int *) (_msg)->hdr)[4] = inv_id; /* request id */    \
    ((unsigned int *) (_msg)->hdr)[5]               \
        = (_msg)->response; /* response expected */     \
}


/* Globbing & Chunking code */
/*
 * XXX --- The IIOP runtime function `flick_cdr_encode_IOR_internal' knows how
 * globbing is implemented.  If you change this function you must also change
 * the globbing in that runtime function.
 */
#define flick_iiop_encode_new_glob(max_size, _onerror) {         \
    if (((char *) _buf_end) - ((char *) _buf_current)        \
        < (long) (max_size)) {                   \
            flick_stub_error(FLICK_ERROR_NO_MEMORY,      \
                     _onerror);          \
    }                                \
}

#define flick_iiop_encode_end_glob(max_size)    /* We don't do squat */
#define flick_iiop_encode_new_chunk(size)   /* Don't do anything */
#define flick_iiop_encode_end_chunk(size) \
    _buf_current = ((char *) _buf_current) + size
#define flick_iiop_encode_new_chunk_align(size, final_bits, init_bits,        \
                      init_ofs) {                 \
    unsigned long _align = (1 << (final_bits)) - 1;                \
                                          \
    _buf_current = ((void *)                          \
            (((unsigned long) (((char *) _buf_current) + _align)) \
             & ~_align));                         \
}

#define flick_iiop_encode_new_glob_plain(max_size, _onerror) {       \
    if (((char *) _buf_end) - ((char *) _buf_current)        \
        < (int) (max_size)) {                    \
            flick_stub_error(FLICK_ERROR_NO_MEMORY,      \
                     _onerror);          \
    }                                \
}

#define flick_iiop_encode_end_glob_plain(max_size)  /* We don't do squat */
#define flick_iiop_encode_new_chunk_plain(size)     /* Don't do anything */
#define flick_iiop_encode_end_chunk_plain(size)

#define flick_iiop_decode_new_glob(max_size)    /* We don't do squat */
#define flick_iiop_decode_end_glob(max_size)    /* Again - do nothing */
#define flick_iiop_decode_new_chunk(size)   /* Fait rien */
#define flick_iiop_decode_end_chunk(size)       \
    _buf_current = ((char *) _buf_current) + size
#define flick_iiop_decode_new_chunk_align(size, final_bits, init_bits,        \
                      init_ofs) {                 \
    unsigned long _align = (1 << (final_bits)) - 1;                \
                                          \
    _buf_current = ((void *)                          \
            (((unsigned long) (((char *) _buf_current) + _align)) \
             & ~_align));                         \
}

/* The following stuff probably belongs in corba_on_iiop.h */

#define flick_iiop_client_encode_target(ref, _ofs, ENCNAME, _onerror) { \
        if (1 == flick_##ENCNAME##_iiop_client_encode_target(ref, _stream, &_stub_state)) \
        goto _onerror; \
    }

#define flick_iiop_client_decode_target(ref, _ofs, ENCNAME, _onerror) \
    if (0) goto _onerror
#define flick_iiop_server_encode_target(ref, _ofs, ENCNAME, _onerror) \
    if (0) goto _onerror
#define flick_iiop_server_decode_target(ref, _ofs, ENCNAME, _onerror) { \
    (ref) = _this_obj;                      \
    if (0) goto _onerror;                       \
}

#define flick_iiop_mark_port_for_cleanup(a, b)

#define flick_iiop_send_encode_target(ref, _ofs, ENCNAME) {     \
    unsigned int __len = (ref)->key._length;            \
    unsigned int __siz = (__len + 7) & ~3;              \
                                    \
    if ((__siz + 24)                        \
        < (((char *) (_msg)->msg) - ((char *) (_msg)->buf))) {  \
        (_msg)->hdr = ((char *) (_msg)->hdr) - __siz;       \
        *((unsigned int *) (_msg)->hdr) = __len;        \
        memcpy(((char *) (_msg)->hdr) + 4,          \
               (ref)->key._buffer,              \
               __len);                      \
    } else {                            \
        /* XXX - Houston, we have a problem. */         \
    }                               \
}

#define flick_iiop_server_decode_client(_ref, _ofs, ENCNAME, _onerror) {    \
    unsigned int _pr_len;                           \
                                        \
    flick_iiop_decode_new_chunk_align(0, 2, 0, 0);              \
    flick_##ENCNAME##_decode_unsigned32((_ofs), _pr_len, unsigned int); \
    if (((_ref) = t_calloc(FLICK_PSEUDO_CLIENT_STRUCT, 1)) == 0)        \
        flick_stub_error(FLICK_ERROR_NO_MEMORY, _onerror);      \
    flick_iiop_msg_decode_principal((_ofs) + 4, (_ref), _pr_len);       \
}

typedef struct flick_target_struct *FLICK_TARGET;
typedef struct flick_pseudo_client_struct *FLICK_PSEUDO_CLIENT;

typedef int flick_invocation_id;

#define _typedef___FLICK_SERVER
typedef flick_operation_success_t (*FLICK_SERVER)(flick_marshal_stream_t,
                          flick_marshal_stream_t,
                          unsigned int,
                          FLICK_TARGET);
typedef int mom_server_t(flick_marshal_stream_t, flick_marshal_stream_t);

#include "ORB-mvcc.h"
#include <flick/pres/corba.h>

typedef struct flick_target_struct {
    CORBA_BOA boa;
    CORBA_ReferenceData key;
    char *type_id;      /* considered a const literal string */
    unsigned int type_id_len;   /* strlen(typeid) */
    char *ior;          /* object's cached IOR */
    unsigned int ior_len;

    /* server side only */
    FLICK_SERVER server_func;
    CORBA_BOA_Client *client;
} FLICK_TARGET_STRUCT;

typedef struct flick_pseudo_client_struct {
    CORBA_BOA boa;
    void *data;
} FLICK_PSEUDO_CLIENT_STRUCT;

typedef void (*flick_client_work_func)(FLICK_PSEUDO_CLIENT,
                       flick_msg_t,
                       flick_invocation_id,
                       FLICK_TARGET);
typedef void (*flick_server_work_func)(FLICK_TARGET,
                       flick_msg_t,
                       flick_invocation_id,
                       FLICK_PSEUDO_CLIENT);

typedef struct {
    flick_invocation_id inv_id;
    FLICK_PSEUDO_CLIENT client;
    FLICK_TARGET        obj;
    flick_client_work_func  func;
} flick_client_table_entry;

/* Pickled message structure */
typedef struct flick_msg_struct_t {
    void *msg;      /* IIOP message from operation to end */
    unsigned int msg_len;   /* Length of above message */
    char response;      /* message flag */
    void *hdr;      /* pointer to beginning of GIOP header */
    void *buf;      /* pointer to beginning of alloced buffer */
    void *principal;    /* pointer to principal (client pseudo obj) */

    void *pvect[8];     /* unmarshaled ``in'' data pointers */

    /*
     * To be filled in by *_encode_request()'s on the client side.
     * This is used in *_send_request()'s to build the table and
     * eventually link the reply to the request and the work function.
     */
    flick_client_work_func func;
} flick_msg_struct_t;

/* types for various presentations */
typedef FLICK_TARGET mom_ref_t;
typedef flick_operation_success_t flick_dispatch_t(FLICK_BUFFER *,
                           FLICK_BUFFER *,
                           unsigned int,
                           FLICK_TARGET);

typedef void (*flick_request_continuer)(FLICK_TARGET,
                    flick_msg_t,
                    flick_invocation_id,
                    FLICK_PSEUDO_CLIENT,
                    void *);
typedef void (*flick_reply_continuer)  (FLICK_PSEUDO_CLIENT,
                    flick_msg_t,
                    flick_invocation_id,
                    FLICK_TARGET,
                    void *);

int flick_client_start_request(FLICK_TARGET obj, CORBA_BOA boa,
        struct flick_request_state *request_state);
int flick_client_wait_request(FLICK_TARGET obj,
        struct flick_request_state *request_state);
int flick_client_finish_request(FLICK_TARGET obj, CORBA_BOA boa,
        struct flick_request_state *request_state);

/* library routines */
int flick_destruct_boa_client(CORBA_BOA boa);
int flick_client_send_request(FLICK_TARGET obj,
                  FLICK_BUFFER *buf);
int flick_client_get_reply(FLICK_TARGET obj,
               FLICK_BUFFER *buf);
int flick_server_get_request(flick_mvcc_channel_t *chan,
                 FLICK_BUFFER *buf);
int flick_server_send_reply(flick_mvcc_channel_t *chan,
                FLICK_BUFFER *buf);

int flick_send_request_msg(FLICK_TARGET, flick_msg_t,
               flick_invocation_id, FLICK_PSEUDO_CLIENT);
int flick_send_reply_msg(FLICK_PSEUDO_CLIENT, flick_msg_t,
             flick_invocation_id, FLICK_TARGET);

int flick_continue_request_msg(FLICK_TARGET, flick_msg_t,
                   flick_invocation_id, FLICK_PSEUDO_CLIENT,
                   flick_request_continuer, void *);
int flick_continue_reply_msg(FLICK_PSEUDO_CLIENT, flick_msg_t,
                 flick_invocation_id, FLICK_TARGET,
                 flick_reply_continuer, void *);

char *find_system_exception_id(char *name, int size);

int flick_server_send_exception(flick_mvcc_channel_t *chan,
                unsigned int request_id,
                const char *exception_type);

#define flick_buffer_contains_more(buf)     \
    ((buf)->buf_end < (buf)->buf_read)

/* Macro operations and functions on messages */
#define flick_iiop_msg_set_response_expected(val, msg) {    \
    (msg)->response = (val);                \
/*  (msg)->replymsg = 0;    * not reply message */      \
}

#define flick_iiop_msg_set_principal_mark(val, _msg) {      \
    (_msg)->principal = ((char *) (_msg)->msg) + (val); \
}

#define flick_iiop_msg_set_reply(msg)       \
    (msg)->replymsg = 1 /* reply message */

#define flick_iiop_msg_set_reply_handler(val, msg)  \
    (msg)->func = (val)

#define flick_iiop_extract_invocation_id(id)    \
    (id) = _request_id

#define flick_iiop_msg_decode_principal(_ofs, cli, len) {            \
    unsigned short cli_port;                         \
    char addr[16];                               \
    CORBA_Environment ev;                            \
                                         \
    if ((len) == 8) {                            \
        char *_pos = (char *) _buf_current + (_ofs);             \
        /* we can be reasonably sure that                \
           this is a client pseudo object */                 \
        cli_port = ((unsigned short *) _pos)[2];             \
        sprintf(addr, "%u.%u.%u.%u",                     \
            (unsigned int) ((unsigned char) _pos[0]),        \
            (unsigned int) ((unsigned char) _pos[1]),        \
            (unsigned int) ((unsigned char) _pos[2]),        \
            (unsigned int) ((unsigned char) _pos[3]));       \
        ev._major = CORBA_NO_EXCEPTION;                  \
        cli->boa = CORBA_ORB_BOA_create(0, "FlickClientBOA--",       \
                        addr, ntohs(cli_port), &ev); \
        if (ev._major != CORBA_NO_EXCEPTION) {               \
            fprintf(stderr, "EXCEPTION: %s\n",           \
                CORBA_exception_id(&ev));            \
        }                                \
        /* XXX - boa isn't initialized... can't do this! *\/         \
        cli->boa->ipaddr->sin_addr.s_addr = cli_ip;          \
        cli->boa->ipaddr->sin.port = cli_port;          */   \
    } else {                                 \
        /* XXX - we need to figure out how to reply */           \
        fprintf(stderr, "Couldn't get client info!!\n");         \
    }                                    \
}


/*
 * Runtime marshal/unmarshal functions.
 */
int         flick_cdr_encode_IOR_internal(flick_marshal_stream_t stream,
                                          struct flick_stub_state *stub_state,
                      FLICK_TARGET obj,
                      const char *link,
                      int ref_adjust);
FLICK_TARGET flick_cdr_decode_IOR_internal(flick_marshal_stream_t stream,
                                          struct flick_stub_state *stub_state,
                       int cdr_swap,
                       const char *link,
                       int ref_adjust);

int flick_cdr_encode_fd_internal(flick_marshal_stream_t stream, CORBA_fd fd, char* fd_valid);
CORBA_fd flick_cdr_decode_fd_internal(flick_marshal_stream_t _stream, char* fd_valid);

int flick_cdr_decode_system_exception(CORBA_Environment *loc,
                                      FLICK_BUFFER *_stream,
                                      struct flick_stub_state *stub_state);

int flick_cdr_encode_system_exception(CORBA_Environment *loc,
                                      FLICK_BUFFER *_stream,
                                      struct flick_stub_state *stub_state);

void flick_iiop_client_start_encode_internal(CORBA_Object _obj,
        FLICK_BUFFER *_stream, struct flick_request_state *_request_state);

int flick_cdr_iiop_client_encode_target(CORBA_Object ref, FLICK_BUFFER *_stream,
        struct flick_stub_state *stub_state);

void flick_iiop_client_start_decode_internal(CORBA_Object _obj,
        FLICK_BUFFER *_stream);

int flick_corba_cdr_iiop_client_error(CORBA_Environment *_ev,
        struct flick_stub_state *stub_state);


#endif /* __flick_link_iiop_h */

/* End of file. */

