
#ifndef __STM32_TYPE_H
#define __STM32_TYPE_H

typedef signed long long  s64;
typedef signed long   		s32;
typedef signed short  		s16;
typedef signed char   		s8;

typedef const s64		sc64;  /*!< Read Only */
typedef const s32		sc32;  /*!< Read Only */
typedef const s16		sc16;  /*!< Read Only */
typedef const s8		sc8;   /*!< Read Only */

typedef volatile long long	vs64;
typedef volatile long				vs32;
typedef volatile short			vs16;
typedef volatile char				vs8;

typedef volatile const signed long long	vsc64;  /*!< Read Only */
typedef volatile const signed long			vsc32;  /*!< Read Only */
typedef volatile const signed short			vsc16;  /*!< Read Only */
typedef volatile const signed char			vsc8;   /*!< Read Only */

typedef unsigned long long	u64;
typedef unsigned long				u32;
typedef unsigned short			u16;
typedef unsigned char				u8;

typedef const unsigned long long	uc64;  /*!< Read Only */
typedef const unsigned long				uc32;  /*!< Read Only */
typedef const unsigned short			uc16;  /*!< Read Only */
typedef const unsigned char				uc8;   /*!< Read Only */

typedef volatile unsigned long long	vu64;
typedef volatile unsigned long  		vu32;
typedef volatile unsigned short			vu16;
typedef volatile unsigned char			vu8;

typedef volatile const  unsigned long long	vuc64;  /*!< Read Only */
typedef volatile const  unsigned long				vuc32;  /*!< Read Only */
typedef volatile const  unsigned short			vuc16;  /*!< Read Only */
typedef volatile const  unsigned char				vuc8;   /*!< Read Only */

#ifndef bool
typedef unsigned char	bool;
#endif

typedef enum {	FALSE=0, 	TRUE=1	}	BOOL;

#endif  /* __STM32_TYPE_H */
