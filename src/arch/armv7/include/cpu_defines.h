#ifndef __ARMV7_CPU_DEFINES_H__
#define __ARMV7_CPU_DEFINES_H__
/**Basic Operation --- by sr */
#define BIT32(nr)		(1 << (nr))

/** CPU Operation Modes */
#define  USER_MODE		0x10
#define  FIQ_MODE		0x11
#define  IRQ_MODE		0x12
#define  SUPERVISOR_MODE	0x13
#define  MONITOR_MODE		0x16
#define  ABORT_MODE   		0x17
#define  HYPERVISOR_MODE   	0x1A	// Not Used
#define  UNDEFINED_MODE		0x1B
#define  SYSTEM_MODE		0x1F

#define CPSR_MODE_USR	0x10
#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_MON	0x16
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f



/** CPSR FIQ & IRQ Bits*/
#define	FIQ_BIT          	0x40
#define	IRQ_BIT          	0x80

#define STACK_SIZE 8192
#define STACK_SIZE_SHIFT 12

/** SCR Bits --- by sr*/
#define  SCR_NS_BIT     	0x1
#define  SCR_FIQ_BIT        	0x4
#define  SCR_IRQ_BIT        	0x2
#define  SCR_HCR_BIT		0x100

#define SCR_NS		BIT32(0)
#define SCR_IRQ		BIT32(1)
#define SCR_FIQ		BIT32(2)
#define SCR_EA		BIT32(3)
#define SCR_FW		BIT32(4)
#define SCR_AW		BIT32(5)
#define SCR_NET		BIT32(6)
#define SCR_SCD		BIT32(7)
#define SCR_HCE		BIT32(8)
#define SCR_SIF		BIT32(9)

/** SCTLR Bits --- by sr*/
#define	SCTLR_MMU_BIT		0x1
#define SCTLR_DCACHE_BIT	(1 << 2)
#define SCTLR_BRANCH_PRED_BIT	(1 << 11)
#define SCTLR_ICACHE_BIT	(1 << 12)

#define SCTLR_M		BIT32(0)
#define SCTLR_A		BIT32(1)
#define SCTLR_C		BIT32(2)
#define SCTLR_CP15BEN	BIT32(5)
#define SCTLR_SW	BIT32(10)
#define SCTLR_Z		BIT32(11)
#define SCTLR_I		BIT32(12)
#define SCTLR_V		BIT32(13)
#define SCTLR_RR	BIT32(14)
#define SCTLR_HA	BIT32(17)
#define SCTLR_WXN	BIT32(19)
#define SCTLR_UWXN	BIT32(20)
#define SCTLR_FI	BIT32(21)
#define SCTLR_VE	BIT32(24)
#define SCTLR_EE	BIT32(25)
#define SCTLR_NMFI	BIT32(26)
#define SCTLR_TRE	BIT32(28)
#define SCTLR_AFE	BIT32(29)
#define SCTLR_TE	BIT32(30)

/**TODO: */
#define AUXREG_SMP         	0x41
#define AUXREG_UP		0x0

#endif /* __ARMV7_CPU_DEFINES_H__ */
