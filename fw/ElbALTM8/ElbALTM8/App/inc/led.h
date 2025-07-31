/*
 * led.h
 *
 *  Created on: May 15, 2023
 *      Author: BaracchiF
 */

#ifndef _LED_H_
#define _LED_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/

typedef enum
{
	LED1 = 0, LED2, LED3, LED4
} LED_SEL;

typedef enum
{
	LED_OFF = 0x0,
	BLINK_VFAST = 0x1,
	BLINK_FAST = 0x4,
	BLINK_SLOW = 0x10,
	BLINK_VSLOW = 0x40,
	LED_ON = 0xff,
} LED_MODE;

/* Macro ---------------------------------------------------------------------*/
/* Defines -------------------------------------------------------------------*/
/* functions -----------------------------------------------------------------*/

void	LED_Init(void);
void	LED_Cmd(LED_MODE led_mode);


#ifdef __cplusplus
}
#endif


#endif /* _LED_H_ */
