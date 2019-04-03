#ifndef __IO_H
#define __IO_H

#include <types.h>

/**
 * Read a 32-bit address
 *
 * @param	addr = pointer to address
 *
 * @retval  content of address
 */
uint32_tee read32( volatile void *addr );

/**
 * Read a 32-bit address
 *
 * @param	addr = pointer to address
 *
 * @retval  content of address
 */
void write32( volatile void *addr, uint32_tee data );

#endif /* __IO_H */
