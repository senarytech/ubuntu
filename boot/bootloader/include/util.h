/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */

#ifndef	__UTIL_H__
#define	__UTIL_H__

#include <stddef.h>
#include <stdint.h>
#include "com_type.h"
#include "io.h"
#include "timer.h"

void *	UtilMemSet(void *s, int c, int n);
void *	UtilMemCpy(void *s1, const void *s2, int n);
int	UtilMemCmp(const void *s1, const void *s2, int n);
void *	UtilMemAllocZ(int size);
void    UtilMemReset(void);

void	UtilHalt(void);

//used for bitmaps that could be more than 32 bits wide
#define BITMASK(b)		(1 << ((b) % 8))
#define BITSLOT(b)		((b) / 8)
#define BITSET(a, b)	((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b)	((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b)	((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb)	((nb + 8 - 1) / 8)

#ifndef BFM_HOST_Bus_Write32
#define BFM_HOST_Bus_Write32(offset, val)	((*(UNSG32*)((uintmax_t)offset))=val)
#endif

#ifndef BFM_HOST_Bus_Read32
#define BFM_HOST_Bus_Read32(offset, holder)	((*(UNSG32*)(holder))=(*(UNSG32*)((uintmax_t)offset)))
#endif
#define BFM_HOST_Bus_Write16(offset, val)	((*(unsigned short*)((uintmax_t)offset))=val)
#define BFM_HOST_Bus_Read16(offset, holder)	((*(unsigned short*)(holder))=(*(unsigned short*)((uintmax_t)offset)))
#define BFM_HOST_Bus_Write8(offset, val)	((*(unsigned char*)((uintmax_t)offset))=val)
#define BFM_HOST_Bus_Read8(offset, holder)	((*(unsigned char*)(holder))=(*(unsigned char*)((uintmax_t)offset)))


#define CMD_BUF_SIZE	2048
#define	CMD_LINE_BUFFER_SIZE		CMD_BUF_SIZE

#define PROMPT "BerlinDebug>"

#define	UtilMemAlloc malloc

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);

void free(void *ptr);

static inline int ctoi(char c)
{
	if((c>='0')&&(c<='9'))
		return c-'0';
	else if((c>='a')&&(c<='f'))
		return c-'a' + 10;
	else if((c>='A')&&(c<='F'))
		return c-'A' + 10;
	else
		return -1;
}

static inline char itoc(char i)
{
	int j = i&0xf;
	if(j<10)
		return j+'0';
	else
		return j-10+'A';
}

#ifndef GET_ALIGNED
static inline unsigned get_aligned(unsigned address, unsigned page_size) {
	return (address + page_size - 1) / page_size * page_size;
}
#define GET_ALIGNED
#endif

/* defined in uish.S */
extern void *__user_read_heap_top(void);
extern void *__user_read_heap_bottom(void);
#endif	/* __UTIL_H__	*/
