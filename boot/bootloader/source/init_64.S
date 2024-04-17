/*
 *  Copyright Marvell Semiconductor, Inc. 2006. All rights reserved.
 *
 *  This code initialises the Integrator board (eg REMAP) before calling
 *  TCM Initialization and MMU Initialization if they exist.
 *  this allows scatter loading to relocate code into the TCMs
 *
 *  This code must be run from a privileged mode
 */

/* SCTLR definitions */
#include "mmu_config_64.h"

#define SCTLR_EL2_RES1  ((1 << 29) | (1 << 28) | (1 << 23) | (1 << 22) | \
			(1 << 18) | (1 << 16) | (1 << 11) | (1 << 5) |  \
			(1 << 4))

#define SCTLR_EL1_RES1  ((1 << 29) | (1 << 28) | (1 << 23) | (1 << 22) | \
			(1 << 11))
#define SCTLR_AARCH32_EL1_RES1 \
			((1 << 23) | (1 << 22) | (1 << 11) | (1 << 4) | \
			(1 << 3))

#define MODE_NS_AARCH64_EL2_H	((1<<5) | (2<<2) | (1<<0))

#define SCTLR_M_BIT		(1 << 0)
#define SCTLR_A_BIT		(1 << 1)
#define SCTLR_C_BIT		(1 << 2)
#define SCTLR_SA_BIT		(1 << 3)
#define SCTLR_I_BIT		(1 << 12)
#define SCTLR_WXN_BIT		(1 << 19)
#define SCTLR_EE_BIT		(1 << 25)

/*
 * MMU OFF
 * D Cache OFF
 * r0	- block size
 * r1	- page size
 * r2	- addr_cycle
 */
	.text
.globl BootLoaderEntry
BootLoaderEntry:

#if 0  
//Cash: Hacking bl_normal to bypass sstz and jump to post_bl directly
		mrs	x0, HCR_EL2
		orr	x0, x0, #(1<<31)	//RW=1, EL1 is AArch64
		msr	HCR_EL2, x0

		mrs	x0, SCR_EL3
		orr     x0, x0, #(1<<10)            // RW=1 AArch64 mode for EL2  
		bic     x0, x0, #(1<<0)             // NS=0 security for EL0/El1  
		bic     x0, x0, #((1<<3) | (1<<2) | (1<<1))        // EA=0, FIQ=0, IRQ=0  
		msr     SCR_EL3, x0

		mrs     x0, SCTLR_EL2               // reset value 0x00c52838  
		bic     x0, x0, #0x00000001         // disable MMU  
		bic     x0, x0, #0x00001000         // disable I cache  
		bic     x0, x0, #0x00000004         // disable D cache  
		msr     SCTLR_EL2, x0
		
		mov	x0, #MODE_NS_AARCH64_EL2_H  
		smc 	0

#endif
		mov 	x3, #1
loop1:
		cmp		x3, #0
		b.eq	loop1

		//save x0, x1, x2
		mov		x19, x0
		mov		x20, x1
		mov		x21, x2

		mrs     x0, DAIF
		msr     DAIFSET, #3                 /* disable interrupt */

		/*
		 * Clear BSS so that there are no surprises
		 */
		ldr 	x0, =__bss_start
		ldr 	x1, =__bss_stop
z_loop:
		cmp x0, x1
		b.eq	z_end
		stp     xzr, xzr, [x0], #16
		b	z_loop
z_end:

		mrs	x0, hcr_el2
		orr x0, x0, #0xf8
		msr	hcr_el2, x0
		isb sy

		ic	ialluis

		mrs x0, sctlr_el2
		orr x0, x0, #SCTLR_I_BIT
		bic x0, x0, #SCTLR_M_BIT
		bic x0, x0, #SCTLR_C_BIT
		msr sctlr_el2, x0

		isb sy

		/* ---------------------------------------------
		 * Set the exception vector to something sane.
		 * ---------------------------------------------
		 */
		adr	x0, exception_handler
		msr	vbar_el2, x0
		isb

		ldr x0, = __svc_stack_start
		mov sp, x0

#ifdef CONFIG_DCACHE
		bl	enable_dcache
#endif

		/*
		 * branch to C Library entry point
		 */
		mov		x0, x19
		mov		x1, x20
		mov		x2, x21

		bl		StartBootLoader

panic:
		b		panic
