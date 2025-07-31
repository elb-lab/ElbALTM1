/******************************************************************
 *	BEEP
 *	data: 04/03/2008
 *	autore: Frank
 *	MODULO GESTIONE SEGNALATORE ACUSTICO
 *	NOTA:
 ******************************************************************/

#include "stm32_type.h"
#include "stm32_pin.h"
#include "stm32_mcu.h"
#include "tim.h"
#include "gpio.h"
#include "tmr.h"
#include "beep.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define htim_beep			htim3
#define HTIM_BEEP_CH	TIM_CHANNEL_3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
			
BEEP_MODE	beep_mode = BEEP_LEVEL_1;

const	beep_conf beep_tab[] = {
/*	frequency ,	volume		*/
	{	4000			,	0				},
	{	4000			,	60			},
	{	4000			,	80			},
	{	4000			,	100			},
};

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : BEEP_Init
* Description    : Beeper inizialize
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BEEP_Init(void)
{
	HAL_TIM_PWM_Init(&htim_beep);
	beep_mode = BEEP_LEVEL_3;
	beepSetup(beep_tab[beep_mode]);
}

/*******************************************************************************
* Function Name  : beepSet
* Description    : Beeper inizialize
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void beepMode(BEEP_MODE mode)
{
	beepSetup(beep_tab[mode]);
	beep_mode = mode;
}

/*******************************************************************************
* Function Name  : beep
* Description    : COMANDA IL BUZZER PER GENERARE UN SEGNALE ACUSTICO DI
*                  DURATA IMPOSTABILE
* Input          : - nbeep: numero di impulsi sonori
*                  - lsong: durata impulso sonoro
*                  - lwait: durata pausa
* Output         : None
* Return         : None
*******************************************************************************/
void beep(u16 nbeep,u16 lsong,u16 lwait)
{
	while ((nbeep > 0) && (beep_mode != BEEP_DISABLE))
	{
		HAL_TIM_PWM_Start(&htim_beep, HTIM_BEEP_CH);
		HAL_Delay(lsong);
		HAL_TIM_PWM_Stop(&htim_beep, HTIM_BEEP_CH);
		HAL_Delay(5);
		if (--nbeep)
			HAL_Delay(lwait);	/* durata pausa */
	} /*WHILE*/
} /* BEEP */

/*******************************************************************************
* Function Name  : beepSetup
* Description    : IMPOSTA LA FREQUENZA DELLA NOTA SUL BEEPER
* Input          : frequency = frequenza della nota
* Output         : None
* Return         : None
*******************************************************************************/
void beepSetup(beep_conf config)
{
	u32 period;		// periodo

	HAL_TIM_PWM_Stop(&htim_beep, HTIM_BEEP_CH);
	period = 1000000 / config.freq;
	if (period > 0x0ffff)	period = 0x0ffff;
	htim_beep.Instance->ARR = period;
	htim_beep.Instance->CNT = 0;
	if (config.vol > 100)		config.vol = 100;
	htim_beep.Instance->CCR1 = ((period >> 1) * config.vol) / 100;
}

/*******************************************************************************
* Function Name  : beepf
* Description    : COMANDA IL BUZZER PER GENERARE UN SEGNALE ACUSTICO DI
*                  DURATA E FREQUENZA IMPOSTABILE
* Input          : - nbeep: numero di impulsi sonori
*                  - lsong: durata impulso sonoro
*                  - lwait: durata pausa
*                  - freq : frequenza del suono (tonalit�)
* Output         : None
* Return         : None
*******************************************************************************/
void beepf(u16 freq, u16 nbeep, u16 lsong, u16 lwait)
{
	beep_conf	conf;
	conf.freq = freq;
	conf.vol = beep_tab[beep_mode].vol;
	beepSetup(conf);
	beep(nbeep,lsong,lwait);
	beepSetup(beep_tab[beep_mode]);
}


/**** END OF FILE ****/
