
#include <cpu_defines.h>
#include <asm-offsets.h>
#include <platform_asm.h>

.text
.global __stack
.global __irq_stack
.global __supervisor_stack
.global __abort_stack
.global __fiq_stack
.global __undef_stack
.global __mon_stack
.global _reset_handler

/* Stack Pointer locations for boot code */
.set Undef_stack,	__undef_stack
.set FIQ_stack,		__fiq_stack
.set Abort_stack,	__abort_stack
.set SPV_stack,		__supervisor_stack
.set IRQ_stack,		__irq_stack
.set MON_stack,		__mon_stack
.set SYS_stack,		__stack

/* MMU need*/
.globl MMUTable
.set TblBase ,		MMUTable
/* workaround for simulation not working when L1 D and I caches,MMU and  L2 cache enabled - DT568997 */
@.if SIM_MODE == 1
@.set CRValMmuCac,	0b00000000000000	/* Disable IDC, and MMU */
@.else
.set CRValMmuCac,	0b01000000000101	/* Enable IDC, and MMU */
@.endif


_reset_handler:

	/** Run CPU0 and stuck other CPUs */
	bl	get_cpu_id
	cmp	r0, #0		@ CPU0?
	bne	__stuck_loop

	/*
	 * 32bit entry is expected to execute Supervisor mode,
	 * some bootloader may enter in Supervisor or Monitor
	 */
	cps	#SUPERVISOR_MODE

/**
 * Cortex A9 early configuration:
 *
 * - use registers R0-R3;
 * - no stack usage;
 * - LR store return address;
 * - Trap CPU in case of error.
 */
/**plat_cpu_reset_early: */
__setup_CPU:
	/**TODO: FIXME!
	 * Configure SCTLR(System Control Register):
	 * SCTLR = 0x00004000
	 * - Round-Robin replac. for icache, btac, i/duTLB (bit14: RoundRobin)
	 */
	mrc	p15, 0, r1, c1, c0, 0			@ Read SCTLR
	bic	r1, r1, #0x10000000			@ Clear TEX bit
	bic	r1, r1, #0x00002000			@ Clear Vectors bit /*Normal exception vectors base address 0x00000000*/
	mcr	p15, 0, r1, c1, c0, 0			@ Write SCTLR

	/**TODO: FIXME!
	 * Configure NSACR(Non-secure Access Control Register):
	 * NSACR = 0x00020C00
	 * - NSec cannot change ACTRL.SMP (NS_SMP bit18=0)
	 * - Nsec can lockdown TLB (TL bit17=1)
	 * - NSec cannot access PLE (PLE bit16=0)
	 * - NSec can use SIMD/VFP (CP10/CP11) (bit15:14=2b00, bit11:10=2b11)
	 */
	mrc	p15, 0, r1, c1, c1, 2			@ Read NSACR
	ldr	r2, =NSACR_REG_VAL			/*set bit 18,17,11,10*/
	orr	r1, r1, r2				@ Mask r1 with r2
	mcr	p15, 0, r1, c1, c1, 2			@ Write NSACR

	/**TODO: FIXME!
	 * Configure SCR(Secure Configuration Register):
	 * - Disallow NSec to mask FIQ [bit4: FW=0]
	 * - Allow NSec to manage Imprecise Abort [bit5: AW=1]
	 * - Imprecise Abort trapped to Abort Mode [bit3: EA=0]
	 * - In Sec world, FIQ trapped to FIQ Mode [bit2: FIQ=0]
	 * - IRQ always trapped to IRQ Mode [bit1: IRQ=0]
	 * - Secure World [bit0: NS=0]
	 */
	mrc	p15, 0, r1, c1, c1, 0			@ Read SCR
	bic  	r1, r1, #SCR_FIQ_BIT			@ Clear FIQ bit (disable route FIQs monitor)
	mcr	p15, 0, r1, c1, c1, 0			@ Write SCR

	/**TODO: FIXME!
	 * Configure ACTLR (Auxiliary Control register):
	 * ACTRL = 0x00000041
	 * - core always in full SMP (FW bit0=1, SMP bit6=1)
	 * - L2 write full line of zero disabled (bit3=0)
	 *   (keep WFLZ low. Will be set once outer L2 is ready)
	 */
	mrc	p15, 0, r0, c1, c0, 1			@ Read ACTLR
	ldr  	r0, =0x00				@ Clear registers
	mcr	p15, 0, r0, c1, c0, 1			@ Write ACTLR

	/**TODO: FIXME!
	 * Configure PCR:
	 * PCR = 0x00000001
	 * - no change latency, enable clk gating
	 */
	@mov_imm r0, 0x00000001
	@write_pcr r0

	/**TODO: CHECK!!
	 * Setup some core configuration in CP15 SCTLR:
	 * Setup required by current implementation of the OP-TEE core:
	 * - Disable data and instruction cache.
	 * - MMU is expected off and exceptions trapped in ARM mode.
	 * - Enable or disable alignment checks upon platform configuration.
	 * - Optinally enable write-implies-execute-never.
	 * - Optinally enable round robin strategy for cache replacement.
	 */
	@set_sctlr
	mrc	p15, 0, r1, c1, c0, 0			@ Read SCTLR
	bic	r0, r0, #(SCTLR_M | SCTLR_C)
	bic	r0, r0, #SCTLR_I
	bic	r0, r0, #SCTLR_TE
	bic	r0, r0, #SCTLR_A
	orr	r0, r0, #(SCTLR_WXN | SCTLR_UWXN)
	orr	r0, r0, #SCTLR_RR
	mcr	p15, 0, r1, c1, c0, 0			@ Write SCTLR

	isb

	/** Set secure vector table (VBAR) */
	@ Init
