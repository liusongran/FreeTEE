/*
 * @Author: elk.songran
 * @Date: 2019-03-25 21:25:48
 * @Last Modified by: elk.songran
 * @Last Modified time: 2019-03-27 00:49:06
 */

#include <ltzvisor.h>

/** Config NS_Guest struct */
extern struct nsguest_conf_entry nsguest_config;

/** NS_Guest context */
tzmachine NS_Guest __attribute__ ((aligned (4))) __attribute__ ((section (".bss")));

/**
 * LTZVisor main entry point
 *
 * @param
 *
 * @retval
 */
void ltzvisor_main(void){
	uint32_tee ret;

	/** Initialize LTZVisor */
	ret = ltzvisor_init();
	if(!ret){
		/* ERROR */
		printk("ERROR: LTZVisor Init ... \n\t");
		while(1);
	}

	/**TODO: write function to check the CPU mode now
	 * 1. cpu mode state
	 * 2. cpu id
	 * 3. ...
	*/
	//say_hello_world();

	/** TODO: tonight!! --- done
	 * Leaving Monitor and start running Sfreertos
	 * 1. Blink LED for a while to show we have already get into Sfreertos.
	 * 2. Init the TAs and put them into the Suspend Queue.
	 * 3. Print some message (blink LED for a while) to show that we are going back to Monitor mode to Boot Nfreertos
	 * 4. Call smc (YIELD) to switch back.
	*/
	printk(" -> Monitor mode: kicking off ... \n\t", ARCH);
	printk(" -> Monitor mode: starting TEE OS (Sfreertos) ... \n\t", ARCH);
	printk("----------------------------------------------------------\n\n\t");
	ltzvisor_kickoff();

	/** This point should never be reached */
	while (1);
}

void boot_nfreertos(void)
{
	uint32_tee ret;
	/** Create NS_Guest (Nfreertos) */
	printk(" -> Monitor mode: Nfreertos creation ... \n\t", ARCH);
	ret = ltzvisor_nsguest_create( &nsguest_config );
	if(!ret){
		/* ERROR */
		printk("Error: Nfreertos Create\n\t");
		while(1);
	}

	/**Teset settings for Nfreertos*/
	//1
	uint32_tee xCpuMode;
		__asm volatile ( "mrs %0, cpsr" : "=r" ( xCpuMode ) :: "memory" );
	//2
		//__asm volatile("msr cpsr_c,#(0x1f | 0x80 | 0x40)");
	//3
		write32( (void *)SLCR_UNLOCK, SLCR_UNLOCK_KEY);
		/* APB slave security (S) */
		write32( (void *) SECURITY6_APBSL, 0x00000000);
		/* APB slave security (NS) */
		write32( (void *) SECURITY6_APBSL, 0xffffffff);
		/** Locking SLCR register */
		write32( (void *)SLCR_LOCK, SLCR_LOCK_KEY);
	printk(" -> Monitor mode: Nfreertos creation, done! \n\t", ARCH);
	printk(" -> Monitor mode: Test settings for Nfreertos... \n\t", ARCH);
	printk("      ||- Current CPU mode 0x%08x. \r\n", xCpuMode);
	printk("      ||- Set all peripheral to non-secure world.  \n\t");

	printk(" -> Monitor mode: Going to Nfreertos... \n\t", ARCH);
	//return ret;

}
