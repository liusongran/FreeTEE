
/** TEE includes */
#include "sm.h"
#include "types.h"

/** Variable & function definition */
extern int main_monitor(uint32_tee func_ID);

static uint32_tee fast_smc_entry(uint32_tee func_ID);
static uint32_tee std_smc_entry(uint32_tee func_ID);

/**---------------------------------------------------------------------*/
/**
 * Handling syscalls (SMCs) from non-secure world
 *
 * @param
 *
 * @retval
 */
uint32_tee sm_from_nsec(uint32_tee arg0, uint32_tee arg1, uint32_tee arg2, uint32_tee arg3)
{
    /**FIXME: NEXT VERSION!
     * Currently, we only use register r0 here.
     * other registers can be used for shared memory registration.
     */
    uint32_tee func_ID;
    uint32_tee res;   //result

    func_ID = TEE_SMC_FUNC_NUM(arg0);
	/** fast SMC or std SMC ? */
    if (TEE_SMC_IS_FAST_CALL(arg0)) {
        res = fast_smc_entry(func_ID);
    }
    else
    {
        res = std_smc_entry(func_ID);
    }
	return res;	/* return into secure state */

}



/**
 * fast smc handler
 *
 * @param  	
 *
 * @retval 	
 */
static uint32_tee fast_smc_entry(uint32_tee func_ID){

    /**
     * TODO: mask FIQ/IRQ, missing some functions here...
    */
	uint32_tee ret;

	ret = main_monitor(func_ID);
	return ret;

}

/**
 * Standard smc handler
 *
 * @param
 *
 * @retval
 */
static uint32_tee std_smc_entry(uint32_tee func_ID){
    /**
     * TODO: missing functions...
    */
	uint32_tee ret;

	ret = main_monitor(func_ID);
	return ret;
}
