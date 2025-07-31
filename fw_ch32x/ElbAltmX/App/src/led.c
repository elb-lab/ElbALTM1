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
LED_MODE	mled = LED_OFF;

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
	GPIO_InitTypeDef io_param = {0};

	led_mux = 0x0;
	rit_led = 0;
	mled = LED_OFF;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_WriteBit(PB7, Bit_SET);
	io_param.GPIO_Mode = GPIO_Mode_Out_PP;
	io_param.GPIO_Pin = GPIO_Pin_7;
	io_param.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &io_param);

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
		GPIOB->OUTDR &= ~GPIO_Pin_7;
	}
	else
	{
		GPIOB->OUTDR |=  GPIO_Pin_7;
	}

	++led_mux;
	rit_led = 25;		// 25ms
}


/***** end of LED module *****/
