#ifndef __TYPES_H
#define __TYPES_H

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif
#ifndef flase
	#define true	1
	#define false   0
#endif

#ifndef NULL
	#define NULL	0
#endif

typedef char				char_tee;
typedef	const char			cchar_tee;
typedef signed char			int8_tee;
typedef signed short		int16_tee;
typedef signed int			int32_tee;
typedef signed long			int64_tee;
typedef unsigned char		uint8_tee;
typedef unsigned short		uint16_tee;
typedef unsigned int		uint32_tee;
typedef unsigned long		uint64_tee;
typedef float				float32_tee;
typedef double				float64_tee;
typedef long double			float128_tee;
typedef unsigned int		size_t;
typedef unsigned long long	big_ulong;


#endif
