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

#define SCTLR_M_BIT		(1 << 0)
#define SCTLR_C_BIT		(1 << 2)
#define SCTLR_I_BIT		(1 << 12)

	
	/*
	 * __reset_cpu(unsigned int cpu_id, void *address)
	 */

.global __reset_cpu
__reset_cpu:

	mov	x5, #1
loop1:
	cmp	x5, #0
	b.eq	loop1

	stp	x2, x0, [sp, #-16]!

#ifdef CONFIG_DCACHE
#if 1
	mrs     x0, SCTLR_EL2               // reset value 0x00c52838
    bic     x0, x0, #0x00000001         // disable MMU
    //bic		x0, x0, #0x00001000         // disable I cache
    bic     x0, x0, #0x00000004         // disable D cache
    msr     SCTLR_EL2, x0
    ISB

	bl	flush_dcache_all

	bl	invalidate_tlb_all
#else
	//FIXME: don't know why the function doesn't work with same logic
	bl	disable_dcache
#endif
#endif

	ldp	x2, x0, [sp], #16

	mov x19, x0
	mov	x0, x2

	//Clear x1-x3 to x0, required by arm64 booting protocol
	mov	x1, xzr
	mov	x2, xzr
	mov	x3, xzr
	br	x19

panic:
	b		panic
