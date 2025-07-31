/******************************************************************
*	OLED
*	data: 07/09/2016
*	autore: Frank
*	DRIVER x OLED via BUS I2C (driver SSD1306)
*	NOTA:
******************************************************************/

#ifndef __OLED_H
#define	__OLED_H

/* Private typedef -----------------------------------------------------------*/

#include "sys.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void	OLED_Init(void);
void	OLED_DisplayRefresh(void);
void 	OLED_Fill(u8 color);
void	OLED_Pixel(u8 x, u8 y, u8 pixel);
void	OLED_PutImage(u8 *image, u8 x, u8 y, u8	width, u8 high);

#endif	/* __OLED_H */

/**** END OF FILE ****/
