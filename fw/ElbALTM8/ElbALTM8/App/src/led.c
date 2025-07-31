/*
 * led.c
 *
 *  Created on: May 15, 2023
 *      Author: BaracchiF
 */
/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "led.h"

/* Private typedef -----------------------------------------------------------*/

typedef	u8	ledcfg;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8	led_mux = 0x0;
u8	rit_led = 0;
u8	mled = LED_OFF;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LED_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void LED_Init(void)
{
	led_mux = 0x0;
	rit_led = 0;
	mled = LED_OFF;
}

/*******************************************************************************
* Function Name  : LED_Cmd
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void LED_Cmd(LED_MODE led_mode)
{
	mled = led_mode;
}

/*******************************************************************************
* Function Name  : LED_Interrupt
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void LED_Irq(void)
{
	/* esegue il refresh dei led ogni 50ms */
	if (rit_led)
	{
		--rit_led;
		return;
	}

	if ((mled == LED_ON) || (mled & led_mux))
	{
		GPIOD->OUTDR &= ~GPIO_Pin_4;
	}
	else
	{
		GPIOD->OUTDR |= GPIO_Pin_4;
	}

	++led_mux;
	rit_led = 25;		// 25ms
}


/***** end of LED module *****/
