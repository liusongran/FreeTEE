#ifndef SM_SM_H
#define SM_SM_H

//#include "types.h"

#define TEE_SMC_FAST_CALL		0x80000000
#define TEE_SMC_FUNC_MASK		0xff

/**Non-secure world ID: TA task ID in Secure FreeRTOS*/
#define MAX_NUM_TA				10
#define TA_HELLO_WORLD			0
#define TA_BLINK_LED			1
#define	TA_GEN_RANDOM_NUM		2
#define	TA_ENCRYPTION			3

/** Secure world ID in r0*/
#define INIT_TEE_SUCCESS		0x20190530
#define TA_CALL_SUCCESS			0x20190612
#define TA_CALL_FAILED			0x20190726

#define TEE_SMC_IS_FAST_CALL(smc_val)	((smc_val) & TEE_SMC_FAST_CALL)
#define TEE_SMC_FUNC_NUM(smc_val)	((smc_val) & TEE_SMC_FUNC_MASK)

//unsigned int sm_from_nsec(unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3);

#endif /*SM_SM_H*/
