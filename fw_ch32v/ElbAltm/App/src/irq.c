/*
 * irq.c
 *
 *  Created on: May 8, 2023
 *      Author: BaracchiF
 */

/* Includes ------------------------------------------------------------------*/
#include "ch32v00x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/

/*** from TMR.C Module ***/
extern void TMR_Irq(void);
/*** dal modulo LED.C ***/
extern void	LED_Irq(void);

/* Private functions ---------------------------------------------------------*/

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void SysTick_Handler(void)
{
	SysTick->SR = 0x0;
  SysTick->CTLR &= ~0x8000;

	TMR_Irq();				// routine delle temporizzazioni
	LED_Irq();				// interrupt gestione led
} /* SysTick_Handler */


/***** end of IRQ file *****/
