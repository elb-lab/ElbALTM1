/******************************************************************
*	DRAW
*	data: 12/09/2016
*	autore: Frank
*	Libreria grafica x display in bianco/nero
*	NOTA:
******************************************************************/

#include <math.h>
#include "stm32_mcu.h"			// librerie stm32
#include "oled128x64.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constances --------------------------------------------------------*/

double sin_tab[] = {
	0.00, 0.02, 0.03, 0.05, 0.07, 0.09,	0.10,	0.12,	0.14,	0.16,			
	0.17,	0.19,	0.21,	0.22,	0.24,	0.26,	0.28,	0.29,	0.31,	0.33,			
	0.34,	0.36,	0.37,	0.39,	0.41,	0.42,	0.44,	0.45,	0.47,	0.48,			
	0.50, 0.52,	0.53,	0.54,	0.56,	0.57,	0.59,	0.60,	0.62,	0.63,			
	0.64,	0.66,	0.67,	0.68,	0.69,	0.71,	0.72,	0.73,	0.74,	0.75,			
	0.77,	0.78,	0.79,	0.80,	0.81,	0.82,	0.83,	0.84,	0.85,	0.86,			
	0.87,	0.87,	0.88,	0.89,	0.90,	0.91,	0.91,	0.92,	0.93,	0.93,			
	0.94,			
	0.95,			
	0.95,			
	0.96,			
	0.96,			
	0.97,			
	0.97,			
	0.97,			
	0.98,			
	0.98,			
	0.98,			
	0.99,			
	0.99,			
	0.99,			
	0.99,			
	1.00,			
	1.00,			
	1.00,			
	1.00,			
	1.00,			
	1.00
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : DRAW_Swap
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static
void	DRAW_Swap(uint16_t *a, uint16_t *b)
{
	uint16_t c;
	
	c = *a;	*a = *b;	*b = c;
}

/*******************************************************************************
* Function Name  : DRAW_Line
* Description    : I2C BUS Write command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	DRAW_PixelXY(uint16_t	x, uint16_t y, uint16_t color)
{
	uint8_t c;
	
	if (x > 127 || y > 127)	return;
	
	c = color & 0x0f;
	
	OLED_Pixel(x, y, c);
	
}

/*******************************************************************************
* Function Name  : DRAW_Line
* Description    : I2C BUS Write command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	DRAW_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t x, y, k;
	
	if (x1 > x2)		DRAW_Swap(&x1, &x2);
	if (y1 > y2)		DRAW_Swap(&y1, &y2);
	
	if ((x2 - x1) >= (y2 - y1))
	{
		k = (x2 - x1) / (y2 - y1);
		y = y1;
		for (x=x1; x<x2; x++)
		{
			DRAW_PixelXY(x, y, color);
			if ((x % k) == 0)		++y;
		}
	}
	else
	{
		k = (y2 - y1) / (x2 - x1);
		x = x1;
		for (y=y1; y<y2; y++)
		{
			DRAW_PixelXY(x, y, color);
			if ((y % k) == 0)		++x;
		}
	}
}

/*******************************************************************************
* Function Name  : DRAW_Rect
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	DRAW_Rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	DRAW_Line(x1, y1, x2, y1, color);
	DRAW_Line(x2, y1, x2, y2, color);
	DRAW_Line(x2, y2, x1, y2, color);
	DRAW_Line(x1, y2, x1, y1, color);
}	

/*******************************************************************************
* Function Name  : DRAW_FillRect
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	DRAW_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t y;
	
	for (y=y1; y<y2; y++)
	{
		DRAW_Line(x1, y, x2, y, color);
	}
}

/*******************************************************************************
* Function Name  : DRAW_Circle
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void DRAW_Circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	double x1, y1, x2, y2;
	uint16_t	a;
	
	for (a=0; a<90; a++)
	{
		x1 =  (uint16_t)( r * sin_tab[a]) + x0;
		y1 =  (uint16_t)( r * sin_tab[90-a]) + y0;
		x2 =  (uint16_t)( r * sin_tab[a+1]) + x0;
		y2 =  (uint16_t)( r * sin_tab[90-a-1]) + y0;
		DRAW_Line(x1,y1,x2,y2,color);
		x1 =  (uint16_t)( r * sin_tab[a]) + x0;
		y1 = -(uint16_t)( r * sin_tab[90-a]) + y0;
		x2 =  (uint16_t)( r * sin_tab[a+1]) + x0;
		y2 = -(uint16_t)( r * sin_tab[90-a-1]) + y0;
		DRAW_Line(x1,y1,x2,y2,color);
		x1 = -(uint16_t)( r * sin_tab[a]) + x0;
		y1 =  (uint16_t)( r * sin_tab[90-a]) + y0;
		x2 = -(uint16_t)( r * sin_tab[a+1]) + x0;
		y2 =  (uint16_t)( r * sin_tab[90-a-1]) + y0;
		DRAW_Line(x1,y1,x2,y2,color);
		x1 = -(uint16_t)( r * sin_tab[a]) + x0;
		y1 = -(uint16_t)( r * sin_tab[90-a]) + y0;
		x2 = -(uint16_t)( r * sin_tab[a+1]) + x0;
		y2 = -(uint16_t)( r * sin_tab[90-a-1]) + y0;
		DRAW_Line(x1,y1,x2,y2,color);
	}
}	

/*******************************************************************************
* Function Name  : DRAW_PutChar
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

//void	DRAW_PutChar(uint16_t x0, uint16_t y0, uint8_t chr)
//{
//	uint16_t	x, y;
//	uint8_t		d;
//
//	if (chr < ' ' || chr > 0x7f)		return;
//
//	chr -= ' ';
//
//	for (y=0; y<8; y++)
//	{
//		d = font5x7[chr][y];
//		for (x=0; x<8; x++)
//		{
//			DRAW_PixelXY(x0+x, y0+y, d & 0x80);
//			d <<= 1;
//		}
//	}
//
//}

/*******************************************************************************
* Function Name  : DRAW_PutChar
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

//void	DRAW_PutString(uint16_t x0, uint16_t y0, uint8_t *s)
//{
//	do {
//		DRAW_PutChar(x0, y0, *s);
//		x0 += 6;	++s;
//	} while(*s != '\0');
//}

/*******************************************************************************
* Function Name  : DRAW_PutImage
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

//void	DRAW_PutImage(uint8_t *img, uint8_t x, uint8_t y, uint8_t	w, uint8_t h)
//{
//	uint16_t	dw;
//	uint16_t	px, py, dx;
//	uint8_t		byt;
//
//	dw = w / 8;
//	if (w % 8)		dw += 1;
//
//	for (py=0; py<h; py++)
//	{
//		for (px=0; px<dw; px++)
//		{
//			byt = img[px+dw*py];
//			for (dx=0; dx<8; dx++)
//			{
//				DRAW_PixelXY(8 * px + dx + x, py + y, (byt & 0x80));
//				byt <<= 1;
//			}
//		}
//	}
//}

/**** END OF FILE ****/
