/*
 * ms5611.c
 *
 *  Created on: Jan 28, 2025
 *      Author: BaracchiF
 */
#include <string.h>
#include <math.h>
#include "sys.h"
#include "tmr.h"
#include "ms5611.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum MS_CMD_LIST_ {
	MS_RESET = 0x1E,
	MS_CONV_D1_OSR_256 = 0x40,
	MS_CONV_D1_OSR_512 = 0x42,
	MS_CONV_D1_OSR_1024 = 0x44,
	MS_CONV_D1_OSR_2048 = 0x46,
	MS_CONV_D1_OSR_4096 = 0x48,
	MS_CONV_D2_OSR_256 = 0x50,
	MS_CONV_D2_OSR_512 = 0x52,
	MS_CONV_D2_OSR_1024 = 0x54,
	MS_CONV_D2_OSR_2048 = 0x56,
	MS_CONV_D2_OSR_4096 = 0x58,
	MS_ADC_READ = 0x00,
	MS_COEF_0 = 0xA0,
	MS_COEF_1 = 0xA2,
	MS_COEF_2 = 0xA4,
	MS_COEF_3 = 0xA6,
	MS_COEF_4 = 0xA8,
	MS_COEF_5 = 0xAA,
	MS_COEF_6 = 0xAC,
	MS_COEF_7 = 0xAE,
} MS_CMD_LIST;

typedef struct ms_coef_ {
	u16 Res, C1, C2, C3, C4, C5, C6;
} ms_coef;

/* Private define ------------------------------------------------------------*/

//#define __SPI_MODE__
#define __I2C_MODE__

#ifdef __SPI_MODE__
#define MS_SPI_MOSI_PIN			PC6
#define MS_SPI_MISO_PIN			PC7
#define MS_SPI_SCLK_PIN			PC5
#define MS_SPI_CSEL_PIN			PC1
#define MS_SPI_PORT					SPI1
#elif defined __I2C_MODE__
#define MS_I2C_SDA_PIN			PC1
#define MS_I2C_SCL_PIN			PC2
#define MS_I2C_PORT					I2C1
#endif // __SPI_MODE__


#define	s64									int64_t
#define u64									uint64_t

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms_coef	coef;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __SPI_MODE__

#define __SPI_CS_ENABLE__()			GPIO_WriteBit(MS_SPI_CSEL_PIN, Bit_RESET)
#define __SPI_CS_DISABLE__()		GPIO_WriteBit(MS_SPI_CSEL_PIN, Bit_SET)

/*******************************************************************************
* Function Name  : MS5611_InitConfig
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_InitConfig(void)
{
	GPIO_InitTypeDef	io_param;
	SPI_InitTypeDef		spi_param;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1, ENABLE);

	/* I/O Configurations */
	/* MOSI & SCLK Pins */
	GPIO_WriteBit(MS_SPI_MOSI_PIN, Bit_RESET);
	GPIO_WriteBit(MS_SPI_SCLK_PIN, Bit_RESET);
	io_param.GPIO_Mode = GPIO_Mode_AF_PP;
	io_param.GPIO_Speed = GPIO_Speed_30MHz;
	io_param.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOC, &io_param);
	/* NSS Pin */
	__SPI_CS_DISABLE__();
	io_param.GPIO_Mode = GPIO_Mode_Out_PP;
	io_param.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &io_param);
	/* MISO Pin */
	io_param.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	io_param.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &io_param);

	/* SPI Configuration */
	spi_param.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	spi_param.SPI_CPHA = SPI_CPHA_1Edge;
	spi_param.SPI_CPOL = SPI_CPOL_Low;
	spi_param.SPI_CRCPolynomial = 7;
	spi_param.SPI_DataSize = SPI_DataSize_8b;
	spi_param.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi_param.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_param.SPI_Mode = SPI_Mode_Master;
	spi_param.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(MS_SPI_PORT, &spi_param);

	SPI_Cmd(MS_SPI_PORT, ENABLE);

}

