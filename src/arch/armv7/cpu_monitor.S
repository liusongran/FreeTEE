
 #include <cpu_defines.h>
 #include <asm-offsets.h>
 #include <ltzvisor_syscall_asm.h>
 #include "sm.h"
/**
 * SCR Non-Secure bit set (macro)
 *
 * @param
 *
 * @retval
 */
.macro SCR_NS_SET
	push 	{r0}
	mrc     p15, 0, r0, c1, c1, 0	@ Read SCR
	orr	r0, r0, #SCR_NS_BIT	@ Enable NS bit
	orr	r0, r0, #SCR_FIQ_BIT	@ Enable the route of FIQ in Monitor
	mcr     p15, 0, r0, c1, c1, 0	@ Write SCR
	pop 	{r0}
.endm

/**
 * SCR Non-Secure bit clear (macro)
 *
 * @param
 *
 * @retval
 */
.macro SCR_NS_CLEAR
	push 	{r0}
	mrc     p15, 0, r0, c1, c1, 0	@ Read SCR
	bic	r0, r0, #SCR_NS_BIT	@ Disable the NS bit
	bic	r0, r0, #SCR_FIQ_BIT	@ Disable the route of FIQs in Monitor
	mcr     p15, 0, r0, c1, c1, 0	@ Write SCR
	pop 	{r0}
.endm

/**
 * Get NS guest context's pointer (macro)
 *
 * @param
 *
 * @retval	NS_guest context pointer
 */
.macro GET_NS_WORLD_CONTEXT
	mrc p15, 0, r0, c13, c0, 4		@ Read PL1 only thread ID reg
	add r0, r0, #_ASM_ARCH_REGS_OFFSET	@ Add offset
.endm

/**
 * World context save (macro)
 *
 * @param	r0 = world context pointer
 *
 * @retval
 */
.macro WORLD_CONTEXT_SAVE
	mov	r4, r0
	pop	{r0 - r3}
	stmia	r4, {r0 - r3}
	mov	r3, r4
	add	r4, r4, #_ASM_R4_OFFSET
	mov	r0, r4
	pop	{r4, lr}
	stmia	r0!, {r4-r12}
	mrs	r1, spsr
	stmia	r0!, {r1, lr}
	/* Save SVC */
	cps #SUPERVISOR_MODE
	mrs	 r1, spsr
	stmia   r0!, {r1, r13, lr}
	/* Save SYS */
	cps #SYSTEM_MODE
	mrs	 r1, spsr
	stmia   r0!, {r1,r13, lr}
	/* Save ABORT */
	cps #ABORT_MODE
	mrs	 r1, spsr
	stmia   r0!, {r1, r13, lr}
	/* Save UNDEF */
	cps #UNDEFINED_MODE
	mrs	 r1, spsr
	stmia   r0!, {r1, r13, lr}
	/* Save IRQ */
	cps #IRQ_MODE
	mrs	 r1, spsr
	stmia   r0!, {r1, r13, lr}
	cps #MONITOR_MODE
.endm

/**
 * World context restore (macro)
 *
 * @param	r0 = world context pointer
 *
 * @retval
 */
.macro WORLD_CONTEXT_RESTORE
	mov	r4, r0
	ldmia 	r4!, {r0 - r3}
	push 	{r0, r1}
	mov 	r0, r4
	ldmia	r0!, {r4 - r12}
	ldmia	r0!, {r1, lr}
	msr	spsr_cxsf, r1
	/* Restore SVC */
	cps 	#SUPERVISOR_MODE
	ldmia   r0!, {r1, r13, lr}
	msr 	spsr_cxsf, r1
	/* Restore SYS */
	cps 	#SYSTEM_MODE
	ldmia   r0!, {r1,r13, lr}
	msr 	spsr_cxsf, r1
	/* Restore ABORT */
	cps 	#ABORT_MODE
	ldmia	r0!, {r1, r13, lr}
	msr	spsr_cxsf, r1
	/* Restore UNDEF */
	cps 	#UNDEFINED_MODE
	ldmia	r0!, {r1, r13, lr}
	msr	spsr_cxsf, r1
	/* Restore IRQ */
	cps 	#IRQ_MODE
	ldmia	r0!, {r1, r13, lr}
	msr	spsr_cxsf, r1
	cps 	#MONITOR_MODE
	pop	{r0, r1}
