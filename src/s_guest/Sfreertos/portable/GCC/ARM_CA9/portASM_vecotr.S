/*
 * @Author: elk.songran
 * @Date: 2019-03-23 20:53:46
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-24 15:20:50
 */

@ checking  _boot fun                       --- done (stack setup)---done in linker.ld
@           FreeRTOS_Undefined              --- done
@           _swi                            --- done
@           FreeRTOS_PrefetchAbortHandler   --- done
@           FreeRTOS_DataAbortHandler       --- done
@           _irq                            --- done
@           FreeRTOS_FIQHandler             --- done

.org 0
.text
.arm

.global _secure_freertos_vector_table
@.global _vector_table

@.global FIQInterrupt
@.global DataAbortInterrupt
@.global PrefetchAbortInterrupt
.global vPortInstallFreeRTOSVectorTable

.extern FreeRTOS_IRQ_Handler
.extern FreeRTOS_SWI_Handler

.section .vectors
_secure_freertos_vector_table:
	b		.
	b	  	FreeRTOS_Undefined
	ldr   	pc, _swi      @portASM.S		@????
	B	  	FreeRTOS_PrefetchAbortHandler
	B	  	FreeRTOS_DataAbortHandler
	NOP	  	/* Placeholder for address exception vector*/
	LDR   	PC, _irq      @portASM.S
	B	  	FreeRTOS_FIQHandler

_irq:   .word FreeRTOS_IRQ_Handler
_swi:   .word FreeRTOS_SWI_Handler


.align 4
FreeRTOS_FIQHandler:			/* FIQ vector handler */
    b       .

.align 4
FreeRTOS_Undefined:				/* Undefined handler */
	b		.

.align 4
FreeRTOS_DataAbortHandler:		/* Data Abort handler */
	b       .

.align 4
FreeRTOS_PrefetchAbortHandler:	/* Prefetch Abort handler */
	b       .

.align 4
.type vPortInstallFreeRTOSVectorTable, %function
vPortInstallFreeRTOSVectorTable:
	/* Set VBAR to the vector table that contains the FreeRTOS handlers. */
	ldr	r0, =_secure_freertos_vector_table
	mcr	p15, 0, r0, c12, c0, 0
	dsb
	isb
	bx lr
.end
