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

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms5611_data		atm_data;
mpu_struct		imu_data = {0};
u8						sel = 0;

/* timers */
tmr	tmr_spy = 0;
tmr tmr_atm = 0;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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

	sel = 0;
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
	u8 s[32];

	OLED_Fill(1); // Clear the OLED display
	OLED_DisplayRefresh(); // Refresh the display to show the cleared screen
	beep(1,50,0); // Beep once with a short sound
	wait(1000); // Wait for 1 second

	PRT_SetFont(mSUIGothic_48ptFontInfo);

	while(1)
	{
		if (tmrTick(tmr_atm, 100))
		{
			IO_Service();	
			MS5611_ReadData(&atm_data);
			MPU_ReadData(&imu_data);

			OLED_Fill(0); // Clear the OLED display

			// PRT_PutString((u8*)"ATM: ", LEFT, 0, 0);
			// sprintf((char*)s, "Pres: %ld Pa", atm_data.pres);
			// PRT_PutString(s, LEFT, 0, 10);
			// sprintf((char*)s, "Alt: %ld m", atm_data.altm);
			// PRT_PutString(s, LEFT, 0, 20);
			// sprintf((char*)s, "Temp: %ld C", atm_data.temp);
			// PRT_PutString(s, LEFT, 0, 30);

			//sprintf((char*)s, "%ld.%01ld", atm_data.altm/100, (atm_data.altm % 100) / 10);
			butt = IO_GetPin();
			switch (sel)
			{
				case MNU_ALTIMETER:
					PRT_SetFont(mSUIGothic_48ptFontInfo);
					sprintf((char*)s, "%ld", atm_data.altm/100);
					PRT_PutString((u8*)s, CENTER, 63, 0);
					PRT_SetFont(lucidaConsole_10ptFontInfo);
					PRT_PutString((u8*)"-- metri --", CENTER, 63, 50);
					if (butt[2].re == ON)
					{
						sel = MNU_TEMPERATURE;
						beep(1,50,0);
					}
					else if (butt[0].re == ON)
					{
						sel = MNU_PRESSURE;
						beep(1,50,0);
					}
					break;
				case MNU_TEMPERATURE:
					PRT_SetFont(mSUIGothic_48ptFontInfo);
					sprintf((char*)s, "%ld", atm_data.temp/100);
					PRT_PutString((u8*)s, CENTER, 63, 0);
					PRT_SetFont(lucidaConsole_10ptFontInfo);
					PRT_PutString((u8*)"-- gradi C --", CENTER, 63, 50);
					if (butt[2].re == ON)
					{
						sel = MNU_PRESSURE;
						beep(1,50,0);
					}	
					else if (butt[0].re == ON)
					{
						sel = MNU_ALTIMETER;
						beep(1,50,0);
					}
					break;
				case MNU_PRESSURE:
					PRT_SetFont(mSUIGothic_48ptFontInfo);
					sprintf((char*)s, "%ld", atm_data.pres/100);
					PRT_PutString((u8*)s, CENTER, 63, 0);
					PRT_SetFont(lucidaConsole_10ptFontInfo);
					PRT_PutString((u8*)"-- mbar --", CENTER, 63, 50);
					if (butt[2].re == ON)
					{
						sel = MNU_ALTIMETER;
						beep(1,50,0);
					}	
					else if (butt[0].re == ON)
					{
						sel = MNU_TEMPERATURE;
						beep(1,50,0);
					}
					break;
			} /* switch */
			OLED_DisplayRefresh(); // Refresh the display to show the updated data
		}
	} /* while(1) */
} /* usr_main() */



/***** end of file *****/
