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

#include <stdint.h>

#include "util.h"
#include "ctypes.h"
#include "string.h"
#include "debug.h"
#include "apbRegBase.h"
//#include "lgpl_printf.h"

/*
 * retrieve top and bottom of heap
 */
extern int __heap_start_usr, __heap_end_usr;

inline void * __user_read_heap_bottom(void)
{
	return (void *)((uintptr_t)__heap_start_usr);
}

inline void * __user_read_heap_top(void)
{
	return (void *)((uintptr_t)__heap_end_usr);
}

/*
 * heap start point
 */
void *	UtilMemSet(void *s, int c, int n)
{

	unsigned char *	char_ptr = s;
	unsigned int *	int_ptr = s;
	int		remainer;

	if((remainer = ((uintmax_t)s & 3)) != 0) {
		while (remainer-- > 0) {
			*char_ptr++ = (unsigned char)c;
			n--;
		}
		int_ptr = (unsigned int *)char_ptr;
		remainer = n & 3;
	}

	n >>= 2;
	while (n-- > 0) {
		*int_ptr++ = (unsigned int)c;
	}

	if (remainer != 0) {
		unsigned int	temp = *int_ptr;
		unsigned char *	temp_p = (unsigned char *)&temp;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)c;
		}
		*int_ptr = temp;
	}
	return(s);
}

void *	UtilMemCpy(void *s1, const void *s2, int n)
{

	unsigned int *		dst = s1;
	const unsigned int *	src = s2;
	int			remainer = n & 3;

	if (((uintmax_t)s1 & 3) || ((uintmax_t)s2 & 3)) {
		unsigned char *		dst_char = s1;
		const unsigned char *	src_char = s2;

		while (n-- > 0) {
			*dst_char++ = *src_char++;
		}
		return s1;
	}

	n >>= 2;
	while (n-- > 0) {
		*dst++ = *src++;
	}

	if (remainer != 0) {
		unsigned int		temp = *dst;
		unsigned char *		temp_p = (unsigned char *)&temp;
		const unsigned char *	src_c = (const unsigned char *)src;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)*src_c++;
		}
		*dst = temp;
	}

	return(s1);
}

int	UtilMemCmp(const void *s1, const void *s2, int n)
{

	int			i = 0;
	const unsigned char *	ss1 = s1;
	const unsigned char *	ss2 = s2;

	for ( ; i < n; i++) {
		if (ss1[i] > ss2[i]) {
			return(i + 1);
		}
		else if (ss1[i] < ss2[i]) {
			return(-i - 1);
		}
	}

	return 0;
}

typedef struct _mem_node{
        void * ptr;
        unsigned int size;
        int is_used;
}mem_node;

#define MAX_ALLOC_NUM 128
static mem_node mem_table[MAX_ALLOC_NUM] = {{0}};
static int allocator_initialized = 0;
extern int __heap_start_usr;
extern int __heap_end_usr;
#define HEAP_START ((uintmax_t)(__heap_start_usr))
#define HEAP_END ((uintmax_t)(__heap_end_usr))
#define DCACHE_LINE_LEN 0x40
#define CACHE_MASK (0xFFFFFFFF&~(DCACHE_LINE_LEN - 1))
#define ALIGNED(addr) ((addr&CACHE_MASK) + DCACHE_LINE_LEN)

static void init_allocator(void)
{
        UtilMemSet(&mem_table[0], 0, sizeof(mem_node)*MAX_ALLOC_NUM);
        mem_table[0].ptr = (void *)ALIGNED(HEAP_START);// allocate addr always DCACHE_LINE_LEN alligned.
        mem_table[0].size = ALIGNED(HEAP_END) - DCACHE_LINE_LEN - ALIGNED(HEAP_START);
        mem_table[0].is_used = 0;
        allocator_initialized = 1;
}


static mem_node * find_available_mem_node(void)
{
        int i;
        for (i = 0; i < MAX_ALLOC_NUM; i++) {
                if (mem_table[i].ptr == NULL) {
                        return &mem_table[i];
                }
        }

        return NULL;
}

