/*
 * LTZVisor, a Lightweight TrustZone-assisted Hypervisor
 *
 * Copyright (c) TZVisor Project (www.tzvisor.org), 2017-
 *
 * Authors:
 *  Sandro Pinto <sandro@tzvisor.org>
 *  Jorge Pereira <jorgepereira89@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 *
 * [ltzvisor_hw.c]
 *
 * This file contains the LTZVisor hardware-specific initialization.
 * 
 * (#) $id: ltzvisor_hw.c 10-06-2015 s_pinto & j_pereira $
 * (#) $id: ltzvisor_hw.c 17-09-2017 s_pinto (modified)$
*/

#include <ltzvisor_hw.h>

/**
 * LTZVisor hardware initialization
 *
 * @param  
 *
 * @retval Return TRUE if success or False if not
 */
uint32_tee ltzvisor_hw_init(void){

	uint32_tee ret = TRUE;
	uint32_tee i;
	uint32_tee xCpuMode;
	__asm volatile ( "mrs %0, cpsr" : "=r" ( xCpuMode ) :: "memory" );
	/** Init Serial Port */
	for(i=0; i<10000; i++); //Comment if not running with U-boot
	ret = uart_init(1);
	if(!ret){
		/* ERROR */
		/* FIXME - Signal Error somewhere (LED) */
		return ret;
	}

	printk("\n\n\t");
	printk("----------------------------------------------------------\n\t");
	printk("                 MiniTEE (version %s)        \n\t", VERSION);
	printk("----------------------------------------------------------\n\t");
	printk("      *Monitor Mode: Current CPU mode 0x%08x... \r\n", xCpuMode);
	printk(" -> Arch (%s): CPU (%s) initialization ... \n\t", ARCH, CPU);
	printk(" -> Arch (%s): GIC initialization ... \n\t", ARCH);

	/** Initialize GIC Distributer and GIC Interface*/
	/* Distributor init */
	//ret = interrupt_distributor_init();
	//if(!ret){

		/* ERROR */
	//	printk("ERROR: GIC distributor init!\n\r");
	//	return ret;
	//}
	printk("      * GIC distributor - cancel  \n\t");
	/* Interface init */
	//ret = interrupt_interface_init();
	//if(!ret){

		/* ERROR */
	//	printk("ERROR: GIC interface init!\n\r");
	//	return ret;
	//}
	printk("      * GIC interface - cancel  \n\t");
	/* Config Interrupts Security */
	//interrupt_security_configall();		//ALL SPI --> NS
	//interrupt_security_config(UART_1_INTERRUPT,Int_NS);		//uart1(#82) --> NS
	//interrupt_security_config(TTC1_TTCx_2_INTERRUPT,Int_S);	//TTC1(#70)	---> S
	printk("      * GIC security - cancel  \n\t");

	/** Initialize Platform-specific */
	printk(" -> Platform (%s): initialization ... \n\t", PLATFORM);
	ret = board_init();
	if(!ret){
		/* ERROR */
		printk("ERROR: Platform init!\n\r");
		return ret;
	}

	return ret;
}

