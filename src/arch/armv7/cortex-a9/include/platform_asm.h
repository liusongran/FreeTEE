/**
 * This file contains CortexA9-specific register definitions.
 *
 */

#ifndef __PLATFORM_ASM
#define __PLATFORM_ASM

#define  NSACR_REG_VAL		0x60C00
#define  CPACR_REG_VAL		0xF00000

#define	CACHE_LEVEL1_SET	1
#define	CACHE_LEVEL2_SET	0

/*
 * LEVEL 1 Configuration
 * 32KB, NumSets = 256, Ways=4,
 * Line Size = 5 (log2 line_len) and Line len = 32 bytes
 */
#define MAX_L1_CACHE_WAYS	3   	/* Ways -1 */
#define MAX_L1_SETS		255 	/* NumSets -1 */
#define MAX_L1_LINE_LEN		5

#define MAX_L2_CACHE_WAYS	0   	/* Ways -1 */
#define MAX_L2_SETS		0	/* NumSets -1 */
#define MAX_L2_LINE_LEN		0

#endif /* __PLATFORM_ASM */
