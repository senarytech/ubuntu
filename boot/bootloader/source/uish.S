/*
 *  Copyright ARM Ltd 2002. All rights reserved.
 *
 *  This implementation of __user_initial_stackheap places the stack and heap
 *  using absolute value parameters.
 *  The memory locations were chosen to be suitable to the Integrator platform.
 *
 *  The default build implements a one region stack and heap model
 *  To implement a two region model, predefine TWO_REGION_MODEL on the command line.
 *  ie: armasm -g -PD "TWO_REGION_MODEL SETL {TRUE}" uish.s
 */

#include "memmap.h"
#include "ra_gbl.h"

#define RA_vProRst_vPro		(0x0004)
#define RA_vProRst_vproBiu	(0x0008)

#define PROCESSOR_ID_REG_BASE		(0x00 + MEMMAP_PROCESSOR_ID_REG_BASE)

#define	CHIP_CTRL_REG_BASE		(0x00 + MEMMAP_CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_PIN_MUX_REG		(RA_Gbl_pinMux + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_BOOT_STRAP_REG	(RA_Gbl_bootStrap + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SYS_PLL_CTL		(RA_Gbl_sysPllCtl + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RA_GBL_CLK_SELECT	(RA_Gbl_clkSelect + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RA_GBL_CLK_SWITCH	(RA_Gbl_ClkSwitch + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RESET_TRIGGER_REG	(RA_Gbl_ResetTrigger + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RESET_STATUS_REG	(RA_Gbl_ResetStatus + CHIP_CTRL_REG_BASE)
/*
 * #define	RESET_CPU0_TRIGGER		(bGbl_ResetTrigger_cpu0SyncReset << LSb32Gbl_ResetTrigger_cpu0SyncReset)
 * #define	RESET_CPU1_TRIGGER		(bGbl_ResetTrigger_cpu1SyncReset << LSb32Gbl_ResetTrigger_cpu1SyncReset)
*/
#define CHIP_CPU0_RESET_REG			(0x0+MEMMAP_SOC_REG_BASE)

#define	RESET_CPU0_TRIGGER			(bCPUConfig_cpuSoftReset<<BA_CPUConfig_cpuSoftReset)
#define	RESET_CPU1_TRIGGER			(bCPUConfig_cpuSoftReset<<BA_CPUConfig_cpuSoftReset)

#define CHIP_CTRL1_CPU0_RWCT             (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ChipCntl1)



	/*
	 * void *	__user_read_heap_bottom(void)
	 */
_heap_bottom:
		.word		__heap_start

.global	__user_read_heap_bottom
__user_read_heap_bottom:
		LDR		r0, _heap_bottom
		MOV		pc, lr

	/*
	 * void *	__user_read_heap_top(void)
	 */
_heap_top:
		.word		__svc_stack_start

.global	__user_read_heap_top
__user_read_heap_top:
		LDR		r0, _heap_top
		MOV		pc, lr

	/*
	 * void *	__get_commands_table_start_address(void)
	 */
_commands_base:
		.word		0

.global __get_commands_table_start_address
__get_commands_table_start_address: 
		LDR		r0, _commands_base
		ADD		r0, #0x1000		/* Allocate 4K bytes for commands	*/
		MOV		pc, lr


	/*
	 * unsigned int	__get_cpu_id(void)
	 */
	
.global __get_cpu_id
__get_cpu_id: 
		LDR		r0, =PROCESSOR_ID_REG_BASE
		LDR		r0, [r0]
		AND		r0, r0, #0x03
		MOV		pc, lr


	/*
	 * __jump_to_target(void *target_address)
	 */
.global __jump_to_target
__jump_to_target: 
		MOV		pc, r0
	

	
	/*
	 * __reset_cpu(unsigned int cpu_id, void *address)
	 */

.global __reset_cpu
__reset_cpu:

		STMDB		sp!, {r0-r5}

		bl		flush_cache_all
#if 0
#if !defined(BERLIN_SOC_BG3CD)
		/*
		 * turn off vmeta SRAM
		 */
		ldr		r7, =0x0
		ldr		r6, =(MEMMAP_VPRO_REG_BASE + RA_vProRst_vproBiu)	//0xf8000008
		str		r7, [r6]
		ldr		r6, =(MEMMAP_VPRO_REG_BASE + RA_vProRst_vPro)		//0xf8000004
		str		r7, [r6]
#endif
#endif
		/*
		* disable MMU/PU stuff and caches
		*/
		MRC		p15, 0, r6, c1, c0, 0
		BIC		r6, r6, #0x00001800             /* clear bits 12:11 (--IZ)        */
		BIC		r6, r6, #0x00000005             /* clear bits 2:0 (-CM)       */
		MCR		p15, 0, r6, c1, c0, 0
		dsb
		isb
#if !defined(CONFIG_TRUSTZONE)
		//we use extern sstz image from now on. so delete it
		//bl		c_main
#endif
		LDMIA	sp!, {r0-r5}
		MOV		lr, pc
		MOV		pc, r0
