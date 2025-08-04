/*
 * io.c
 *
 *  Created on: Oct 30, 2020
 *      Author: BaracchiF
 */


#include <stdlib.h>
#include "sys.h"
#include "main.h"
#include "tmr.h"
#include "io.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define 	DELAY_FILTER			10
#define		dir_logic(data, pin)		(((data) & (1 << pin)) ? ON : OFF)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static pin pin_list[] = {
	{PA5},		// Butt 1
	{PA0},		// Butt 2
	{PA4},		// Butt 3
};

#define INPUT_NR		sizeof(pin_list) / sizeof(pin)

pin_stat	inp[INPUT_NR] = { 0 };

tmr	tmr_filter = 0;
tmr	tmr_repeat = 0;
u32	io_mask, io_change, io_output, io_set;
u32	code = 0;
u32 rept = 0;

/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
* Function Name  : rd_pin
* Description    : Read pin status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/* static
u8 rd_pin(GPIO_TypeDef *port, u16 pin)
{
	return ((port->IDR & pin) ? 1 : 0);
}
 */
/*******************************************************************************
* Function Name  : IO_Init
* Description    : Inizializzazione segnali digitali
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void	IO_Init(void)
{
	io_output = io_mask = io_change = 0x0;
	tmr_filter = DELAY_FILTER;
}

/*******************************************************************************
* Function Name  : IO_Pulses
* Description    : Reset impulsi sui segnali di IO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u8	IO_Service(void)
{
	u8	j;
	u16	tmp;
	u16	re = 0x0;		// buttons rising edge
	u16	fe = 0x0;		// buttons failing edge

	/* lettura input */
	io_mask = 0x0;
  for (j=0; j<INPUT_NR; j++)
  {
    io_mask |= (pin_list[j].GPIOx->IDR & pin_list[j].GPIO_Pin) ? 0x0 : (1 << j);
  }

	if (io_mask != io_change)
	{
		io_change = io_mask;
		tmrStart(tmr_filter, DELAY_FILTER);
	}
	else if ((tmr_filter == 0) && (io_change != io_output))
	{
		tmp = io_output ^ io_change;
		re = io_change & tmp;
		fe = io_output & tmp;
		io_output = io_change;
		tmrStart(tmr_repeat, 2000);
	}
#ifdef __KEY_REPETER_ON
	else if ((io_output & (i_mux[0] | i_mux[2])) && tTick(tmr_repeat, 100) == 0)
	{
		re = io_output;
	}
#endif
	/* generazione dei segnali */
	for (j=0; j<INPUT_NR; j++)
	{
		inp[j].re = dir_logic(re, j);
		inp[j].fe = dir_logic(fe, j);
		inp[j].hl = dir_logic(io_output, j);
		inp[j].ll = dir_logic(~io_output, j);
	}

	return 1;
}

/*******************************************************************************
* Function Name  : IO_GetNr
* Description    : Restituisce il numero di segnali digitali
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u16	IO_GetNr(void)
{
	return (INPUT_NR);
}

/*******************************************************************************
* Function Name  : IO_GetNr
* Description    : Restituisce i segnali digitali degli ingressi
*                  (high level, low level, rising edge, falling edge)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

pin_stat *IO_GetPin(void)
{
	return (inp);
}

/**
 * @brief Restituisce lo stato del pin
 * 
 * @return u32 
 */
u32	IO_GetPinState(void)
{
	return (io_output);
}

/***** end of file *****/
