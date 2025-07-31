/**
  ******************************************************************************
  * @file           : usr.c
  * @brief          : user program module
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include "tmr.h"
#include "led.h"
#include "scp.h"
#include "atcmd.h"
#include "ms5611.h"
#include "rel.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms5611_data		atm_data;

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
  TMR_Init();
  LED_Init();
  SCP_Init();
  ATCMD_Init();
  MS5611_Init();

  while(strcmp("ElbAltm", rel.fw_name) != 0);

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
	while(1)
	{
		if (tmrTick(tmr_atm, 100))
		{
			MS5611_ReadData(&atm_data);
		}
		if (ATCMD_Service())
		{
			LED_Cmd(BLINK_VFAST);
			tmrStart(tmr_spy, 1000);
		}
		else if (!tmr_spy)
		{
			LED_Cmd(BLINK_SLOW);
		}
	} /* while(1) */
} /* usr_main() */


/***** end of file *****/