.endm

/**
 * Exception return emulation (macro)
 *
 * @param
 *
 * @retval
 */
.macro exception_return_arg
	ldmfd	sp!, {r1}
	msr	spsr, r1
	push	{r1}@@
	mov	r1, #0
	dsb
	pop	{r1}
	ldmfd	sp!, {r1-r12, pc}^
.endm

/**
 * C function call prolog (macro)
 *
 * @param
 *
 * @retval
 */
.macro call_c_function_arg func_name
	stmfd   sp!, {r1-r12, lr}
	mrs	r4, spsr
	stmfd   sp!, {r4}
	bl	\func_name
.endm


/**
 * Monitor vector table
 *
 * @param
 *
 * @retval
 */
.align 8
.section .vectors
.global _monitor_vector_table
	/** Monitor Vector Table definition */
_monitor_vector_table:
@	b	.		@ Not available on MON 	->reset
	ldr	pc, __mon_smc_handler
	b	.							@ Not available on MON	->undef
	ldr	pc, __mon_smc_handler				@	->mon
	ldr	pc, __mon_prefetch_handler			@	->prefetch
	ldr	pc, __mon_abort_handler				@	->abort
	b	.							@ Reserved for HYP
	ldr	pc, __mon_irq_handler		@ Should never come here
	ldr	pc, __mon_fiq_handler				@	->fiq

__mon_smc_handler:
	.word	_mon_smc_handler
__mon_prefetch_handler:
	.word	mon_prefetch_handler
__mon_abort_handler:
	.word	mon_abort_handler
__mon_irq_handler:
	.word	mon_irq_handler
__mon_fiq_handler:
	.word 	_mon_fiq_handler


/**
 * Monitor SMC handler
 *
 * @param
 *
 * @retval
 */
.globl _mon_smc_handler
	/** Monitor - SMC handler */
_mon_smc_handler:
	srsdb	sp!, #CPSR_MODE_MON		@FIXME:store return state onto a stack
	@push {r0-r7}					@param a0-a7
	stmfd   sp!, {r1-r12, lr}
	mrs	r6, spsr
	stmfd   sp!, {r6}

	/* Find out if we're doing an secure or non-secure entry */
	mrc	p15, 0, r7, c1, c1, 0			@ Read SCR -> r7
	tst	r7, #SCR_NS
	bne	.smc_from_nsec

.smc_from_sec:
	/**
	 * As we're coming from secure world (NS bit clear).
	 * 	Sfreertos init done or TA exe done ?
	*/
	ldr r7, =INIT_TEE_SUCCESS
	cmp r0, r7	@Sfreertos init success?

	bne	.ta_return
@	bl	sfreertos_smc
@	cmp r0, #1
@	beq _mon_scheduler			@schedule Nfreertos
@	bx	lr
	bl	boot_nfreertos
	b	_mon_scheduler
	@ never reach here!
	b	.


.ta_return:
	/**
	 * As we are coming from secure world after TA YIELD().
	 */
	bl	ta_smc
	bl	_mon_scheduler
	bx	lr@???
	@ never reach here!
	b	.
	/**TODO: refer _mon_scheduler!!!
	 * As 'TA exe done' makes this call,
	 *		1. return TA's result back
	 *		2. restore Nfreertos context
	 */
	@cmn	r0, #0
	@blt	board_smc
	@cmp 	r0, #LTZVISOR_SCHED_SYSCALL
	@beq	_mon_scheduler
	@bx	lr@???

.smc_from_nsec:
	/**
	 * As we're coming from non-secure world (NS bit set).
	 */
	bic	r7, r7, #(SCR_NS | SCR_FIQ) 	/* Clear NS and FIQ bit in SCR */
	mcr	p15, 0, r7, c1, c1, 0			@write_scr r7
	isb

	/**TODO:????????
	 * fast SMC or std SMC ?
	 * FIXME: DOUBLE CHECK. As we donot change values in r0, we can safely
	 * pass it to sm_from_nsec() as a parameter. The other way is
	 * to read values from stack.
	*/
	bl	sm_from_nsec
	cmp	r0, #0
	//beq	.sm_ret_to_nsec

	/*
	 * Continue into secure world
	 */
	@add	sp, sp, #(SM_CTX_SEC + SM_SEC_CTX_R0)

