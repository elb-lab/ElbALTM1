/*
 * io.h
 *
 *  Created on: Oct 31, 2020
 *      Author: BaracchiF
 */

/**
  ******************************************************************************
  * @file           : io.h
  * @brief          : input/output manager
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __IO_H__
#define __IO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* Typedef -------------------------------------------------------------------*/

#ifndef bit
typedef enum bit_ {
  OFF = 0,
  ON = 1, 
  _OFF = 2,
  _ON = 3,
  TOGGLE = 0x80,
} bit;
#endif

#ifndef pin
typedef struct pin_ {
  GPIO_TypeDef*	GPIOx;
	u16				GPIO_Pin;
} pin;
#endif

/* stato pin */
typedef struct pin_stat_ {
	bit	hl;		// high level
	bit	ll;		// low level
	bit	re;		// rising edge
	bit	fe;		// falling edge
} pin_stat;

/* Macro ---------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

#define BUTT_NR			3

/* IO Definitions */
#define PIN_T1			PA5
#define PIN_T2			PA0
#define PIN_T3			PA6

/* IO Mask */
typedef enum {
	PIN_MSK_T1	=		0x0001,
	PIN_MSK_T2  =		0x0002,
	PIN_MSK_T3  =		0x0004,
} pin_mask;

/* Variables -----------------------------------------------------------------*/

extern pin	butt[3];

/* functions -----------------------------------------------------------------*/

void	IO_Init(void);
u8		IO_Service(void);
u16		IO_GetNr(void);
pin_stat *IO_GetPin(void);
u32		IO_GetPinState(void);

#ifdef __cplusplus
}
#endif

#endif /* __IO_H__ */

/**
  * @}
  */

/**
* @}
*/
/****END OF FILE****/
