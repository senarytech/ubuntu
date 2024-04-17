/*
 *  Copyright Marvell Semiconductor, Inc. 2006. All rights reserved.
 *
 *  This code initialises the Integrator board (eg REMAP) before calling
 *  TCM Initialization and MMU Initialization if they exist.
 *  this allows scatter loading to relocate code into the TCMs
 *
 *  This code must be run from a privileged mode
 */

#include "memmap.h"
#include "ra_gbl.h"
#if (BERLIN_CHIP_VERSION < BERLIN_BG4DTV)
#include "gic.h"
#endif
/*
 * cpu id register
 */
#define PROCESSOR_ID_REG_BASE           (MEMMAP_PROCESSOR_ID_REG_BASE)

/*
 * chip control registers
 */
#define CHIP_CTRL_REG_BASE              (0x00 + MEMMAP_CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_PIN_MUX_REG           (RA_Gbl_pinMux + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_BOOT_STRAP_REG        (RA_Gbl_bootStrap + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SYS_PLL_CTL           (RA_Gbl_sysPllCtl + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RA_GBL_CLK_SELECT     (RA_Gbl_clkSelect + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RA_GBL_CLK_SWITCH     (RA_Gbl_ClkSwitch + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RESET_TRIGGER_REG     (RA_Gbl_ResetTrigger + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RESET_STATUS_REG      (RA_Gbl_ResetStatus + CHIP_CTRL_REG_BASE)


/*
 * Declear Extra Features Regsiters -R15
 */
#define	R15_NCNB_WAIT			(0x01 << 30)
#define	R15_STREAM_EN			(0x01 << 29)
#define	R15_WR_ALLOC_EN			(0x01 << 28)

#define MODE_MASK                               (0x1F)
#define FIQ_MODE                                (0x11)
#define IRQ_MODE                                (0x12)
#define SVC_MODE   			        (0x13)
#define USER_MODE                               (0x10)
#define ABORT_MODE                              (0x17)
#define UNDEF_MODE                              (0x1B)


.macro smp_processor_id, ret
	mrc p15, 0, \ret, c0, c0, 5
	and \ret, \ret, #0xf
.endm

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
		//save r0, r1, r2
		mov		r4, r0
		mov		r5, r1
		mov		r6, r2

#ifndef CONFIG_FPGA
#if (BERLIN_CHIP_VERSION < BERLIN_BG4DTV)
		/* enter wfi state if not primary cpu */
		smp_processor_id r0
		cmp		r0, #0
		blne		cpu_enter_wfi
#endif
#endif

		/*
		 * set stack for irq stack
		 */
		MRS		r0, cpsr
		BIC		r0, r0, #MODE_MASK
		ORR		r0, r0, #IRQ_MODE
		MSR		cpsr, r0               //      enter irq mode
		ldr		sp, =__irq_stack_start                 //  setup irq stack pointer

		/*
		 * set stack for supervisor mode
		 */
		MRS		r0, cpsr		// load program status register
		BIC		r0, r0, #MODE_MASK	// r0 = r0 & !0x1F
		ORR		r0, r0, #SVC_MODE	// r0 = r0 | 0xD3
		MSR		cpsr_c, r0		// Supervisor with disabled IRQ/FIQ

		ldr		sp, =__svc_stack_start

		/*
		 * setup important register
		 * setup memory timing
		 */

		/*
		* disable MMU/PU stuff and caches
		*/
		MRC		p15, 0, r0, c1, c0, 0
		BIC		r0, r0, #0x00001000             /* clear bits 12 (---I)        */
		BIC		r0, r0, #0x00000005             /* clear bits 2:0 (-D-M)       */
		MCR		p15, 0, r0, c1, c0, 0

		/*
		 * invalidate I/D caches
		 */
		MOV		r0, #0
		MCR		p15, 0, r0, c7, c5, 0           /* Invalidate entire instuction cache   */
		dsb
		MCR		p15, 0, r0, c7, c1, 0		/* Invalidate entire I Cache inner shareable		*/
		BL		invalidate_dcache_all

		BL		InitMultiLevelMMU		/* Initial MMU & enable caches */
		BL		enable_branch_prediction

		/*
		 * Clear BSS so that there are no surprises
		 */
		ldr		r0, =__bss_start
		ldr		r1, =__bss_stop
		mov		r2, #0
1:
		stmia	r0!, {r2}
		cmp		r1, r0
		bgt		1b

		/*
		 * branch to C Library entry point
		 */
		mov		r0, r4
		mov		r1, r5
		mov		r2, r6
		MOV		lr, pc
		LDR		pc, _c_entry
		SWI		0x123456			/* Termination Sign of ARM applications	*/

_c_entry:
		.word		StartBootLoader

#ifndef CONFIG_FPGA
#if (BERLIN_CHIP_VERSION < BERLIN_BG4DTV)
#define CPU_WAKEUP_ENTRY_REG	(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sw_generic1)

	/*
	 * CPU enter WFI state
	 */
	.globl	cpu_enter_wfi
cpu_enter_wfi:
	ldr	r1, =GIC_CPUControl
	mov	r0, #1
	str	r0, [r1, #0x0]		@ ICC-ICR = 0x1, enable CPU interrupt
	mov	r0, #0xff
	str	r0, [r1, #0x4]		@ ICC-PMR = 0xf0, set priority mask 0xf8

	ldr	r1, =CPU_WAKEUP_ENTRY_REG
	mov	r0, #0
	str	r0, [r1]		@ jump address register = 0x0

	dsb				@ ensure upon instructions are completed

1:
	wfi
	ldr	r0, [r1]
	cmp	r0, #0
	movne	pc, r0
	b	1b
#endif
#endif
