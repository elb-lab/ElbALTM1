/******************************************************************
*	DRAW
*	data: 12/09/2016
*	autore: Frank
*	Libreria grafica x display in bianco/nero
*	NOTA:
******************************************************************/

#ifndef __DRAW_H
#define	__DRAW_H

/* Private typedef -----------------------------------------------------------*/

#include "stm32_mcu.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void	DRAW_PixelXY(uint16_t	x, uint16_t y, uint16_t color);
void	DRAW_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void	DRAW_Rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void	DRAW_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void	DRAW_Circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void	DRAW_PutChar(uint16_t x0, uint16_t y0, uint8_t chr);
void	DRAW_PutString(uint16_t x0, uint16_t y0, uint8_t *s);
void	DRAW_PutImage(uint8_t *img, uint8_t x, uint8_t y, uint8_t	w, uint8_t h);

#endif	/* __DRAW_H */

/**** END OF FILE ****/
