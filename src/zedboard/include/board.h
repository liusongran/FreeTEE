
#ifndef __BOARD_H
#define __BOARD_H

#include <types.h>
#include <io.h>
#include <ltzvisor.h>
#include <ltzvisor_syscall_asm.h>
#include <zynq_ttc.h>

/** TrustZone on Xilinx Zynq-7000 (UG1019) */

#define SECURITY_MOD2		0xE0200000
#define SECURITY2_SDIO0         (SECURITY_MOD2 + 0x8)
#define SECURITY3_SDIO1         (SECURITY_MOD2 + 0xC)
#define SECURITY4_QSPI          (SECURITY_MOD2 + 0x10)
#define SECURITY5_MIOU          (SECURITY_MOD2 + 0x14)
#define SECURITY6_APBSL         (SECURITY_MOD2 + 0x18)
#define SECURITY7_SMC           (SECURITY_MOD2 + 0x1C)

#define SLCR_BASE             	0xF8000000
#define SLCR_LOCK_KEY           0x767B
#define SLCR_UNLOCK_KEY         0xDF0D
#define SLCR_LOCK               (SLCR_BASE + 0x4)
#define SLCR_UNLOCK             (SLCR_BASE + 0x8)
#define DMAC_RST_CTRL           (SLCR_BASE + 0x20C)
#define TZ_OCM_RAM0             (SLCR_BASE + 0x400)
#define TZ_OCM_RAM1             (SLCR_BASE + 0x404)
#define TZ_OCM             	(SLCR_BASE + 0x408)
#define TZ_DDR_RAM              (SLCR_BASE + 0x430)
#define TZ_DMA_NS               (SLCR_BASE + 0x440)
#define TZ_DMA_IRQ_NS           (SLCR_BASE + 0x444)
#define TZ_DMA_PERIPH_NS        (SLCR_BASE + 0x448)
#define TZ_GEM			(SLCR_BASE + 0x450)
#define TZ_SDIO             	(SLCR_BASE + 0x454)
#define TZ_USB              	(SLCR_BASE + 0x458)
#define TZ_FPGA_M          	(SLCR_BASE + 0x484)
#define TZ_FPGA_AFI          	(SLCR_BASE + 0x488)

#define SECURITY_MOD3		0xF8900000
#define SECURITY_FSSW_S0        (SECURITY_MOD3 + 0x1C)
#define SECURITY_FSSW_S1        (SECURITY_MOD3 + 0x20)
#define SECURITY_APB            (SECURITY_MOD3 + 0x28)

/**
 * TrustZone-specific initializations
 *
 * @param  	
 *
 * @retval 	
 */
uint32_tee board_init(void);

/**
 * Handling syscalls (SMCs)
 *
 * @param  	
 *
 * @retval 	
 */
uint32_tee board_handler(uint32_tee arg0, uint32_tee arg1, uint32_tee arg2, uint32_tee arg3);

#endif /* __BOARD_H */
