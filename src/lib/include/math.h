#ifndef __MATH_H
#define __MATH_H

#include <types.h>

/**
 * Check if is pow of 2
 *
 * @param	number = value whose pow of 2
 *
 * @retval  pow of 2 of x
 */
inline int is_pow_of_2(big_ulong x);

/**
 * Compute next pow of 2
 *
 * @param	number = value whose pow is calculated
 *
 * @retval  next pow of 2 of x
 */
inline uint32_tee next_pow_of_2(uint32_tee x);

/**
 * Compute binary logarithm
 *
 * @param	number = value whose logarithm is calculated
 *
 * @retval  binary logarithm of number
 */
uint32_tee log_of_2(uint32_tee number);


#endif /* __MATH_H */
