
#ifndef __LTZVISOR_HW_H
#define __LTZVISOR_HW_H

#include <types.h>
#include <printk.h>
#include <gic.h>
#include <ltzvisor.h>
#include <board.h>
#include <zynq_ttc.h>

#define ARCH		"ARMv7-A"
#define CPU		"Cortex-A9"
#define PLATFORM	"Zynq-7000"
/**
#define LTZVISOR_MON_EXIT() {\
	asm volatile("msr cpsr_c,#(0x13 | 0x80 | 0x40)");\
	asm volatile("ldr r1,=__supervisor_stack");\
	asm volatile("add r1, r1, r0, lsl #12");\
	asm volatile("add sp, r1, #4096");\
}
*/
/**The fisrt exit from mon-->secure*/
#define LTZVISOR_MON_EXIT() {\
	asm volatile("msr cpsr_c,#(0x1f | 0x80 | 0x40)");\
}

/**TODO: TEE_MON_EXIT_WITH_ARG */
/**
#define LTZVISOR_MON_EXIT() {\
	asm volatile("msr cpsr_c,#(0x1f | 0x80 | 0x40)");\
	asm volatile("ldr r1,=__stack");\
	asm volatile("add r1, r1, r0, lsl #12");\
	asm volatile("add sp, r1, #8192");\
}
 */

/**
 * LTZVisor hardware initialization
 *
 * @param  
 *
 * @retval Return TRUE if success or False if not
 */
uint32_tee ltzvisor_hw_init(void);

#endif /* __LTZVISOR_HW_H */
