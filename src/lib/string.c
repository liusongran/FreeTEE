#include <string.h>

/**
 * Copy block of memory
 *
 * @param	dst = pointer to the destination array
 *		src = pointer to the source of data to be copied
 *		count = number of bytes to copy
 *
 * @retval  destination is returned
 */
void * memcpy(void * dst, const void * src, uint32_tee count)
{
	int32_tee i;
	char_tee *dst_tmp = dst;
	const char_tee *src_tmp = src;

	//if (!((uint32_tee)src & 0x3) && !((uint32_tee)dst & 0x3)){
		/** Word aligned - safe word copies */
	//	for (i=0; i < count; i+=4){
	//		if (i + 3 > count - 1)
	//			break; /* Don't copy too much */
	//		*(uint32_tee *)dst_tmp = *(uint32_tee *)src_tmp;
	//		dst_tmp += 4;
	//		src_tmp += 4;
	//	}
	//	if (i <= count - 1){
	//		for (; i < count; i++){
	//			*dst_tmp = *src_tmp;
	//			dst_tmp++;
	//			src_tmp++;
	//		}
	//	}
	//}
	//else{
		/* Generic version */
		for (i=0; i < count; i++)
			dst_tmp[i] = src_tmp[i];
	//}
	return dst;
}

/**
 * Fill block of memory
 *
 * @param	dst = pointer to the block of memory
 *		c = value to be set
 *		count = number of bytes to be set
 *
 * @retval  pointer to the block of memory is returned
 */
void *memset(void * dest, uint32_tee c, uint32_tee count)
{
	/** Standard bytewise memset */
	char_tee* d;
	d = (char_tee*) dest;

	while(count--){
		*d = c;
		d++;
	}

	return dest;
}

/**
 * Concatenate strings
 *
 * @param	dest = pointer to the destination array
 *		src = C string to be appended
 *
 * @retval  destination is returned
 */
char_tee * strcat(char_tee *dest, cchar_tee *src)
{
	char_tee *save = dest;

	for (; *dest; ++dest) ;
	while ((*dest++ = *src++) != 0) ;

	return (save);
}

/**
 * Get string length
 *
 * @param	s = C string
 *
 * @retval  The length of string
 */
uint32_tee strlen(char_tee * s){

	char_tee *sc;
	for (sc = s; *sc != '\0'; ++sc){
		/* Do nothing */
	}
	return sc - s;
}

/**
 * Get fixed-size string length
 *
 * @param	S = C string
 *		n = lsize
 *
 * @retval  The length of string
 */
uint32_tee strnlen(char_tee * s, size_t n){
	char_tee *str;

	for(str = s; *str != '\0' && n-- ; ++str){
		/* Do nothing */
	}
	return str-s;
}

/**
 * Copy string
 *
 * @param	dest = pointer to the destination array
 *		src = C string to be copied
 *
 * @retval  destination is returned
 */
char_tee * strcpy(char_tee * dest, char_tee * src)
{
	char_tee *tmp = dest;

	while ((*dest++ = *src++) != '\0'){
		/* Do nothing */
	}
	return tmp;
}

