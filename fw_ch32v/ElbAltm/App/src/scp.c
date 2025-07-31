/*
 * ucp.c
 *
 *  Created on: 17 dic 2020
 *      Author: BaracchiF
 */

/* Includes ------------------------------------------------------------------*/

#include "sys.h"
#include <string.h>
#include "scp.h"

/* Private typedef -----------------------------------------------------------*/

typedef struct {
	USART_TypeDef	*usart;
	DMA_Channel_TypeDef	*dma_rx;
	DMA_Channel_TypeDef	*dma_tx;
	u16	index;
	u16	pnt;
	RX_Status	status;
	u8	*buffer;
	u16	len_buffer;
} uart_data;

/* Private define ------------------------------------------------------------*/

#define RX_BUFFER_DIM		64
#define	H_SCP_PORT3			huart3
#define	H_SCP_PORT2			huart2

/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#ifdef	__USE_USART1__
#define	RX1_BUFFER_DIM		64
u8	rx1_buffer[RX1_BUFFER_DIM];
#endif
#ifdef	__USE_USART2__
#define	RX2_BUFFER_DIM		64
u8	rx2_buffer[RX2_BUFFER_DIM];
#endif
#ifdef	__USE_USART3__
#define	RX3_BUFFER_DIM		64
u8	rx3_buffer[RX3_BUFFER_DIM];
#endif
#ifdef	__USE_USART4__
#define	RX4_BUFFER_DIM		64
u8	rx4_buffer[RX4_BUFFER_DIM];
#endif
#ifdef	__USE_USART5__
#define	RX5_BUFFER_DIM		64
u8	rx5_buffer[RX5_BUFFER_DIM];
#endif
#ifdef	__USE_USART6__
#define	RX6_BUFFER_DIM		64
u8	rx6_buffer[RX6_BUFFER_DIM];
#endif

uart_data	rx[6] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SCP_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	SCP_Init(void)
{
	GPIO_InitTypeDef	io_param;
	USART_InitTypeDef	usart_param;
	DMA_InitTypeDef		dma_param;

#ifdef __USE_USART1__
	rx[PORT1].status = RX_EMPTY;
	rx[PORT1].buffer = rx1_buffer;
	rx[PORT1].usart	= USART1;
	rx[PORT1].dma_rx = DMA1_Channel5;
	rx[PORT1].dma_tx = DMA1_Channel4;
	rx[PORT1].len_buffer = RX1_BUFFER_DIM;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* USART pins configurations */
	io_param.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	io_param.GPIO_Pin = GPIO_Pin_6;
	io_param.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &io_param);
	io_param.GPIO_Mode = GPIO_Mode_AF_PP;
	io_param.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &io_param);

	/* USART Peripheral Configuration */
	usart_param.USART_BaudRate = 115200;
	usart_param.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_param.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_param.USART_Parity = USART_Parity_No;
	usart_param.USART_StopBits = USART_StopBits_1;
	usart_param.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart_param);

	/* DMA1 Ch4 -> USART1-TX Config. */
	dma_param.DMA_BufferSize = RX1_BUFFER_DIM;
	dma_param.DMA_DIR = DMA_DIR_PeripheralDST;
	dma_param.DMA_M2M = DMA_M2M_Disable;
	dma_param.DMA_MemoryBaseAddr = (u32)rx[PORT1].buffer;
	dma_param.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_param.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_param.DMA_Mode = DMA_Mode_Normal;
	dma_param.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
	dma_param.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_param.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_param.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel4, &dma_param);
  DMA_Cmd(DMA1_Channel4, ENABLE); /* USART1 Tx */

	/* DMA1 Ch5 -> USART1-RX Config. */
	dma_param.DMA_BufferSize = RX1_BUFFER_DIM;
	dma_param.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma_param.DMA_M2M = DMA_M2M_Disable;
	dma_param.DMA_MemoryBaseAddr = (u32)rx[PORT1].buffer;
	dma_param.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma_param.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_param.DMA_Mode = DMA_Mode_Circular;
	dma_param.DMA_PeripheralBaseAddr = (u32)(&USART1->DATAR);
	dma_param.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma_param.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_param.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel5, &dma_param);
  DMA_Cmd(DMA1_Channel5, ENABLE); /* USART1 Rx */

  USART_Cmd(USART1, ENABLE);
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