void *malloc(size_t size)
{
        if (allocator_initialized == 0) {
                init_allocator();
        }

        if (size == 0) {
                return 0;
        }

        int i;
        size_t size_aligned = ALIGNED(size);
        for (i = 0; i < MAX_ALLOC_NUM; i++) {
                if (mem_table[i].is_used == 0 && mem_table[i].size >= size_aligned) {

                        mem_node *buddy = find_available_mem_node();
                        if (buddy != NULL) {
                                buddy->ptr = mem_table[i].ptr + size_aligned;
                                buddy->size = mem_table[i].size - size_aligned;
                        }

                        mem_table[i].size = size_aligned;
                        mem_table[i].is_used = 1;

                        return mem_table[i].ptr;
                }
        }

        return 0;

}

void *	UtilMemAllocZ(int size)
{
	void *ptr = malloc(size);

        if (ptr != NULL) {
                UtilMemSet(ptr, 0, size);
        }

        return ptr;
}

static mem_node *find_left_buddy(mem_node *curr)
{
        int i;

        if(curr) {
                for (i = 0; i < MAX_ALLOC_NUM; i++) {
                        if((mem_table[i].ptr + mem_table[i].size) == curr->ptr) {
                                return &mem_table[i];
                        }
                }
        }

        return NULL;
}


static mem_node * merge_left_buddy(mem_node *l_buddy, mem_node *curr)
{
        l_buddy->size += curr->size;
        UtilMemSet(curr, 0, sizeof(mem_node));
        return l_buddy;
}

static mem_node *find_right_buddy(mem_node * curr)
{
        int i;

        if(curr) {
                for (i = 0; i < MAX_ALLOC_NUM; i++) {
                        if(mem_table[i].ptr == curr->ptr + curr->size) {
                                return &mem_table[i];
                        }
                }
        }

        return NULL;
}


static mem_node * merge_right_buddy(mem_node *r_buddy, mem_node *curr)
{
        curr->size += r_buddy->size;
        UtilMemSet(r_buddy, 0, sizeof(mem_node));
        return curr;
}

static void merge_buddy(mem_node *free_node)
{
        mem_node *l_buddy, *r_buddy, *curr = free_node;

        l_buddy = find_left_buddy(curr);
        while (l_buddy != NULL && l_buddy->is_used == 0) {
                curr = merge_left_buddy(l_buddy, curr);
                l_buddy = find_left_buddy(curr->ptr);
        }

        r_buddy = find_right_buddy(curr);
        while (r_buddy != NULL && r_buddy->is_used == 0) {
                curr = merge_right_buddy(r_buddy, curr);
                r_buddy = find_right_buddy(curr);
        }
}


void free(void *ptr)
{
        if (ptr == NULL) {
                return;
        }

        int i;
        for (i = 0; i < MAX_ALLOC_NUM; i++) {
                if (mem_table[i].ptr == ptr) {
                        mem_table[i].is_used = 0;
                        merge_buddy(&mem_table[i]);
                        return;
                }
        }

}

void	UtilMemReset(void)
{
        allocator_initialized = 0;
}

void * calloc(size_t nmemb, size_t size)
{
        void *ptr = UtilMemAllocZ(nmemb*size);
        return ptr;
}


#define DBG_MEM	                (0x3000000)
#define DBG_PORT_BASE			(SM_APB_UART0_BASE)
#define PUT_CHAR(ch)  BFM_HOST_Bus_Write32((DBG_PORT_BASE+0x00), (UNSG32)ch)
#define	put_char	PUT_CHAR

#define MAX_HISBUF_ENTRY 16
static char his_buf[MAX_HISBUF_ENTRY][80];      //  for Historical commands support
static SIGN32 his_mode=0;
SIGN32 his_next_idx=0;
SIGN32 his_cur_idx=0;
SIGN32 his_cmd_cnt=0;
SIGN32 pre_his_cmd_length;

#if 0
unsigned char GET_CHAR()
{
	UNSG32 data32;
	unsigned char data8;
	BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x00), &data32);

	data8=data32&0xFF;
	return data8;

}
#endif

////////////////////////////////////////////////////////////
// dbg port (uart) driver
SIGN32 dbg_port_tx_ready()
{
	UNSG32 status=0;
	do
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x7c),&status);
	} while ((status&0x03)!=0x02);
	// add extra protection below
	do
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x80),&status);
	} while (status>2);  //delay control
	return 1;
}

