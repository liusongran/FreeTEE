
#ifndef __LTZVISOR_H
#define __LTZVISOR_H

#include <types.h>
#include <printk.h>
#include <cpu_vcpu.h>
#include <ltzvisor.h>
#include <ltzvisor_api.h>
#include <board.h>

#define VERSION		"0.0.2"

#define PREEMPTION 	1
#define NO_PREEMPTION 	0

typedef struct {
	struct vcpu_arch core;
	char_tee name[30];
	uint32_tee id;
	uint32_tee booting;
}tzmachine;

extern tzmachine NS_Guest;

/**
 * LTZVisor main entry point
 *
 * @param
 *
 * @retval
 */
void ltzvisor_main(void);

void boot_nfreertos(void);

#endif /* __LTZVISOR_H */
