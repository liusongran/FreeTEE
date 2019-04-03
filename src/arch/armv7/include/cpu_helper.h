#ifndef __ARMV7_CPU_HELPER_H
#define __ARMV7_CPU_HELPER_H

#include <types.h>

/**
 * Set the NS guest context pointer (optimization)
 *
 * @param	addr = context pointer address
 *
 * @retval
 */
void set_guest_context(uint32_tee addr);


#endif /* __ARMV7_CPU_HELPER_H */