/////////////////////////////////////////////////////////////////////////
//
//  showDbgPrompt() - Used by the debugger.  Displays the appropriate
//                debugger prompt.
//
/////////////////////////////////////////////////////////////////////////
void showDbgPrompt(char* prompt)
{
    lgpl_printf("\n\n%s", prompt);
}


#if 0
SIGN32 dbg_port_rx_ready()
{
	UNSG32 read;
	BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x7c),&read);      // status
	if(read&0x08)
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x84),&read);  // RFL
		if (read>0)
		return 1;
	}
	return 0;
}
#endif

void dbg_port_print( char* str)
{
	char ch;
	SIGN32 i;
	SIGN32 len=strlen(str);
	dbg_port_tx_ready();
	for (i=0;i<len;i++)
	{
		ch=str[i];
		put_char(ch);
		if (ch=='\n')
		{
			ch='\r';
			put_char(ch);
		}
		//extra delay
		i=i;
		len=len;
		ch=str[i];
	}
}


int previous_cmd(char *pCmd, int cur_cmd_length, unsigned int loop)
{
	int i, j;
	int erase;

	if (his_cmd_cnt == 0)
		return (cur_cmd_length - 3);

	if (his_mode == 0)
	{
		his_mode = 1;
	}
	else
	{
		if (loop == 0)
		{
			if (his_next_idx == his_cur_idx + 1)
				return (cur_cmd_length - 3);

			if ((his_next_idx == 0) && (his_cur_idx == his_cmd_cnt))
				return (cur_cmd_length - 3);
		}

		if (his_next_idx == 0)
			his_next_idx = his_cmd_cnt - 1;
		else
			his_next_idx--;
	}

	// need to clear line, move cursor, and print prompt again
	erase = strlen(PROMPT) + (pre_his_cmd_length > cur_cmd_length ? pre_his_cmd_length : cur_cmd_length);
	put_char('\r');
	for (j = 0; j <= erase ; j++)
	{
		put_char(' ');
	}
	put_char('\r');   		// back to beginning of line
	dbg_port_print(PROMPT);	// show prompt

	dbg_port_print(his_buf[his_next_idx]);

	memcpy(pCmd, his_buf[his_next_idx], strlen(his_buf[his_next_idx]));
	i = strlen(his_buf[his_next_idx]) - 1;
	pre_his_cmd_length = i;		// record the his command length

	return i;
}

int next_cmd(char *pCmd, int cur_cmd_length)
{
	int i, j;
	int erase;

	his_mode = 2;
	if (his_cur_idx == his_next_idx + 1)
		return (cur_cmd_length - 3);

	if ((his_cur_idx == 0) && (his_next_idx == his_cmd_cnt))
		return (cur_cmd_length - 3);

	if (his_next_idx == his_cmd_cnt - 1)
		his_next_idx = 0;
	else
		his_next_idx++;

	// need to clear line, move cursor, and print prompt again
	erase = strlen(PROMPT) + (pre_his_cmd_length > cur_cmd_length ? pre_his_cmd_length : cur_cmd_length);
	put_char('\r');
	for (j = 0; j <= erase; j++)
	{
		put_char(' ');
	}
	put_char('\r');   		// back to beginning of line
	dbg_port_print(PROMPT);	// show prompt

	dbg_port_print(his_buf[his_next_idx]);

	memcpy(pCmd, his_buf[his_next_idx], strlen(his_buf[his_next_idx]));
	i = strlen(his_buf[his_next_idx]) - 1;
	pre_his_cmd_length = i;		// record the his command length

	return i;
}

/////////////////////////////////////////////////////////////////////////
//
//  skipSpace() - Used by the debugger.  Skips non-text characters at
//                the beginning of a string
//
/////////////////////////////////////////////////////////////////////////
void skipSpace ( char **inStr )
{
    char *s = *inStr;

    while (( *s == ' ' ) || ( *s == '\t' ) || ( *s == '\r' ) || ( *s == '\n' ))
        s++;

    *inStr = s;
}