@	ldr	r0, =_secure_vector_table		@ Read the Secure Vector Table's Base Address
@	mcr	p15, 0, r0, c12, c0, 0			@ Write VBAR

	/** Set monitor vector table (MVBAR) */
	ldr	r0, =_monitor_vector_table		@ Read the Monitor Vector Table's Base Address
	mcr	p15, 0, r0, c12, c0, 1			@ Write MVBAR


	/** Setup Stacks for all CPU modes */
__setup_stacks:

	/* Trap into IRQ mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the irq stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x12			/* IRQ mode */
	msr	cpsr, r2
	ldr	r13,=IRQ_stack			/* IRQ stack pointer */
	bic r2, r2, #(0x1 << 9)   	/* Set EE bit to little-endian */
	msr spsr_fsxc,r2
	/* Trap into SVC mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the supervisor stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x13			/* supervisor mode */
	msr	cpsr, r2
	ldr	r13,=SPV_stack			/* Supervisor stack pointer */
	bic r2, r2, #(0x1 << 9)     /* Set EE bit to little-endian */
	msr spsr_fsxc,r2
	/* Trap into ABT mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the Abort  stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x17			/* Abort mode */
	msr	cpsr, r2
	ldr	r13,=Abort_stack		/* Abort stack pointer */
	bic r2, r2, #(0x1 << 9)     /* Set EE bit to little-endian */
	msr spsr_fsxc,r2
	/* Trap into FIQ mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the FIQ stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x11			/* FIQ mode */
	msr	cpsr, r2
	ldr	r13,=FIQ_stack			/* FIQ stack pointer */
	bic r2, r2, #(0x1 << 9)    	/* Set EE bit to little-endian */
	msr spsr_fsxc,r2
	/* Trap into UDF mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the Undefine stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x1b			/* Undefine mode */
	msr	cpsr, r2
	ldr	r13,=Undef_stack		/* Undefine stack pointer */
	bic r2, r2, #(0x1 << 9)     /* Set EE bit to little-endian */
	msr spsr_fsxc,r2
	/* Trap into SYS mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the system stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x1F			/* SYS mode */
	msr	cpsr, r2
	ldr	r13,=SYS_stack			/* SYS stack pointer */
	/* Trap into MON mode */
	mrs	r0, cpsr				/* get the current PSR */
	mvn	r1, #0x1f				/* set up the Monitor stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x16			/* Monitor mode */
	msr	cpsr, r2
	ldr	r13,=MON_stack			/* Monitor stack pointer */
	bic r2, r2, #(0x1 << 9)     /* Set EE bit to little-endian */
	msr spsr_fsxc,r2

	/** Handling cache and MMU subsystems */
