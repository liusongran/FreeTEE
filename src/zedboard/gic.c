
#include <gic.h>
#include <zynq_ttc.h>

/** Interrupt Distributor instance */
Interrupt_Distributor * int_dist = (Interrupt_Distributor *)(MPID_BASE);

/** CPU Interface instance - aliased for each CPU */
Cpu_Interface * const cpu_inter = (Cpu_Interface *)(MPIC_BASE);

/** Array of FIQ handlers */
tHandler* fiq_handlers[NO_OF_INTERRUPTS_IMPLEMENTED] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, ttc_interrupt_clear, ttc_interrupt_clear, ttc_interrupt_clear, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ttc_interrupt_clear,
	ttc_interrupt_clear, ttc_interrupt_clear, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL
};


void fiq_handler(uint32_tee interrupt){
	if (fiq_handlers[interrupt])
		fiq_handlers[interrupt]((void *) interrupt);
}




/**
 * Initialize the GIC Distributor
 *
 * @param
 *
 * @retval	True for success and FALSE in case ERROR
 */
uint32_tee interrupt_distributor_init(void){

	uint32_tee i;

	/** Check the ARM Legacy Identification Register Field values */
	if (int_dist->ICPIDR_0_3[0] != 0x90 || int_dist->ICPIDR_0_3[1] != 0xB3 ||
	    int_dist->ICPIDR_0_3[2] != 0x1B || int_dist->ICPIDR_0_3[3] != 0x00 ||
	    int_dist->ICPIDR_4_7[0] != 0x04 || int_dist->ICPIDR_4_7[1] != 0x00 ||
	    int_dist->ICPIDR_4_7[2] != 0x00 || int_dist->ICPIDR_4_7[3] != 0x00 ||
	    int_dist->ICCIDRx[0] != 0x0D || int_dist->ICCIDRx[1] != 0xF0 ||
	    int_dist->ICCIDRx[2] != 0x05 || int_dist->ICCIDRx[3] != 0xB1){

		return FALSE;
	}

	/** Disable the whole controller (S and NS) */
	int_dist->ICDDCR = 0x00000000;	//Distributor Control Register

	/** Disable all interrupts */
	for (i = 0; i < GIC_NUM_REGISTERS; i++){
		/* Clear-Enable */
		int_dist->ICDICERx[i] = 0xFFFFFFFF;
	}

	/** Clear all SPI interrupts */
	for (i = 1; i < GIC_NUM_REGISTERS; i++){
		/* Clear-Pending */
		int_dist->ICDICPRx[i] = 0xFFFFFFFF;
	}

	/** Reset SPI interrupt priorities */
	for (i = 8; i < GIC_PRIORITY_REGISTERS; i++){

		int_dist->ICDIPRx[i] = 0x00000000;
	}

	/** Reset interrupt targets */
	for (i = 0; i < GIC_TARGET_REGISTERS; i++){

		int_dist->ICDIPTRx[i] = 0x00000000;
	}

	/** Set interrupt configuration (level high sensitive, 1-N) */
	for (i = 2; i < (NO_OF_INTERRUPTS_IMPLEMENTED - 32) / 16; i++){

		int_dist->ICDICFRx[i] = 0x55555555;
	}

	/** Enable the interrupt controller (group0 and group1) */
	int_dist->ICDDCR = 0x00000003;

	return TRUE;
}


/**
 * Initialize the CPU Interface
 *
 * @param
 *
 * @retval	True for success and FALSE in case ERROR
 */
uint32_tee interrupt_interface_init(void){

	uint32_tee i;

	/** Clear the bits of the distributor which are CPU-specific */
	/* Clear-Pending */
	int_dist->ICDICPRx[0] = 0xFFFFFFFF;
	for (i = 0; i < 8; i++){
		/* SGI and PPI interrupt priorities */
		int_dist->ICDIPRx[i] = 0x00000000;
	}
	/* SGI and PPI set interrupt configuration */
	int_dist->ICDICFRx[0] = 0xAAAAAAAA;
	int_dist->ICDICFRx[1] = 0xAAAAAAAA;

	/** Disable CPU Interface */
	cpu_inter->ICCICR = 0x00000000;

	/** Allow interrupts with higher priority (i.e. lower number) than 0xF */
	cpu_inter->ICCPMR = 0x000000F0;

	/** All priority bits are compared for pre-emption */
	cpu_inter->ICCBPR = 0x00000003;

	/** Clear any pending interrupts */
	for( ; ; ){

		uint32_tee temp;
		/* interrupt_ack */
		temp = cpu_inter->ICCIAR;

		if((temp & GIC_ACK_INTID_MASK) == FAKE_INTERRUPT)
		{
			break;
		}
		/* end_of_interrupt */
		cpu_inter->ICCEOIR = temp;
	}

	/** Enable the CPU Interface */
	cpu_inter->ICCICR = 0x00000009;

	return TRUE;
}


