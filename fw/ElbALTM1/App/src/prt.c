/******************************************************************
*	PRT
*	data: 29/01/2020
*	autore: Frank
*	Output stringhe sul display
*	NOTA:
******************************************************************/
#include <string.h>
#include <stdio.h>
#include "stm32_mcu.h"
#include "stm32_type.h"
#include "tmr.h"
#include "font.h"
#include "oled128x64.h"
#include "prt.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u16	next_x;
FONT_INFO	pfont;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : PRT_SetFont
* Description    : Set the characters font
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	PRT_SetFont(FONT_INFO f)
{
	pfont = f;
}

/*******************************************************************************
* Function Name  : PRT_CharWidth
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u16	PRT_CharWidth(u8 c)
{
	return (pfont.c_descr[c-pfont.c_start].width);
}

/*******************************************************************************
* Function Name  : PRT_StringWidth
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u16	PRT_StringWidth(u8 *s)
{
	u16 sum = 0;

	do {
		sum += PRT_CharWidth(*s);
		++s;
	} while (*s != '\0');
	
	return (sum);
}

/*******************************************************************************
* Function Name  : PRT_PutChar
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	PRT_PutChar(u8 c, u16 x, u16 y)
{
	FONT_CHAR_INFO	c_info;
	u8	*p;
	u8	ch;
	u16	w;
	u16	h;
	
	ch = c - pfont.c_start;
	c_info = pfont.c_descr[ch];
	p =	(u8*)pfont.c_bmp + c_info.offset;
	w = c_info.width;
	h = c_info.height;
	OLED_PutImage(p,x,y,w,h);
	next_x = x + w;
}

/*******************************************************************************
* Function Name  : PRT_PutString
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u16	PRT_PutString(u8 *s, ALLIGN mode, u16 x, u16 y)
{
	u16 sum = 0;
	
	sum = PRT_StringWidth(s);

	switch (mode)
	{
		case CENTER:
			sum >>= 1;
			next_x = x - sum;
			break;
			/* fall through */
			
		case RIGHT:
			if (x > sum)
				next_x = x - sum;
			else
				next_x = 0;
			break;
			
		default:
			next_x = x;
	}

	do {		
		PRT_PutChar(*s, next_x, y); 
		++s;
	}	while(*s != '\0');
	
	return (sum);
}

/*******************************************************************************
* Function Name  : PRT_FillSpace
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	PRT_FillSpace(u16 x1, u16 x2, u16 y)
{
	u16	xx;
	
	if (x2 > x1)
	{
		xx = x1;
		x1 = x2;
		x2 = xx;
	}
	
	if (x1 != PRT_NEXT)
		next_x = x1;
		
	do {
		PRT_PutChar(' ',next_x, y);
	} while(next_x < x2);
}

/*******************************************************************************
* Function Name  : PRT_PutImageGS
* Description    : Double to ASCII
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void PRT_PutImageGS(u8 *image, u16 x, u16 y)
{
	u16 w = (image[3] << 8) + image[2];
	u16	h = (image[5] << 8) + image[4];
	u16	nc = image[1];
	if (nc != 4)	return;
	OLED_PutImage((u8*)(&image[6]), x, y, w, h);
}

/**** END OF FILE ****/
