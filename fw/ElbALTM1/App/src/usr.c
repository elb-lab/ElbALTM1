/**
  ******************************************************************************
  * @file           : usr.c
  * @brief          : user program module
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "sys.h"
#include "tmr.h"
#include "ms5611.h"
#include "mpu6050.h"
#include "oled128x64.h"
#include "prt.h"
#include "draw.h"
#include "font.h"
#include "beep.h"
#include "io.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum MENU_LIST_ {
	MNU_ALTIMETER = 0,
	MNU_TEMPERATURE,
	MNU_PRESSURE,
} MENU_LIST;

typedef struct menu_struct_ {
	MENU_LIST	menu, prev, next;
	void (*func)(s16 y);
} menu_struct;

typedef enum MENU_SKROLL_DIR_ {
	SKROLL_RIGHT = 0,
	SKROLL_LEFT,
} MENU_SKROLL_DIR;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms5611_data		atm_data;
mpu_struct		imu_data = {0};
s16						x0 = 0;

/* timers */
tmr	tmr_dsy = 0;
tmr tmr_atm = 0;
tmr	tmr_anim;

menu_struct menu;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief Visualizzazione pagina
 * 
 * @param pos_x 
 * @param s_value 
 * @param s_title 
 */
static void vis_value(s16 pos_x, u32 s_value, u8 *s_title)
{
	u8 s[32];

	PRT_SetFont(mSUIGothic_48ptFontInfo);
	sprintf((char*)s, "%ld", s_value);
	PRT_PutString((u8*)s, CENTER, 63 + pos_x, 0);
	PRT_SetFont(lucidaConsole_10ptFontInfo);
	PRT_PutString((u8*)s_title, CENTER, 63 - pos_x, 50);
}

/**
 * @brief Visualizzazione altimetria
 * 
 */
void __menu_altimeter__(s16 pos_x)
{
	vis_value(pos_x, atm_data.altm / 100, (u8*)"-- Metri --");
}

/**
 * @brief Visualizzazione pressione
 * 
 */
void __menu_pressure__(s16 pos_x)
{
	vis_value(pos_x, atm_data.pres / 100, (u8*)"-- mbar --");
}

/**
 * @brief Visualizzazione temperature
 * 
 */
void __menu_temperature__(s16 pos_x)
{
	vis_value(pos_x, atm_data.temp / 100, (u8*)"-- Gradi C --");
}

static const menu_struct menu_list[] = {
	/*menu,							next,							prev,							function							*/
	{	MNU_ALTIMETER,		MNU_TEMPERATURE,	MNU_PRESSURE, 		__menu_altimeter__		},
	{ MNU_TEMPERATURE,	MNU_PRESSURE, 		MNU_ALTIMETER, 		__menu_temperature__	},
	{ MNU_PRESSURE,			MNU_ALTIMETER, 		MNU_TEMPERATURE,	__menu_pressure__			},
};

/*******************************************************************************
* Function Name  : usr_setup
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void usr_setup(void)
{
  MS5611_Init();
  MPU_Init();
	BEEP_Init();
	IO_Init();

	wait(100);

	OLED_Init();
}

/*******************************************************************************
* Function Name  : usr_main
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void usr_main(void)
{
	pin_stat *butt;
	s16 x;
	MENU_SKROLL_DIR dir = SKROLL_RIGHT;

	OLED_Fill(1); // Clear the OLED display
	OLED_DisplayRefresh(); // Refresh the display to show the cleared screen
	beep(1,50,0); // Beep once with a short sound
	wait(1000); // Wait for 1 second

	PRT_SetFont(mSUIGothic_48ptFontInfo);

	menu = menu_list[MNU_ALTIMETER];

	tmr_anim = 0;

	x0 = -63;

	while(1)
	{
		IO_Service();	

		butt = IO_GetPin();
		if (tmrTick(tmr_atm, 100))
		{
			MS5611_ReadData(&atm_data);
			//MPU_ReadData(&imu_data);
		}

		if (tmrTick(tmr_anim, 5))
		{
			if (dir == SKROLL_RIGHT)
			{
				x += 16;
				if (x >= 128)
				{
					x = -128;
					menu = menu_list[menu.next];
				}
				else if (x == 0)
				{
					tmr_anim = 7000;
				}
			} // SKROLL_RIGHT
			else
			{
				x -= 16;
				if (x <= -128)
				{
					x = 128;
					menu = menu_list[menu.prev];
				}
				else if (x == 0)
				{
					tmr_anim = 7000;
				}
			}
		}

		/* cambio menu */
		if (butt[0].re)
		{
			tmr_anim = 10;
			dir = SKROLL_LEFT;
		}
		else if (butt[2].re)
		{
			tmr_anim = 10;
			dir = SKROLL_RIGHT;
		}

		if (tmrTick(tmr_dsy, 50))
		{
			OLED_Fill(0); // Clear the OLED display
			menu.func(x);
			OLED_DisplayRefresh(); // Refresh the display to show the updated data
		}

	} /* while(1) */
} /* usr_main() */



/***** end of file *****/
