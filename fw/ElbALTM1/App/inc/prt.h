/******************************************************************
*	PRT
*	data: 29/01/2020
*	autore: Frank
*	DRIVER x Sensore di correnre TI INA219 via BUS I2C
*	NOTA:
******************************************************************/

#ifndef __PRT_H
#define	__PRT_H

#include "stm32_mcu.h"
#include "stm32_type.h"
#include "oled128x64.h"
#include "font.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum {	LEFT = 0,	CENTER, RIGHT	}	ALLIGN;

/* Private define ------------------------------------------------------------*/

#define prt_font		lucidaConsole_10ptFontInfo
#define PRT_NEXT		0xffff

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u16		PRT_CharWidth(u8 c);
u16		PRT_StringWidth(u8 *s);
void	PRT_PutChar(u8 c, u16 x, u16 y);
u16		PRT_PutString(u8 *s, ALLIGN mode, u16 x, u16 y);
void	PRT_FillSpace(u16 x1, u16 x2, u16 y);
void	PRT_SetFont(FONT_INFO f);
void PRT_PutImageGS(u8 *image, u16 x, u16 y);

#endif	/* __PRT_H */

/**** END OF FILE ****/
