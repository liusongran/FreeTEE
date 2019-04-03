#ifndef __ARMV7_CPU_VCPU_H
#define __ARMV7_CPU_VCPU_H

#include <types.h>

/** ARMv7-A CPU core registers */
struct core_regs {
	uint32_tee r0;
	uint32_tee r1;
	uint32_tee r2;
	uint32_tee r3;
	uint32_tee r4;
	uint32_tee r5;
	uint32_tee r6;
	uint32_tee r7;
	uint32_tee r8;
	uint32_tee r9;
	uint32_tee r10;
	uint32_tee r11;
	uint32_tee r12;
	uint32_tee spsr_mon;
	uint32_tee lr_mon;
	uint32_tee spsr_svc;
	uint32_tee r13_svc;
	uint32_tee lr_svc;
	uint32_tee spsr_sys;
	uint32_tee r13_sys;
	uint32_tee lr_sys;
	uint32_tee spsr_abt;
	uint32_tee r13_abt;
	uint32_tee lr_abt;
	uint32_tee spsr_undef;
	uint32_tee r13_undef;
	uint32_tee lr_undef;
	uint32_tee spsr_irq;
	uint32_tee r13_irq;
	uint32_tee lr_irq;
};

/** ARMv7-A CPU CP15 registers */
struct cp15_regs {
	uint32_tee c0_CSSELR;      /* Cache Size Selection Register */
	uint32_tee c1_SCTLR;       /* System Control Register */
	uint32_tee c1_ACTLR;       /* Auxilliary Control Register */
	uint32_tee c2_TTBR0;       /* Translation Table Base Register 0 */
	uint32_tee c2_TTBR1;       /* Translation Table Base Register 1 */
	uint32_tee c2_TTBCR;       /* Translation Table Base Register Control */
	uint32_tee c3_DACR;        /* Domain Access Control Register */
	uint32_tee c5_DFSR;        /* Data Fault Status Register */
	uint32_tee c5_IFSR;        /* Instruction Fault Status Register */
	uint32_tee c6_DFAR;        /* Data Fault Address Register */
	uint32_tee c6_IFAR;        /* Instruction Fault Address Register */
	uint32_tee c7_PAR;         /* Physical Address Register */
	uint32_tee c10_PRRR;       /* PRRR */
	uint32_tee c10_NMRR;       /* NMRR */
	uint32_tee c12_VBAR;       /* VBAR register */
	uint32_tee c13_FCSEIDR;    /* FCSE PID Register */
	uint32_tee c13_CONTEXTIDR; /* Context ID Register */
	uint32_tee c13_TPIDRURW;   /* User Read/Write Thread and Process ID */
	uint32_tee c13_TPIDRURO;   /* User Read-only Thread and Process ID */
	uint32_tee c13_TPIDRPRW;   /* Privileged only Thread and Process ID */
};

/** ARMv7-A CPU GIC registers */
struct gic_regs {
	uint32_tee gic_icdiser[4];
};

/** ARMv7-A CPU architecture registers */
struct vcpu_arch {
	struct core_regs vcpu_regs_core;
	struct cp15_regs vcpu_regs_cp15;
	struct gic_regs vcpu_regs_gic;
};

#endif /* __ARMV7_CPU_VCPU_H */
