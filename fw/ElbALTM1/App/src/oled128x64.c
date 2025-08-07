/******************************************************************
*	OLED
*	data: 07/09/2016
*	autore: Frank
*	DRIVER x OLED via BUS I2C (driver SSD1306)
*	NOTA:
******************************************************************/
#include <string.h>
#include "sys.h"
#include "i2c.h"
#include "tmr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define DELAY_EPR_T_OUT			1200		/* time-out comunivazione con eeprom */
#define	OLED_SLAVE_ADDRESS	0x78	/* comando di Scrittura */
#define OLED_CONTIN_STREAM	0x80	/* flag di continnuazione scrittura */
#define OLED_END_STREAM			0x00	/* flag di fine scrittura */
#define OLED_DATA_STREAM		0x40	
#define OLED_COMMAND_STREAM	0x00	

#define OLED_ORIZ_RES				128
#define OLED_VERT_RES				64
#define OLED_VERT_PAGES			OLED_VERT_RES / 8

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8	fb[(OLED_VERT_PAGES * OLED_ORIZ_RES) + 1]; // frame buffer

/* binary multiplexer */
static const u8 mux[] ={ 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7 };

static const u8 oled_init_tab[] = {
	0xAE, //display off
	0x20, //Set Memory Addressing Mode
	0x10, //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET,11,Invalid
	
	0x21,	// Set column address range
	0x20,	// from 0
	0x7F,	// to 127
	0x22,	// Set pages address range
	0x04,	// from page #0
	0x0F,	// to page #3
	
	0xB0, //Set Page Start Address for Page Addressing Mode,0-7
	0xC8, //Set COM Output Scan Direction
	0x00, //---set low column address
	0x10, //---set high column address
	0x40, //--set start line address

	0x81, //--set contrast control register
	0xFF,
	
	0xA1, //--set segment re-map 0 to 127
	0xA6, //--set normal display
	0xA8, //--set multiplex ratio(1 to 64)
	0x3F, //
	0xA4, //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	0xD3, //-set display offset
	0x00, //-not offset
	0xD5, //--set display clock divide ratio/oscillator frequency
	0xF0, //--set divide ratio
	0xD9, //--set pre-charge period
	0x22, //
	0xDA, //--set com pins hardware configuration
	0x12,
	0xDB, //--set vcomh
	0x20, //0x20,0.77xVcc
	0x8D, //--set DC-DC enable
	0x14, //
	0xAF, //--turn on SSD1306 panel
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : OLED_Wait
* Description    : I2C BUS Write command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//static
//void OLED_Wait(u32 i)
//{
//	u32	n = i;
//	while(--n);
//}

/*******************************************************************************
* Function Name  : OLED_WrCmd
* Description    : I2C BUS Write command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static
u8	OLED_WrCmd(u8 command)
{
	HAL_StatusTypeDef	res;

	do {
		res = HAL_I2C_Mem_Write(
				&hi2c1,
				OLED_SLAVE_ADDRESS,
				OLED_END_STREAM | OLED_COMMAND_STREAM,
				I2C_MEMADD_SIZE_8BIT,
				&command,
				1, 1000
		);
	} while (res == HAL_BUSY);

	return res;
}

/*******************************************************************************
* Function Name  : OLED_WrData
* Description    : I2C BUS Write Data
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static
HAL_StatusTypeDef	OLED_WrData(u8 *buffer, u16 len)
{
	HAL_StatusTypeDef	res;

	do {
		res = HAL_I2C_Mem_Write(
				&hi2c1,
				OLED_SLAVE_ADDRESS,
				OLED_DATA_STREAM,
				I2C_MEMADD_SIZE_8BIT,
				(u8*)buffer,
				len, 1000
		);
	} while(res == HAL_BUSY);

	return res;
}

/*******************************************************************************
* Function Name  : OLED_DisplayRefresh
* Description    : Transfer the intere buffer contenent into dispaly
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	OLED_DisplayRefresh(void)
{
	u8	j;

	for (j=0; j<OLED_VERT_PAGES; j++)
	{
		OLED_WrCmd(0xB0 + j);
		OLED_WrCmd(0x00);
		OLED_WrCmd(0x10);
		OLED_WrData(&fb[OLED_ORIZ_RES * j], OLED_ORIZ_RES);
	}

}

/*******************************************************************************
 * Function Name  : OLED_FillPage().
 * Description    : Fill all display
 *                  via SPI1 port.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/

void OLED_Fill(u8 color)
{
	u8 c;

	if (color)	c = 0xff;
	else				c = 0x00;
	memset(fb, c, sizeof(fb));
}

/*******************************************************************************
* Function Name  : OLED_Pixel
* Description    : ON/OFF a pixel on screnn
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void OLED_Pixel(u8 x, u8 y, u8 color)
{
	u8	*c;
	u8	s;

	if (x >= OLED_ORIZ_RES || y >= OLED_VERT_RES)
		return;

	c = &fb[x + (y / 8) * OLED_ORIZ_RES];
	s = 1 << (y % 8);

	if (color)	*c |= s;
	else				*c &= ~s;

}

/*******************************************************************************
* Function Name  : OLED_PutImage
* Description    : Put a Image on screen
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	OLED_PutImage(u8 *image, u8 x, u8 y, u8	width, u8 high)
{
	u8	x1, y1, z1;
	u8	pixel;

	for (y1=0; y1<high; y1++)
	{
		for (x1=0; x1<width; x1++)
		{
			z1 = x1 % 8;
			if (z1 == 0)
			{
				pixel = *image;
				++image;
			}
			OLED_Pixel(x+x1, y+y1, pixel & mux[z1]);
		}
	}
}

/*******************************************************************************
* Function Name  : OLED_Init
* Description    : I2C BUS Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void OLED_Init(void)
{
	u8 i;

	/* OLED ON Command */
	for (i=0; i<sizeof(oled_init_tab); i++)
	{
		OLED_WrCmd((u8)oled_init_tab[i]);
	}
} /* eprInit */



/**** END OF FILE ****/
