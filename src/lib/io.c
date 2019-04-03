#include <io.h>

/**
 * Read a 32-bit address
 *
 * @param	addr = pointer of address
 *
 * @retval  value pointed by addr
 */
inline uint32_tee read32( volatile void * addr ){

	return *(volatile uint32_tee*)addr;
}
/**
 * Write in a 32-bit address
 *
 * @param	addr = pointer of address
 * 		data = value
 *
 * @retval
 */
inline void write32( volatile void *addr, uint32_tee data ){

	*(volatile uint32_tee*)addr = data;
}

