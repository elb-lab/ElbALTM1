/*
 * atcmd.c
 *
 *  Created on: 13 ott 2020
 *      Author: BaracchiF
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tmr.h"
#include "usr.h"
#include "scp.h"
#include "ms5611.h"
#include "rel.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum {
	NONE = 0,
	SERIAL1,
	SERIAL2,
	SERIAL3,
	USB1 = 0x99,
} PORT_TYPE;


typedef struct {
	u8	*str;
	u8	*(*app)();
} atStruct;


/* Private define ------------------------------------------------------------*/

#define RX_DELAY		5		// ms
#define NR_ELEM			sizeof(at_cmd_list) / sizeof(atStruct)

/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/

static const u8	atstr_ok[] = "OK\n";
//static const u8	atstr_err[] = "ERR\n";
//static const u8	atstr_on[] = "ON\n";
//static const u8	atstr_off[] = "OFF\n";

static const u8	atstr_get_rel[] = "REL?";
static const u8	atstr_get_ok[]	= "OK?";
static const u8	atstr_get_tmr[]	= "TIMERS?";
static const u8	atstr_get_val[]= "VAL?";

/* Private variables ---------------------------------------------------------*/

PORT_TYPE	port;
u8	at_buff[64];
u8	*at_pnt[10];
u16	nr_chr;
tmr	tmr_out;
PORT_TYPE	port = NONE;
u32 gen_value = 0;

/* Extern variables ---------------------------------------------------------*/

extern ms5611_data	atm_data;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : ATCMD_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	ATCMD_Init(void)
{
	nr_chr = 0;
	tmr_out = RX_DELAY;
}

/*******************************************************************************
* Function Name  : outDataString
* Description    : Restiruisce il puntatore alla stringa contenente il dato.
* Input          : *buffer = puntatore al buffer.
* Output         : None.
* Return         : puntatore alla stringa dato/i
*******************************************************************************/

char	*outDataString(u8 *buffer)
{
	char *a, *b, *c;

	c = strchr((char*)(buffer+3), '=');
	a = strchr(c, 0x0a);
	b = strchr(c, 0x0d);
	if (a == 0x0 && b == 0x0)
	{
		return 0x0;
	}
	else if (a == 0x0 && b != 0x0)
	{
		*b = '\0';
	}
	else if (a != 0x0 && b == 0x0)
	{
		*a = '\0';
	}
	else if (a < b)
	{
		*a = '\0';
	}
	else
	{
		*b = '\0';
	}

	return (c + 1);
}

/*******************************************************************************
* Function Name  : outDataString
* Description    : Restiruisce il puntatore alla stringa contenente il dato.
* Input          : *buffer = puntatore al buffer.
* Output         : None.
* Return         : puntatore alla stringa dato/i
*******************************************************************************/

u8	dataSplit(u8 *buffer, u16 buffer_len, u8 **pointers, u8 *number)
{
	u16	n = 0;

	*number = 0;
	for (u16 j=0; j<buffer_len; j++)
	{
		switch (buffer[j])
		{
			case '|':
			case '=':
				buffer[j] = '\0';
				pointers[n] = (u8*)(buffer + j + 1);
				++n;
				break;
			case '\n':
			case '\r':
			case '\0':
				buffer[j] = '\0';
				*number = n;
				return 0;		// ok! fine scansione
		} /* switch */

	}

	return 1;
}

/*******************************************************************************
* Function Name  : cmdRel
*******************************************************************************/
static
u8	*cmdRel(void)
{
	sprintf((char*)at_buff, "%s (v%s - %s build:%s)\n", 
		(char*)rel.fw_name,
		(char*)rel.rel_nr,
		(char*)rel.rel_date,
		(char*)&rel.rel_comp[7]
	);
	gen_value = 0;
	return (at_buff);
}

/*******************************************************************************
* Function Name  : cmdOk
*******************************************************************************/
static
u8	*cmdOk(void)
{
	return ((u8*)atstr_ok);
}

/*******************************************************************************
* Function Name  : cmdOk
*******************************************************************************/
static
u8	*cmdGetTmrUsed(void)
{
	sprintf((char*)at_buff, "%d", tmrUsed());
	return (at_buff);
}

/*******************************************************************************
* Function Name  : cmdOk
*******************************************************************************/
static
u8	*cmdGetValue(void)
{
	sprintf((char*)at_buff, "P=%ld.%02ld T=%ld.%02ld Q=%ld.%02ld\n", 
		atm_data.pres / 100, 	// parte intera della pressione
		atm_data.pres % 100, 	// parte decimale della pressione
		atm_data.temp / 100,	// parte intera della temperatura
		atm_data.temp % 100,	// parte decimale della temperatura
		atm_data.altm / 100,	// quota in metri
		atm_data.altm % 100		// quota in metri	
	);
	return (at_buff);
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************
 *******************************************************************************/

static const atStruct at_cmd_list[] =
{
//	index, string, subroutine
		{ (u8*)atstr_get_rel,	cmdRel, },
		{	(u8*)atstr_get_ok,	cmdOk, },
		{	(u8*)atstr_get_tmr,	cmdGetTmrUsed, },
		{ (u8*)atstr_get_val,	cmdGetValue, },
};

/*******************************************************************************
* Function Name  : ATCMD_Service
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u8	ATCMD_Service(void)
{
	u16	j;			// indice generico
	u16	n;			// numero temp.
	u8	*s;			// puntatore a stringa

	/* controllo ricezione comando */
	if ((n = SCP_RxCnt(PORT1)) != 0)
		port = USB1;
	else
		return 0;

	if (n != nr_chr)			// arrivati nuovi caratteri?
	{
		tmrStart(tmr_out, RX_DELAY);	// avvio attesa di fine ricez.stringa
		nr_chr = n;					// aggiorna il nr. caratteri
		return 0;
	}
	/* attesa fine ricez. dati */
	else if (tmr_out != 0 || nr_chr == 0)
	{
		return 0;
	}

	/* scarico dei ricevuti dati sul buffer */
	if (port == USB1)
		SCP_RxBuffer(PORT1, at_buff, &nr_chr);
	else
		return 0;

	if (at_buff[0] == 'A' && at_buff[1] == 'T' && at_buff[2] == '+' )
	{
		/* riconoscimento comando AT arrivato */
		for (j=0; j<NR_ELEM; j++)
		{
			/* confronta con l'elenco dei comandi previsti */
			n = strlen((char*)at_cmd_list[j].str);
			if (strncmp((char*)at_buff+3, (char*)at_cmd_list[j].str, n) == 0)
			{
				/* comando trovato -> avvio subroutine di gestione comando */
				s = at_cmd_list[j].app();
				n = nr_chr = 0;
				if (s != 0)	// comando riconosciuto
				{
					++gen_value;
					SCP_TxBuffer(PORT1, s, strlen((char*)s));
					return 1;
				}
				break;
			} /* if (strncmp(... */
		} /* ofr (j=0... */
	} /* if (at_buff[0]... */

	n = nr_chr = 0;
	return 0;
}

/***** end of file *****/
