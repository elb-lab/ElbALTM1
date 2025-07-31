/******************************************************************
*	IRQ
*	data: 20/07/2018
*	autore: FrankOz
*	MODULO DI SET-UP DEL uP ALLO START
*	NOTA:
******************************************************************/

#include "stm32_mcu.h"

/*** dal modulo TIMER.C ***/
extern void	TMR_Irq(void);

/************************************************************
*	Routine di servizio interrupt timer di sistema (tick = 1ms)
************************************************************/

void	HAL_SYSTICK_Callback(void)
{
	TMR_Irq();				// routine delle temporizzazioni
}

/***** end of file ****/
