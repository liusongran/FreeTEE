#ifndef __PRINTK_H
#define __PRINTK_H

#include <types.h>
#include <string.h>
#include <zynq_uart.h>

typedef char *va_list;

#define ALIGNBND		(sizeof (signed int) - 1)

#define bnd(X, bnd)		(((sizeof (X)) + (bnd)) & (~(bnd)))

#define va_arg(ap, T)		(*(T *)(((ap) += (bnd (T, ALIGNBND))) - (bnd (T,ALIGNBND))))

#define va_end(ap)		(void) 0

#define va_start(ap, A)		(void) ((ap) = (((char *) &(A)) + (bnd (A,ALIGNBND))))

#define PRINT_TEXT_LEN         0x100

/**
 * Print formatted data to stdout
 *
 * @param	fmt =
 *
 * @retval
 */
uint32_tee printk(const char *fmt, ...);

#endif /* __PRINTK_H */