.sm_exit:
	pop	{r0-r7}
	rfefd	sp!




/**
 * Monitor FIQ handler
 *
 * @param
 *
 * @retval
 */
.globl _mon_fiq_handler
	/** Monitor - FIQ handler */
_mon_fiq_handler:
	sub	lr, lr, #4			@ Adjust LR
	push	{r4, lr}
	push	{r0 - r3}
	/* ACK FIQ */
	mrc 	p15,4,r0,c15,c0,0		@ Read Periph Base Addr
	add 	r0,r0, #0x0100			@ Add GIC interface offset (A9)
	ldr 	r1,[r0,#_ASM_GIC_INT_ID]	@ Get interrupt ID
@@@	ldr 	r2,=interrupt
	str 	r1,[r2]
	ldr 	r0,[r0,#0x0C]			@ ACK FIQ
	/* Save NS context */
	SCR_NS_CLEAR				@ Disable NS bit and route FIQs in mon
	GET_NS_WORLD_CONTEXT
	WORLD_CONTEXT_SAVE
	/* Handling FIQ */
@@@	ldr 	r2,=interrupt
	ldr 	r0,[r2]
	//bl 	ttc_interrupt_clear		/* FIXME - supposing always ttc_isr */
	bl 	fiq_handler			/* FIXME - supposing always ttc_isr */
@@@	ldr 	r2,=interrupt
	ldr 	r0,[r2]
	ldr 	r1,=0x00
	bl 	interrupt_clear
	/* Restore FIQ state */
	pop 	{r1-r2}				@ Pop SPSR_FIQ and LR_FIQ
	mov 	r0, sp				@ Update stack pointer
	cps 	#FIQ_MODE
	msr 	spsr,r1
	mov 	lr,r2
	ldmfd 	r0, {r8-r13}			@ Restore all banked FIQ regs (R8-R14)
	add 	r0,r0,#(6*4)			@ Update stack pointer
	/* Get back to MON */
	cps 	#MONITOR_MODE
	mov 	sp,r0
	/* Go S world */
	pop 	{r0-r1}
	msr 	spsr,r0
	mov 	lr,r1
	ldmfd 	sp, {r0-r14}^
	add 	sp,sp,#(15*4)
	movs	pc,lr				@ Return from exception (MON -> S_SVC)

/**
 * TA return
 *
 * @param
 *
 * @retval
 */
ta_smc:
	call_c_function_arg board_handler
	exception_return_arg

/**
 * Init Sfreetos done and go to boot Nfreertos.
 *
 * @param
 *
 * @retval
 */
@sfreertos_smc:
@	call_c_function_arg boot_nfreertos
	@exception_return_arg

/**
 * Monitor scheduler - schedule NS_guest
 *
 * @param
 *
 * @retval
 */
.globl _mon_scheduler
_mon_scheduler:
	stmfd 	sp,{r0-r14}^			@ Push SYS regs to monitor stack
	sub 	sp, sp, #(15*4)      		@ Point stack to the top
	mrs 	r0,spsr				@ Read the SPSR
	stmfd	sp, {r0,lr}			@ Push SPSR_mon & lr_mon
	sub 	sp, sp, #(2*4)      		@ Update stack pointer
	/* Save FIQ state */
	mov	r0, sp				@ Save pointer to monitor stack
	cps #FIQ_MODE
	stmfd 	r0,{r8-r13}			@ Save all banked FIQ regs (R8-R14)
	sub 	r0, r0, #(6*4)      		@ Update stack pointer
	mrs 	r1,spsr				@ Read the SPSR_FIQ
	stmfd	r0, {r1,lr}			@ Push SPSR_FIQ and lr_FIQ
	sub 	r0, r0, #(2*4)      		@ Update stack pointer
	/* Get back to MON */
	cps #MONITOR_MODE
	mov 	sp, r0
	/* Restore NS context */
	GET_NS_WORLD_CONTEXT
	WORLD_CONTEXT_RESTORE
	/* Set NS bit and go NS world */
	SCR_NS_SET
	isb
	dsb
	movs    pc, lr				@ Return from exception (MON -> NS_SVC)

