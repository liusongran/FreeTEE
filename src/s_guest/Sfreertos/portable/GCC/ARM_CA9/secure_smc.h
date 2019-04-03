/*
 * @Author: elk.songran 
 * @Date: 2019-03-26 15:56:10 
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-26 16:27:24
 */

#ifndef SECURE_SMC_H
#define SECURE_SMC_H
/**NOTE: different with taskYIELD() in FreeRTOS. */
#define YIELD(){ \
	asm volatile("ldr r0, =0x20190530");\
	asm volatile("smc #0");\
}

/**TODO: YIELD_WITH_ARG() need or not??*/

#endif /**SECURE_SMC_H */