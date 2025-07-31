/******************************************************************
 *	BEEP
 *	data: 04/03/2008
 *	autore: Frank
 *	MODULO GESTIONE SEGNALATORE ACUSTICO
 *	NOTA:
 ******************************************************************/

#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32_type.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum _BEEP_MODE
{
	BEEP_DISABLE = 0, BEEP_LEVEL_1, BEEP_LEVEL_2, BEEP_LEVEL_3
} BEEP_MODE;

/*								frequenza;	volume	*/
typedef struct {	u16	freq;	u16	vol;	}	beep_conf;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void BEEP_Init(void);
void beep(u16 nbeep,u16 lsong,u16 lwait);
void beepSetup(beep_conf cfg);
void beepf(u16 freq, u16 nbeep, u16 lsong, u16 lwait);
void beepMode(BEEP_MODE mode);

#endif	/* _BEEP_H_ */

/**** END OF FILE ****/