/*******************************************************************************
* Function Name  : MS5611_SendCmd
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_SendCmd(u8 cmd)
{
	__SPI_CS_ENABLE__();
	SPI_I2S_SendData(MS_SPI_PORT, (u16)cmd);
  while(SPI_I2S_GetFlagStatus(MS_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
	wait(10);
  __SPI_CS_DISABLE__();
}

/*******************************************************************************
* Function Name  : MS5611_ReadADC
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_ReadADC(u32 *data)
{
	u8 rx_buff[4];
  __SPI_CS_ENABLE__();
	memset(rx_buff, 0x0, 4);
	for (u8 j=0; j<4; j++)
	{
		SPI_I2S_SendData(MS_SPI_PORT, (u16)MS_ADC_READ);
	  while(SPI_I2S_GetFlagStatus(MS_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
		while(SPI_I2S_GetFlagStatus(MS_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
		rx_buff[j] = SPI_I2S_ReceiveData(MS_SPI_PORT);
	}
	*data = (u16)rx_buff[1] << 16 | (u16)rx_buff[2] << 8 | (u16)rx_buff[3];
  __SPI_CS_DISABLE__();
}

/*******************************************************************************
* Function Name  : MS5611_ReadRegister
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_ReadRegister(u8 cmd, u16 *data)
{
	u8 tx_buff[3], rx_buff[3];
	tx_buff[0] = cmd;	tx_buff[1] = tx_buff[2] = 0;
  __SPI_CS_ENABLE__();
	for (u8 j=0; j<3; j++)
	{
		SPI_I2S_SendData(MS_SPI_PORT, (u16)tx_buff[j]);
	  while(SPI_I2S_GetFlagStatus(MS_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
		while(SPI_I2S_GetFlagStatus(MS_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
		rx_buff[j] = SPI_I2S_ReceiveData(MS_SPI_PORT);
	}
	*data = (u16)rx_buff[1] << 8 | (u16)rx_buff[2];
  __SPI_CS_DISABLE__();
}

#elif defined __I2C_MODE__

#define __I2C_ADDR__					0xEE

/*******************************************************************************
* Function Name  : MS5611_InitConfig
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_InitConfig(void)
{
	GPIO_InitTypeDef	io_param;
	I2C_InitTypeDef		i2c_param;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* I/O Configurations */
	io_param.GPIO_Mode = GPIO_Mode_AF_OD;
	io_param.GPIO_Speed = GPIO_Speed_30MHz;
	io_param.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Init(GPIOC, &io_param);

	i2c_param.I2C_Ack = I2C_Ack_Enable;
	i2c_param.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_param.I2C_ClockSpeed = 100000;
	i2c_param.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_param.I2C_Mode = I2C_Mode_I2C;
	i2c_param.I2C_OwnAddress1 = __I2C_ADDR__;
	I2C_Init(MS_I2C_PORT, &i2c_param);
  I2C_Cmd(MS_I2C_PORT, ENABLE);
  I2C_AcknowledgeConfig(MS_I2C_PORT, ENABLE);

}

