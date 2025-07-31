/**
  ******************************************************************************
  * @file           : vcp.c
  * @brief          : USB virtual com port
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __VCP_H__
#define __VCP_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_type.h"

/* Typedef -------------------------------------------------------------------*/

typedef enum {
	VCP_NOT_CONNECT = 0x0,
	VCP_CONNECT
}	VCP_STATUS;

typedef enum {
	RX_EMPTY = 0,
	RX_OK,
}	RX_Status;	

extern VCP_STATUS	vcp_conn;

/* Macro ---------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
   
/* functions -----------------------------------------------------------------*/

void	VCP_Init(void);
u16		VCP_RxCnt(void);
void	VCP_UsbTransfert(u8	*buff, u16 len);
u8		VCP_Rx(u8 *c);
void	VCP_Tx(u8 c);
void	VCP_TxBuffer(u8 *buff, u16 len);
u16		VCP_RxBuffer(u8 *buffer, u16 len);
void	VCP_Connect(u8 conn);
u8		VCP_GetConn(void);
s16		VCP_RxBuffCmp(u8 *string, u16 len);

#ifdef __cplusplus
}
#endif

#endif /* __VCP_H__ */

/**
  * @}
  */ 

/**
* @}
*/ 
/****END OF FILE****/
