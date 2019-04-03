#ifndef __STRING_H_
#define __STRING_H_

#include <types.h>

/**
 * Copy block of memory
 *
 * @param	dst = pointer to the destination array
 *		src = pointer to the source of data to be copied
 *		count = number of bytes to copy
 *
 * @retval  destination is returned
 */
void *memcpy(void *dst, const void *src, uint32_tee count);

/**
 * Fill block of memory
 *
 * @param	dst = pointer to the block of memory
 *		c = value to be set
 *		count = number of bytes to be set
 *
 * @retval  pointer to the block of memory is returned
 */
void *memset(void * dest, uint32_tee c, uint32_tee count);

/**
 * Concatenate strings
 *
 * @param	dest = pointer to the destination array
 *		src = C string to be appended
 *
 * @retval  destination is returned
 */
char_tee * strcat(char_tee *dest, cchar_tee *src);


/**
 * Get string length
 *
 * @param	s = C string
 *
 * @retval  The length of string
 */
uint32_tee strlen(char_tee * s);

/**
 * Get fixed-size string length
 *
 * @param	S = C string
 *		n = lsize
 *
 * @retval  The length of string
 */
uint32_tee strnlen(char_tee * s, size_t n);

/**
 * Copy string
 *
 * @param	dest = pointer to the destination array
 *		src = C string to be copied
 *
 * @retval  destination is returned
 */
char_tee * strcpy(char_tee * dest, char_tee * src);

#endif /* __STRING_H_ */