#endif
#ifdef __USE_USART2__
	rx[PORT2].status = RX_EMPTY;
	rx[PORT2].buffer = rx2_buffer;
	rx[PORT2].huart	= &huart2;
	rx[PORT2].len_buffer = RX2_BUFFER_DIM;
	HAL_UART_Receive_DMA(&huart2, rx2_buffer, RX2_BUFFER_DIM);
#endif
#ifdef __USE_USART3__
	rx[PORT3].status = RX_EMPTY;
	rx[PORT3].buffer = rx3_buffer;
	rx[PORT3].huart	= &huart3;
	rx[PORT3].len_buffer = RX3_BUFFER_DIM;
	HAL_UART_Receive_DMA(&huart3, rx3_buffer, RX3_BUFFER_DIM);
#endif
#ifdef __USE_USART4__
	rx[PORT4].status = RX_EMPTY;
	rx[PORT4].buffer = rx4_buffer;
	rx[PORT4].huart	= &huart4;
	rx[PORT4].len_buffer = RX4_BUFFER_DIM;
	HAL_UART_Receive_DMA(&huart4, rx4_buffer, RX4_BUFFER_DIM);
#endif
#ifdef __USE_USART5__
	rx[PORT5].status = RX_EMPTY;
	rx[PORT5].buffer = rx5_buffer;
	rx[PORT5].huart	= &huart5;
	rx[PORT5].len_buffer = RX5_BUFFER_DIM;
	HAL_UART_Receive_DMA(&huart5, rx5_buffer, RX5_BUFFER_DIM);
#endif
#ifdef __USE_USART6__
	rx[PORT6].status = RX_EMPTY;
	rx[PORT6].buffer = rx6_buffer;
	rx[PORT6].huart	= &huart6;
	rx[PORT6].len_buffer = RX6_BUFFER_DIM;
	HAL_UART_Receive_DMA(&huart6, rx6_buffer, RX6_BUFFER_DIM);
#endif

}

/*******************************************************************************
* Function Name  : SCP_RxCnt
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u16	SCP_RxCnt(uart_port port)
{
	if (rx[port].usart == 0x0)	return 0;

	rx[port].index = rx[port].len_buffer - rx[port].dma_rx->CNTR;

	if (rx[port].index >= rx[port].pnt)
	{
		return (rx[port].index - rx[port].pnt);
	}

	return (rx[port].len_buffer - rx[port].pnt + rx[port].index);
}

/*******************************************************************************
* Function Name  : SCP_Rx
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

RX_Status	SCP_Rx(uart_port port, u8 *c)
{
	if (SCP_RxCnt(port) == 0)		return(RX_EMPTY);

	*c = rx[port].buffer[rx[port].pnt];
	if (++rx[port].pnt >= rx[port].len_buffer)
		rx[port].pnt = 0;

	return(RX_OK);
}

/*******************************************************************************
* Function Name  : SCP_Rx
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

RX_Status	SCP_RxBuffer(uart_port port, u8 *buff, u16 *len)
{
	u16	j;

	*len = SCP_RxCnt(port);
	if (*len == 0)		return(RX_EMPTY);

	for (j=0; j<*len; j++)
	{
		buff[j] = rx[port].buffer[rx[port].pnt];
		if (++rx[port].pnt >= rx[port].len_buffer)
			rx[port].pnt = 0;
	}
	return(RX_OK);
}

/*******************************************************************************
* Function Name  : SCP_TxBuffer
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	SCP_TxBuffer(uart_port port, u8 *buff, u16 len)
{
	if (rx[port].usart == 0x0)	return;

	rx[port].dma_tx->MADDR = (u32)buff;
	rx[port].dma_tx->CNTR = len;
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	while(rx[port].dma_tx->CNTR != 0);
	while((rx[port].usart->STATR & USART_STATR_TC) == 0);

}

/***** end of file *****/