__init_vmemory:
	/* Disable MMU -OK*/
	mrc 	p15, 0, r1, c1, c0, 0			@ Read SCTLR register
	bic 	r1, r1, #SCTLR_MMU_BIT			@ Clear M bit (disable MMU)
	mcr 	p15, 0, r1, c1, c0, 0			@ Write SCTLR register
	/* Disable L1 Caches -OK*/
	mrc 	p15, 0, r1, c1, c0, 0			@ Read SCTLR register
	bic 	r1, r1, #SCTLR_DCACHE_BIT		@ Clear C bit (disable D-Cache)
	bic 	r1, r1, #SCTLR_ICACHE_BIT		@ Clear I bit (disable I-Cache)
	mcr 	p15, 0, r1, c1, c0, 0			@ Write SCTLR register
	/* Invalidate icache -OK*/
	mov 	r1, #0
	mcr 	p15, 0, r1, c7, c5, 0			@ Instruction Cache Invalidate All
	/* Invalidate Data caches */
	bl		invalidate_dcache		@ Invalidate data cache
	/* Invalidate Branch Predictor arrays -OK*/
	mov 	r1, #0
	mcr		p15, 0, r1, c7, c5, 6			@ Invalidate BP
	/* Invalidate TLBs -OK*/
	mov 	r1, #0
	mcr 	p15, 0, r1, c8, c7, 0			@ Invalidate entire unified TLB Inner Shareable

	/*set scu enable bit in scu*/
	ldr	r7, =0xf8f00000
	ldr	r0, [r7]
	orr	r0, r0, #0x1
	str	r0, [r7]

	/* enable MMU and cache */
	ldr	r0,=TblBase					/* Load MMU translation table base */
	orr	r0, r0, #0x5B				/* Outer-cacheable, WB */
	mcr	15, 0, r0, c2, c0, 0		/* TTB0 */

	mvn	r0,#0						/* Load MMU domains -- all ones=manager */
	mcr	p15,0,r0,c3,c0,0

	/* Enable mmu, icahce and dcache */
	ldr	r0,=CRValMmuCac
	mcr	p15,0,r0,c1,c0,0			/* Enable cache and MMU */
	dsb								/* dsb	allow the MMU to start up */
	isb								/* isb	flush prefetch buffer */

	/* Write to ACTLR */
	mrc	p15, 0, r0, c1, c0, 1		/* Read ACTLR*/
	orr	r0, r0, #(0x01 << 6)		/* set SMP bit */
	orr	r0, r0, #(0x01 )		/* Cache/TLB maintenance broadcast */
	mcr	p15, 0, r0, c1, c0, 1		/* Write ACTLR*/

	mrs	r0, cpsr			/* get the current PSR */
	bic	r0, r0, #0x100			/* enable asynchronous abort exception */
	msr	cpsr_xsf, r0


	/** Handling VFP and NEON */
__init_vfp:
	/* TODO:FIX IT */


	/** Initializing C environment, do pre-start for TEE */
__init_c_env:
	b	_tee_pre_start	@TODO:fixed


/**
 * Stuck other CPUs than CPU0
 *
 * @param
 *
 * @retval
 */
__stuck_loop:
	b	__stuck_loop

/**
 * Get CPU id
 *
 * @param
 *
 * @retval 	r0 - CPU id
 */
.global get_cpu_id
.func get_cpu_id
  @ uint32_t get_cpu_id(void)
get_cpu_id:
	mrc	p15, 0, r0, c0, c0, 5
	and	r0, r0, #0x03
	bx	lr
.endfunc

/*
 *************************************************************************
 *
 * invalidate_dcache - invalidate the entire d-cache by set/way
 *
 * Note: for Cortex-A9, there is no cp instruction for invalidating
 * the whole D-cache. Need to invalidate each line.
 *
 *************************************************************************
 */
invalidate_dcache:
	mrc	p15, 1, r0, c0, c0, 1		/* read CLIDR */
	ands	r3, r0, #0x7000000
	mov	r3, r3, lsr #23			/* cache level value (naturally aligned) */
	beq	finished
	mov	r10, #0				/* start with level 0 */
loop1:
	add	r2, r10, r10, lsr #1		/* work out 3xcachelevel */
	mov	r1, r0, lsr r2			/* bottom 3 bits are the Cache type for this level */
	and	r1, r1, #7			/* get those 3 bits alone */
	cmp	r1, #2
	blt	skip				/* no cache or only instruction cache at this level */
	mcr	p15, 2, r10, c0, c0, 0		/* write the Cache Size selection register */
	isb					/* isb to sync the change to the CacheSizeID reg */
	mrc	p15, 1, r1, c0, c0, 0		/* reads current Cache Size ID register */
	and	r2, r1, #7			/* extract the line length field */
	add	r2, r2, #4			/* add 4 for the line length offset (log2 16 bytes) */
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		/* r4 is the max number on the way size (right aligned) */
	clz	r5, r4				/* r5 is the bit position of the way size increment */
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		/* r7 is the max number of the index size (right aligned) */
loop2:
	mov	r9, r4				/* r9 working copy of the max way size (right aligned) */
loop3:
	orr	r11, r10, r9, lsl r5		/* factor in the way number and cache number into r11 */
	orr	r11, r11, r7, lsl r2		/* factor in the index number */
	mcr	p15, 0, r11, c7, c6, 2		/* invalidate by set/way */
	subs	r9, r9, #1			/* decrement the way number */
	bge	loop3
	subs	r7, r7, #1			/* decrement the index */
	bge	loop2
skip:
	add	r10, r10, #2			/* increment the cache number */
	cmp	r3, r10
	bgt	loop1
finished:
	mov	r10, #0				/* swith back to cache level 0 */
	mcr	p15, 2, r10, c0, c0, 0		/* select current cache level in cssr */
	dsb
	isb

	bx	lr

.end
