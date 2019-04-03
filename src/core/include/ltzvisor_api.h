
#ifndef __LTZVISOR_API_H
#define __LTZVISOR_API_H

#include <string.h>
#include <cpu_defines.h>
#include <cpu_helper.h>
#include <cpu_vcpu.h>
#include <cpu_cp15_switch.h>
#include <ltzvisor.h>
#include <ltzvisor_nsguest_config.h>
#include <ltzvisor_hw.h>


/**
 * LTZVisor initialization
 *
 * @param
 *
 * @retval
 */
uint32_tee ltzvisor_init(void);

/**
 * LTZVisor Kick off implementation
 *
 * @param
 *
 * @retval
 */
void ltzvisor_kickoff(void);

/**
 * LTZVisor Scheduler
 *
 * @param
 *
 * @retval
 */
void ltzvisor_schedule(void);

/**
 * LTZVisor NS_guest create
 *
 * @param  	g = pointer to guest configuration
 *
 * @retval 	TRUE if it is successful; FALSE if not!
 */
uint32_tee ltzvisor_nsguest_create( struct nsguest_conf_entry *g );

/**
 * LTZVisor NS_guest restart
 *
 * @param  	g = pointer to guest configuration
 *
 * @retval 	TRUE if it is successful; FALSE if not!
 */
uint32_tee ltzvisor_nsguest_restart( struct nsguest_conf_entry *g );


#endif /* __LTZVISOR_API_H */
