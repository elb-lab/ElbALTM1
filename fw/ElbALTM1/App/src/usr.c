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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms5611_data		atm_data;
mpu_struct		imu_data = {0};

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
	OLED_Init();
	BEEP_Init();
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
			sprintf((char*)s, "%ld", atm_data.altm/100);
			PRT_PutString((u8*)s, CENTER, 63, 10);

			OLED_DisplayRefresh(); // Refresh the display to show the updated data
		}
	} /* while(1) */
} /* usr_main() */



/***** end of file *****/
