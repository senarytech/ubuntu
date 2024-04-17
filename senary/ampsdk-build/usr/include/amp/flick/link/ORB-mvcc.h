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

#ifndef _flick_link_ORB_h
#define _flick_link_ORB_h

#include "OSAL_api.h"

#include <flick/pres/corba.h>
/* Maximum # of concurrently outstanding client requests */
#define FLICK_MAX_REQUEST_NUM   10
#define FLICK_RPC_TIMEOUT       5000
/*
 * The following `typedef's are contained in <flick/pres/corba.h> because a
 * CORBA presentation includes these (opaque) types.  The actual structures are
 * defined below.
 *
 * typedef struct CORBA_ORB_type *CORBA_ORB;
 * typedef struct CORBA_BOA_type *CORBA_BOA;
 */

struct CORBA_ORB_type {
    char *ORBid;
    int OA_count;
    CORBA_BOA *boas;
};

/* This struct is used by the server to track each client it's talking to. */
typedef struct flick_mvcc_client_info {
    CORBA_BOA_Client                        base;
    int                                     socket;
    UINT32                                  controlQueueId;
    void                                    *context;
    CORBA_BOA                               boa;
    MV_OSAL_HANDLE_TASK_t                   task;
    struct flick_mvcc_client_info           *next;
    MV_OSAL_HANDLE_SEM_t                    initsem;
    int                                     destroying;
} flick_mvcc_client_info_t;

struct flick_server_job {
    int                                     busy;
    flick_mvcc_channel_t                    *channel;
    FLICK_BUFFER                            the_buf;
    FLICK_BUFFER                            return_buf;
    unsigned int                            request_id;
    FLICK_TARGET                            obj;
    MV_OSAL_HANDLE_TASK_t                   thread;
    MV_OSAL_HANDLE_SEM_t                    sem;
    flick_mvcc_client_info_t                *client;
    int                                     jobnr;
    int                                     tid;
    MV_HANDLE_Timer_t                       watchtimer;
    char                                    thread_name[32];
};

struct CORBA_BOA_type {
    FLICK_BUFFER *in, *out;
    CORBA_ORB orb;
    struct sockaddr_un *unaddr;
    char *hostname;
    unsigned short hostport;
    int socket_fd;  /* For communicate with peer (conn thread in server) */
    int connected;  /* 0 if the socket isn't connected already */
    /* Client uses these buffers; server has lots of buffers on the
       stack of each thread, one for each client. */
    flick_mvcc_channel_t writeChannel;
    flick_mvcc_channel_t readChannel;
    int client_sock;/* Server stores socket here used to talk to the client */
    char *OAid;

    /* This is a count of refs on a server,
       on a client, it's the FLICK_TARGET ref count */
    int count_servers;
    /* This is for servers */
    MV_OSAL_HANDLE_MUTEX_t freezeLock;
    MV_OSAL_HANDLE_MUTEX_t refsLock;
    FLICK_TARGET refs;

    /* This is only used from within the client object list above */
    unsigned int max_name_len;

    MV_OSAL_HANDLE_MUTEX_t mutex;
    MV_OSAL_HANDLE_TASK_t reply_thread;
    MV_OSAL_HANDLE_TASK_t monitor_thread;
    int have_reply_thread;
    MV_OSAL_HANDLE_SEM_t wakeup_sem;
    MV_OSAL_HANDLE_SEM_t request_gate_sem;
    struct {
        int seqnum;
        int busy;
        MV_OSAL_HANDLE_SEM_t sem;
    } requests[FLICK_MAX_REQUEST_NUM];

    MV_OSAL_HANDLE_SEM_t job_pool_sem;
    struct flick_server_job job_pool[FLICK_MAX_REQUEST_NUM];
    void (*client_created)(CORBA_BOA, void *);
    void (*client_destroyed)(CORBA_BOA, void *);
    void (*client_event_stop)(void *);
    void (*client_request_start)(void *);
    void (*client_request_end)(void *);
    void (*server_destroyed)();
};

/*
 * `IOP' stuff that we need in order to deal with CORBA Interoperable Object
 * References (IORs).  See Section 10.6.2 of the CORBA 2.0 specification.
 */

typedef CORBA_unsigned_long IOP_ProfileId;
#define IOP_TAG_INTERNET_IOP (0)
#define IOP_TAG_MULTIPLE_COMPONENTS (1)

/*
 * All of the ORB, BOA, and Object interface operations are now prototyped in
 * `flick/pres/corba.h', because (1) those interfaces are arguably part of the
 * CORBA ``presentation'' and (2) because those prototypes rely on other parts
 * of the CORBA presentation (e.g., the presented basic CORBA types).
 */

#endif                          /* _flick_link_ORB_h */

/* End of file. */
