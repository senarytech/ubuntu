#ifndef _REE_SYS_CALLBACK_H_
#define _REE_SYS_CALLBACK_H_

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>

#ifndef REE_MUTEX_NAME_MAX_LEN
#define REE_MUTEX_NAME_MAX_LEN	32
#endif

#ifndef REE_SEMAPHORE_NAME_MAX_LEN
#define REE_SEMAPHORE_NAME_MAX_LEN	32
#endif

typedef struct REE_MutexList
{
	spinlock_t		lock;
	int			count;
	struct list_head	head;
} REE_MutexList;

typedef struct REE_Mutex
{
	struct list_head	node;
	struct mutex		mutex;
	int			refCount;
	char			name[REE_MUTEX_NAME_MAX_LEN + 1];
} REE_Mutex;

typedef struct REE_SemaphoreList
{
	spinlock_t		lock;
	int			count;
	struct list_head	head;
} REE_SemaphoreList;

typedef struct REE_Semaphore
{
	struct list_head	node;
	struct semaphore	sem;
	int			refCount;
	char			name[REE_SEMAPHORE_NAME_MAX_LEN + 1];
} REE_Semaphore;

void REE_SysCallbackInit(void);

int REE_RuntimeInit(void);

#endif /* _REE_SYS_CALLBACK_H_ */
