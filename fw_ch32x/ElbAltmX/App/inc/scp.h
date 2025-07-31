/*
 * ucp.h
 *
 *  Created on: 17 dic 2020
 *      Author: BaracchiF
 */

#ifndef __UCP_H__
#define __UCP_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ch32v_mcu.h"

/* Typedef -------------------------------------------------------------------*/

#define __USE_USART1__
//#define __USE_USART2__
//#define __USE_USART3__

typedef enum {
	PORT1 = 1,
	PORT2,
	PORT3,
	PORT4,
	PORT5,
	PORT6
} uart_port;

#ifndef __RX_RES__
typedef enum {
	RX_EMPTY = 0,
	RX_OK,
}	RX_Status;
#define	__RX_RES__
#endif	/* __RX_RES__ */

/* Marco ---------------------------------------------------------------------*/

#define SCP_Tx(v)		SCP_TxBuffer(&(v), 1)

/* Defines -------------------------------------------------------------------*/
/* functions -----------------------------------------------------------------*/

void	SCP_Init(void);
u16		SCP_RxCnt(uart_port port);
RX_Status	SCP_Rx(uart_port port, u8 *c);
void	SCP_TxBuffer(uart_port port, u8 *buff, u16 len);
RX_Status	SCP_RxBuffer(uart_port port, u8 *buff, u16 *len);

#ifdef __cplusplus
}
#endif


#endif /* __UCP_H__ */
