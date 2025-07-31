/**
  ******************************************************************************
  * @file           : usr.c
  * @brief          : User program
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "stm32_mcu.h"
#include "stm32_type.h"
#include "usbd_cdc_if.h"
#include <string.h>
#include "vcp.h"
#include "tmr.h"
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define RX_BUFFER_DIM		1224

/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8	rx_buff[RX_BUFFER_DIM];
u16	rx_pnt, rx_index;
u8	rx_status;
VCP_STATUS	vcp_conn, vcp_conn2;
tmr	tmr_usb_conn;			// time-out usb non collegato

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : VCP_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	VCP_Init(void)
{
	rx_index = rx_pnt = 0;
	rx_status = RX_EMPTY;
	memset(rx_buff, 0, RX_BUFFER_DIM);
	vcp_conn = VCP_NOT_CONNECT;
}

/*******************************************************************************
* Function Name  : VCP_UsbTransfert
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	VCP_UsbTransfert(u8	*buff, u16 len)
{
	for (u16 j=0; j<len; j++)
	{
		rx_buff[rx_index] = buff[j];
		if (++rx_index >= RX_BUFFER_DIM)	
			rx_index = 0;
	}
}

/*******************************************************************************
* Function Name  : VCP_RxCnt
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u16	VCP_RxCnt(void)
{
	if (rx_index >= rx_pnt)
	{
		return (rx_index - rx_pnt);
	}
	
	return (RX_BUFFER_DIM - rx_pnt + rx_index);
	
}

/*******************************************************************************
* Function Name  : VCP_Rx
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u8	VCP_Rx(u8 *c)
{
	if (VCP_RxCnt() == 0)		return(0);
	
	*c = rx_buff[rx_pnt];
	if (++rx_pnt >= RX_BUFFER_DIM)
		rx_pnt = 0;
	
	return(1);
}

/*******************************************************************************
* Function Name  : VCP_RxBuffer
* Description    : read one character from serial port
* Input          : port = port number (1..5)
* Output         : *s = destination buffer pointer
* Return         : return 0 if the buffer is empty; return 1 if the buffer is not empty
*******************************************************************************/

u16	VCP_RxBuffer(u8 *buffer, u16 len)
{
	u16	j;
	u16	n = VCP_RxCnt();
	
	if (n == 0)		return 0;

	if (len > 0 && len < n)		n = len;
		
	j = n;
	do
	{
		VCP_Rx(buffer);
		++buffer;
	} while (--j);
	
	return n;
}

/*******************************************************************************
* Function Name  : VCP_Tx
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	VCP_Tx(u8 c)
{
	while(CDC_Transmit_FS(&c, 1) == USBD_BUSY)
	{
		;
	}	/* while */
}

/*******************************************************************************
* Function Name  : VCP_TxBuffer
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	VCP_TxBuffer(u8 *buff, u16 len)
{
	while(CDC_Transmit_FS(buff, len) == USBD_BUSY)
	{
		;
	}	/* while */
}

/*******************************************************************************
* Function Name  : VCP_Connect
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	VCP_Connect(u8 conn)
{
	if (conn)
		vcp_conn = 1;
	else
		vcp_conn2 = vcp_conn = 0;
}

/*******************************************************************************
* Function Name  : VCP_GetConn
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u8	VCP_GetConn(void)
{
	if (vcp_conn2 != vcp_conn)
	{
		vcp_conn2 = vcp_conn;
		return 1;
	}
	return 0;
}

/*******************************************************************************
* Function Name  : VCP_RxBuffCmp
* Description    : Compare a input string with the data buffer
* Input          : port = port number (1..5)
* 							 : string = pointer to a string
*                  c = character to transmitt
* Output         : None
* Return         : 0 = the strings are equal;
*******************************************************************************/

s16		VCP_RxBuffCmp(u8 *string, u16 len)
{
	u16	j, k;

	for (j=0; j<len; j++)
	{
		k = j + rx_pnt;
		if (k >= RX_BUFFER_DIM)
		{
			k -= RX_BUFFER_DIM;
		}
		if (string[j] != rx_buff[k])
		{
			return 1;
		}
	}

	return 0;
}


/***** end of file *****/