/**
 * Interrupt enable/disable
 *
 * @param	interrupt = interrupt number
 *		enable = set (enable=1) or clear (enable=0)
 *
 * @retval
 */
void interrupt_enable(uint32_tee interrupt, uint32_tee enable){

	uint32_tee word;

	/** Calculate interrupt position */
	word = interrupt / 32;
	interrupt %= 32;
	interrupt = 1 << interrupt;

	if (enable){
		/* Set */
		int_dist->ICDISERx[word] = interrupt;
	}
	else{
		/* Clear */
		int_dist->ICDICERx[word] = interrupt;
	}
}

/**
 * Interrupt priority set
 *
 * @param	interrupt = interrupt number
 *		priority = priority value
 *
 * @retval	Return original priority
 */
uint32_tee interrupt_priority_set(uint32_tee interrupt, uint32_tee priority){

	uint32_tee word, bit_shift, temp, old_priority;

	priority &= 0xF;
	/* Get register of interrupt */
	word = interrupt / 4;
	bit_shift = (interrupt % 4) * 8 + 4;

	/* Get priority register */
	temp = old_priority = int_dist->ICDIPRx[word];
	/* Reset the priority for this interrupt to 0 */
	temp &= ~((uint32_tee)0xF << bit_shift);
	/* Set the new priority */
	temp |= (priority << bit_shift);
	int_dist->ICDIPRx[word] = temp;

	return ((old_priority >> bit_shift) & 0xF);
}

/**
 * Set interrupt target
 *
 * @param	interrupt = interrupt number
 *		cpu = target cpu id
 *		set = set value
 *
 * @retval
 */
void interrupt_target_set(uint32_tee interrupt, uint32_tee cpu, uint32_tee set){

	uint32_tee word, bit_shift, temp;

	/* Get register of interrupt */
	word = interrupt / 4;
	bit_shift = (interrupt % 4) * 8;
	cpu = (1 << cpu) << bit_shift;

	temp = int_dist->ICDIPTRx[word];
	if (set){
		/* Set */
		temp |= cpu;
	}
	else{
		/* Clear */
		temp &= ~cpu;
	}
	int_dist->ICDIPTRx[word] = temp;
}

/**
 * Clear interrupt
 *
 * @param	interrupt = interrupt number
 *		target =
 *
 * @retval
 */
void interrupt_clear(uint32_tee interrupt, uint32_tee target){

	/* Clear interrupt */
	cpu_inter->ICCEOIR = ( target << 10) | interrupt;
}

/**
 * Configure all interrupts' security
 *
 * @param
 *
 * @retval
 */
void interrupt_security_configall(void){

	uint32_tee num_regs;
	/* Configure all global interrupts as NS Interrupts */
	for(num_regs=1; num_regs < GIC_NUM_REGISTERS; num_regs++){
		int_dist->ICDISRx[num_regs] = 0xFFFFFFFF;
	}

}

/**
 * Config interrupt security
 *
 * @param	interrupt = interrupt number
 * 		security = security state (NS or S)
 *
 * @retval
 */
void interrupt_security_config(uint32_tee interrupt, IntSecurity_TypeDef security){

	uint32_tee word, temp;

	/* Calculate interrupt position in register */
	word = interrupt / 32;
	interrupt %= 32;
	interrupt = 1 << interrupt;

	temp = int_dist->ICDISRx[word];
	if (security == Int_NS){
		temp |= interrupt;
	}
	else{
		temp &= ~interrupt;
	}
	int_dist->ICDISRx[word] = temp;
}

/**
 * Get number of current interrupt
 *
 * @param
 *
 * @retval	Return the number of current interrupt
 */
uint32_tee interrupt_number_get() {

	return cpu_inter->ICCIAR;
}

/**
 * Enter critical section
 *
 * @param
 *
 * @retval
 */
void interrupt_critical_entry() {
	/* Set higher priority to 0x00 */
	cpu_inter->ICCPMR = 0x00;
}

/**
 * Exit critical section
 *
 * @param
 *
 * @retval
 */
void interrupt_critical_exit() {
	/* Set higher priority to 0xF0 */
	cpu_inter->ICCPMR = 0xF0;
}

/**
 * Generate IPI interrupt
 *
 * @param	id = IPI identifier
 *		target = target CPU
 *
 * @retval
 */
void interrupt_IPI_generate(uint32_tee id, uint32_tee target){
	/* Software Generated Interrupt Register */
   	int_dist->ICDSGIR = (target << IPI_TARGET_SHIFT) | id;
}
