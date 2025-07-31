/*
 * tmr.c
 *
 *  Created on: 18 gen 2021
 *      Author: BaracchiF
 */

/* Include -------------------------------------------------------------------*/

#include <string.h>
#include "ch32v_mcu.h"
#include "tmr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define	TMR_ARRAY_DIM			4		/* dimensione massima array dei timers */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

tmr	*tmrstk[TMR_ARRAY_DIM];	/* stack timers da 1 microsec. */
u32	tmr_wait;								/* timer per ritardo generico */
u32	tmr_loop;								/* looping delay variable */
u16	max_timers = 0;					/* rilevamento nuomero massimo di timer attivi */

/* Private function prototypes -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/*******************************************************************************
* Function Name  : TMR_Init.
* Description    : Set the TIM4 as system tick clock for timer generator.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void TMR_Init(void)
{
	max_timers = 0;
	/* reset vettori */
	memset(tmrstk,0x0,sizeof(tmrstk));

  NVIC_EnableIRQ(SysTicK_IRQn);
  SysTick->SR &= ~(1 << 0);
  SysTick->CMP = (SystemCoreClock) / 1014;
  SysTick->CNT = 0;
  SysTick->CTLR = 0x0F;
}

/*******************************************************************************
* Function Name  : wait
* Description    : Ritardo impostabile con base dei tempi di 1 millisecondo.
* Input          : ms = ritardo (in millisecondi).
* Output         : None.
* Return         : None
*******************************************************************************/

void wait(u32 ms)
{
	for (tmr_wait = ms; tmr_wait > 0; );
} /* WAIT */


/*******************************************************************************
* Function Name  : loop
* Description    : Ritardo impostabile con base dei tempi di 1 ciclo macchina.
* Input          : n = ritardo in cicli macchina.
* Output         : None.
* Return         : None
*******************************************************************************/

void loop(u32 n)
{
	while(--n);
}	/* LOOP */

/*******************************************************************************
* Function Name  : TMR_Irq
* Description    : Aggiorna i valori dei timer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void TMR_Irq(void)		// routine di servizio interrupt (vedi nel file "stm32f10x_it.h"
{
	u16 j, nt;

	if (tmr_wait)	--tmr_wait;			// aggiornam. ritardo generico

	nt = 0;
	/* gestione timers esterni da 1 msec. */
	for (j=0; j<TMR_ARRAY_DIM; j++) {		// legge tutti i record
		if (tmrstk[j] != 0x0) {						// record assegnato a un timer?
			if (--*tmrstk[j] == 0) {					// se il conteggio e' nullo
				tmrstk[j] = 0x0;	// resetta il relativo record
			}
			else
			{
				++nt;		// conteggio timer attivi
			}
		} /* if */
	} /* for */

	if (nt > max_timers)
	{
		max_timers = nt;
	}

} /* sysTmrIntrpt */

/*******************************************************************************
* Function Name  : TMR_Start
* Description    : Gestisce una variabile in RAM come timer.
* Input          : *tim = indirizzo alla variabile utilizzata come timer
* Input          : set  = durata del timer
* Output         : None.
* Return         : None
*******************************************************************************/

void TMR_Start(tmr *tim, u32 set)
{
	s16 j;				// puntatore
	s16	k;

	if (tim == 0x0)		return;

	k = -1;
	/* scansiona lo stack dei timer */
	for (j=0; j<TMR_ARRAY_DIM; j++)
	{
		if (tmrstk[j] == tim)	// se il timer e' gia' attivo
		{
			*tmrstk[j] = set;		// reinposta dall'inizio il tempo
			return;
		}
		else if (tmrstk[j] == 0x0 && k < 0)
		{
			k = j;	// memorizza la prima locazione disponibile
		}
	}

	if (k >= 0)	// se ha trovato una locazione vuota
	{
		tmrstk[k] = tim;	// la assegna al timer
		*tmrstk[k] = set;
	}
	else
	{
		while(1);	// locazioni esaurite!!!
	}

} /* timSetTimer */

u8 TMR_GetStatus(tmr *tim)
{
	u16 j;

	/* cerca il record contenente l'ndirizzo corrispondente */
	/* alla variabile "*tim"								*/
	for (j=0; j<TMR_ARRAY_DIM; j++) {
		if (tmrstk[j] == tim) {	// timer trovato?
			return(1);
		}
	} /* for */
	return(0);
} /* TMR_Status */

void TMR_Stop(tmr *tim)
{
	u16 j;

	for (j=0; j<TMR_ARRAY_DIM; j++) {
		if (tmrstk[j] == tim) {	// timer trovato?
			tmrstk[j] = 0x0;			// disab.il timer
		}
	} /* for */

} /* TMR_Enable */

/*******************************************************************************
* Function Name  : TMR_Tick
* Description    : Genera un impulso ogni 't' millisecondi
* Input          : t = durata impulso
* Output         : None.
* Return         : 1 = impulso di fine timer
*******************************************************************************/

u8	TMR_Tick(tmr *tim, u32 set)
{
	if (*tim == 0)
	{
		TMR_Start(tim, set);
		return 1;
	}

	return 0;
}

/*******************************************************************************
* Function Name  : TMR_GetTimersNumbUsed
* Description    : Restituisce il numero di timers attivi
* Input          : None.
* Output         : None.
* Return         : Numero di timers attivi
*******************************************************************************/

u16	TMR_GetTimersNumbUsed(void)
{
	return max_timers;
}

/***** end of file *****/
