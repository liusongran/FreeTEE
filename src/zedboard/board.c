#include "board.h"
#include "sm.h"
/**
 * TrustZone-specific initializations
 *
 * @param  	
 *
 * @retval 	
 */
/**
 * 		// SCU config 
		io_write32(SCU_BASE + SCU_INV_SEC, SCU_INV_CTRL_INIT);
		io_write32(SCU_BASE + SCU_SAC, SCU_SAC_CTRL_INIT);
		io_write32(SCU_BASE + SCU_NSAC, SCU_NSAC_CTRL_INIT);

		//SCU enable
		io_setbits32(SCU_BASE + SCU_CTRL, 0x1);

		//NS Access control
		io_write32(SECURITY2_SDIO0, ACCESS_BITS_ALL);
		io_write32(SECURITY3_SDIO1, ACCESS_BITS_ALL);
		io_write32(SECURITY4_QSPI, ACCESS_BITS_ALL);
		io_write32(SECURITY6_APB_SLAVES, ACCESS_BITS_ALL);

		io_write32(SLCR_UNLOCK_MAGIC, SLCR_UNLOCK);

		io_write32(SLCR_TZ_DDR_RAM, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_DMA_NS, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_DMA_IRQ_NS, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_DMA_PERIPH_NS, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_GEM, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_SDIO, ACCESS_BITS_ALL);
		io_write32(SLCR_TZ_USB, ACCESS_BITS_ALL);

		io_write32(SLCR_LOCK, SLCR_LOCK_MAGIC);
 */

/**TODO:
 * lock/invalidate all lines: pl310 behaves as if disable
 *  bl	pl310_base
 *	bl	arm_cl2_lockallways
 *	bl	pl310_base
 *	bl	arm_cl2_cleaninvbyway
 */
uint32_tee board_init(void){

	/** Unlocking SLCR register */
	write32( (void *)SLCR_UNLOCK, SLCR_UNLOCK_KEY);

	/** Handling memory security */
	write32( (void *)TZ_OCM_RAM0, 0xffffffff);
	write32( (void *)TZ_OCM_RAM1, 0xffffffff);
	write32( (void *)TZ_OCM, 0xffffffff);
	/* Handling DDR memory security (first 5 segments NS)l */
	write32( (void *)TZ_DDR_RAM, 0x0000001f);
	printk("      * Memory security - OK  \n\t");

	/** Handling devices security */
	/* SDIO0 slave security (NS) */
	write32( (void *)SECURITY2_SDIO0, 0x1);
	/* SDIO1 slave security (NS) */
	write32( (void *)SECURITY3_SDIO1, 0x1);
	/* QSPI slave security (NS) */
	write32( (void *)SECURITY4_QSPI, 0x1);
	/* APB slave security (NS) */
	write32( (void *) SECURITY6_APBSL, 0x00007fff);
	/* DMA slave security (S) */
	write32( (void *)TZ_DMA_NS, 0x0);
	write32( (void *)TZ_DMA_IRQ_NS, 0x0);
	/* Ethernet security */
	write32( (void *)TZ_GEM, 0x3);
	/* FPGA AFI AXI ports TrustZone */
		//write32( (void *)SECURITY_APB, 0x3F);
	/* Handling more devices ... */
	printk("      * Devices security - OK  \n\t");

	/** Locking SLCR register */
	write32( (void *)SLCR_LOCK, SLCR_LOCK_KEY);

	return TRUE;
}

/**
 * Handling syscalls (SMCs)
 *
 * @param  	
 *
 * @retval 	
 */
uint32_tee board_handler(uint32_tee arg0, uint32_tee arg1, uint32_tee arg2, uint32_tee arg3)
{
	switch(arg0) {
		case (TA_CALL_SUCCESS):{
			//arg0 = read32((volatile void*)arg1);
			arg0 = TRUE;
			break;
		}
		case (TA_CALL_FAILED):{
			//write32( (volatile void*)arg1, arg2);
			arg0 = FALSE;
			break;
		}
		default:

			break;
	}

		return arg0;
}

