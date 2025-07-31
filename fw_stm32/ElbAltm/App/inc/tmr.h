/*
 * tmr.h
 *
 *  Created on: 18 gen 2021
 *      Author: BaracchiF
 */

#ifndef _TMR_H_
#define _TMR_H_

#include "stm32_type.h"

#define INFINTE_DELAY		0xffffffff

typedef unsigned long	tmr;

void	wait(u32 ms);
void	loop(u32 n);
void	TMR_Start(tmr *tim, u32 set);
void 	TMR_Stop(tmr *tim);
u8 		TMR_GetStatus(tmr *tim);
u8	  TMR_Tick(tmr *tim, u32 set);
u16	  TMR_GetTimersNumbUsed(void);

#define	tmrStart(t, s)		TMR_Start(&(t), (s))
#define	tmrStartSec(t, s)	TMR_Start(&(t), (1000*s))
#define	tmrStartMin(t, s)	TMR_Start(&(t), (60000*s))
#define tmrStop(t)				TMR_Stop(&(t))
#define	tmrStat(t)				TMR_GetStatus(&(t))
#define tmrTick(t, s)			TMR_Tick(&(t), (s))
#define tmrUsed()				  TMR_GetTimersNumbUsed()

#endif /* _TMR_H_ */