/////////////////////////////////////////////////////////////////////////
//
//  getTokenLength() - Used by the debugger.  Gets the length of the
//                     next token in a string
//
/////////////////////////////////////////////////////////////////////////
SIGN32 getTokenLength (char *s )
{
    SIGN32     index;
    char    x;

    for ( index = 0; s[ index] != 0; index++ )
    {
        x = s[ index];
        if (( x == ' ' ) || ( x == '\t' ) || ( x == '\r' ) || ( x == '\n' ))
            break;
    }

    return( index );
}

/////////////////////////////////////////////////////////////////////////
//
//  getIntToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char*  getIntToken ( char *inStr, UNSG32 *outVal )
{
    SIGN32  iSum = 0;
    char  c;
    char  *thisStr = inStr;
	SIGN32  bPositive = 1;
    // If first character is ';', we have a comment, so
    // get out of here.
    if ( *thisStr == ';' )
        return thisStr;

    // Process decimal characters
    // a negative number?
    if ( *thisStr == '-' )
    {
        bPositive =0;
        thisStr ++;
    }

    while ( *thisStr )
    {
        c = *thisStr;

        // Check for valid digits.
        if ( !( c >= '0' && c <= '9' ) )
            break;

        // ASCII to decimal conversion
        iSum = iSum * 10 + ( c - '0' );

        thisStr ++;
    }

    // make it negative
    if ( !bPositive )
        iSum = -1 * iSum;

    // Return updated pointer and decoded value.
    *outVal = iSum;
    return thisStr;
}

/////////////////////////////////////////////////////////////////////////
//
//  getSignedIntToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char*  getSignedIntToken ( char *inStr, SIGN32 *outVal )
{
    SIGN32  iSum = 0;
    char  c;
    char  *thisStr = inStr;
	SIGN32  bPositive = 1;
    // If first character is ';', we have a comment, so
    // get out of here.
    if ( *thisStr == ';' )
        return thisStr;

    // Process decimal characters
    // a negative number?
    if ( *thisStr == '-' )
    {
        bPositive =0;
        thisStr ++;
    }

    while ( *thisStr )
    {
        c = *thisStr;

        // Check for valid digits.
        if ( !( c >= '0' && c <= '9' ) )
            break;

        // ASCII to decimal conversion
        iSum = iSum * 10 + ( c - '0' );

        thisStr ++;
    }

    // make it negative
    if ( !bPositive )
        iSum = -1 * iSum;

    // Return updated pointer and decoded value.
    *outVal = iSum;
    return thisStr;
}

/////////////////////////////////////////////////////////////////////////
//
//  getHexToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char* getHexToken ( char *inStr, UNSG32 *outVal )
{
    UNSG32   thisVal, tVal;
    char    x;
    char    *thisStr = inStr;

    thisVal = 0;

        // If first character is ';', we have a comment, so
        // get out of here.

    if ( *thisStr == ';' )
        return( thisStr );

        // Process hex characters.

    while ( *thisStr )
    {
        // Do uppercase conversion if necessary.

        x = *thisStr;
        if (( x >= 'a') && ( x <= 'f'))
            x &= ~0x20;

        // Check for valid digits.

        if ( !((( x >= '0') && (x <= '9')) || (( x >= 'A') && ( x <= 'F'))))
            break;

        // Hex ASCII to binary conversion.

        tVal = x - '0';
        if ( tVal > 9 )
            tVal -= 7;

        thisVal = (thisVal * 16) + tVal;

        thisStr++;
    }

        // Return updated pointer and decoded value.

    *outVal = thisVal;
    return( thisStr );
}

/////////////////////////////////////////////////////////////////////////
//
//  getToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char* getToken ( char *inStr, char **outVal )
{
    char    *thisStr = inStr;

        // If first character is ';', we have a comment, so
        // get out of here.

    if ( *thisStr == ';' )
        return( thisStr );

    while ( *thisStr != ' ' && *thisStr != '\t' && *thisStr != '\0' )
    {
        thisStr++;
    }

    if ( *thisStr != '\0' )
    {
        *thisStr = '\0';
        thisStr ++;
    }

    *outVal = inStr;
    return( thisStr );
}


// Qingwei Huang added for google's toolchain
int raise(int i)
{
    (void)i;
    return 0;
}