/*******************************************************************************
* Function Name  : MS5611_SendCmd
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_SendCmd(u8 cmd)
{
  while(I2C_GetFlagStatus(MS_I2C_PORT, I2C_FLAG_BUSY ) != RESET);

  I2C_GenerateSTART(MS_I2C_PORT, ENABLE);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(MS_I2C_PORT, __I2C_ADDR__, I2C_Direction_Transmitter);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(MS_I2C_PORT, cmd);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTOP(MS_I2C_PORT, ENABLE);

	wait(10);
}

/*******************************************************************************
* Function Name  : MS5611_ReadADC
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_ReadADC(u32 *data)
{
	u8 buff[4];
	memset(buff, 0x0, 4);
	buff[0] = MS_ADC_READ;

  while(I2C_GetFlagStatus(MS_I2C_PORT, I2C_FLAG_BUSY ) != RESET);

  I2C_GenerateSTART(MS_I2C_PORT, ENABLE);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(MS_I2C_PORT, __I2C_ADDR__, I2C_Direction_Transmitter);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(MS_I2C_PORT, MS_ADC_READ);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(MS_I2C_PORT, ENABLE);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(MS_I2C_PORT, __I2C_ADDR__, I2C_Direction_Receiver);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	for (u16 j=0; j<3; j++)
	{
		buff[j] = I2C_ReceiveData(MS_I2C_PORT);
	}

	I2C_GenerateSTOP(MS_I2C_PORT, ENABLE);

	*data = (u16)buff[0] << 16 | (u16)buff[1] << 8 | (u16)buff[2];
}

/*******************************************************************************
* Function Name  : MS5611_ReadRegister
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
void	MS5611_ReadRegister(u8 cmd, u16 *data)
{
	u8 buff[2];
	buff[0] = cmd;

  while(I2C_GetFlagStatus(MS_I2C_PORT, I2C_FLAG_BUSY ) != RESET);

  I2C_GenerateSTART(MS_I2C_PORT, ENABLE);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(MS_I2C_PORT, __I2C_ADDR__, I2C_Direction_Transmitter);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(MS_I2C_PORT, cmd);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(MS_I2C_PORT, ENABLE);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(MS_I2C_PORT, __I2C_ADDR__, I2C_Direction_Receiver);

  while(!I2C_CheckEvent(MS_I2C_PORT, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	for (u16 j=0; j<2; j++)
	{
		buff[j] = I2C_ReceiveData(MS_I2C_PORT);
	}

	I2C_GenerateSTOP(MS_I2C_PORT, ENABLE);

	*data = (u16)buff[0] << 8 | (u16)buff[1];
}


#endif /* __SPI_MODE__ */

/*******************************************************************************
* Function Name  : MS5611_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

ms5611_stat	MS5611_Init(void)
{
	MS5611_InitConfig();

	/* Device Reset */
	MS5611_SendCmd(MS_RESET);

	/* Coeficients Reading */
	MS5611_ReadRegister(MS_COEF_1, &coef.C1);
	MS5611_ReadRegister(MS_COEF_2, &coef.C2);
	MS5611_ReadRegister(MS_COEF_3, &coef.C3);
	MS5611_ReadRegister(MS_COEF_4, &coef.C4);
	MS5611_ReadRegister(MS_COEF_5, &coef.C5);
	MS5611_ReadRegister(MS_COEF_6, &coef.C6);

	return MS_OK;
}

/*******************************************************************************
* Function Name  : MS5611_ReadData
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

ms5611_stat	MS5611_ReadData(ms5611_data *data)
{
	u32 tmp;
	MS5611_SendCmd(MS_CONV_D1_OSR_4096);
	MS5611_ReadADC(&tmp);
	data->pres = tmp;
	MS5611_SendCmd(MS_CONV_D2_OSR_4096);
	MS5611_ReadADC(&tmp);

	/* Calcolo temperatura */
	s32 dT = tmp - ((u32)coef.C5 << 8);
	data->temp = 2000 + ((s64)dT * coef.C6) / 8388608;

	/* Calcolo Pressione atmosferica */
	s64 OFF = ((s64)coef.C2 << 16) + ((s64)coef.C4 * dT) / 128;
	s64 SENS = ((s64)coef.C1 << 15) + ((s64)coef.C3 * dT) / 256;
//	data->pres = (((s64)data->pres * SENS) / 2097152 - OFF) / 32768;
	
	/* compensazione in temperatura di 2o ordine */
	if (data->temp < 2000) {
		s64 T2 = (dT * dT) / 2147483648;
		s64 OFF2 = 5 * (data->temp - 2000) * (data->temp - 2000) / 2;
		s64 SENS2 = 5 * (data->temp - 2000) * (data->temp - 2000) / 4;
		if (data->temp < -1500) {
			OFF2 = OFF2 + 7 * (data->temp + 1500) * (data->temp + 1500);
			SENS2 = SENS2 + 11 * (data->temp + 1500) * (data->temp + 1500) / 2;
		}
		data->temp = data->temp - T2;
		OFF = OFF - OFF2;
		SENS = SENS - SENS2;
	}
	data->pres = (((s64)data->pres * SENS) / 2097152 - OFF) / 32768;

	/* Calcolo Altimetrico */
	//data->altm = 13512 * (1 - pow((data->pres / 101325), 0.1903));

	return MS_OK;
}

/***** end of file *****/
