/*
 * ms5611.c
 *
 *  Created on: Jan 28, 2025
 *      Author: BaracchiF
 */
#include <string.h>
#include <math.h>
#include "sys.h"
#include "spi.h"
#include "i2c.h"
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

#define __SPI_MODE__
//#define __I2C_MODE__

#ifdef __SPI_MODE__
#define MS_SPI_MOSI_PIN			PA2
#define MS_SPI_MISO_PIN			PA6
#define MS_SPI_SCLK_PIN			PA1
#define MS_SPI_CSEL_PIN			PA8
#define MS_SPI_PORT					hspi1
#elif defined __I2C_MODE__
#define MS_I2C_SDA_PIN			PA6
#define MS_I2C_SCL_PIN			PA7
#define MS_I2C_PORT					hi2c2
#endif // __SPI_MODE__

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

ms_coef	coef;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __SPI_MODE__

#define __SPI_CS_ENABLE__()			HAL_GPIO_WritePin(MS_SPI_CSEL_PIN, GPIO_PIN_RESET)
#define __SPI_CS_DISABLE__()		HAL_GPIO_WritePin(MS_SPI_CSEL_PIN, GPIO_PIN_SET)

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
	__NOP();
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
	while (HAL_SPI_Transmit(&MS_SPI_PORT, &cmd, 1, 1000) != HAL_OK);
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
	u8 tx_buff[4], rx_buff[4];
  __SPI_CS_ENABLE__();
	memset(tx_buff, 0x0, 4);
	memset(rx_buff, 0x0, 4);
	while(HAL_SPI_TransmitReceive(&MS_SPI_PORT, tx_buff, rx_buff, 4, 1000) != HAL_OK);
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
	while(HAL_SPI_TransmitReceive(&MS_SPI_PORT, tx_buff, rx_buff, 3, 1000) != HAL_OK);
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
	__NOP();
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
	while((HAL_I2C_Master_Transmit(&MS_I2C_PORT, __I2C_ADDR__, &cmd, 1, 1000) == HAL_BUSY));
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
	while(HAL_I2C_Master_Transmit(&MS_I2C_PORT, __I2C_ADDR__, buff, 1, 1000) == HAL_BUSY);
	while(HAL_I2C_Master_Receive(&MS_I2C_PORT, __I2C_ADDR__, buff, 3, 1000) == HAL_BUSY);
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
	HAL_I2C_Master_Transmit(&MS_I2C_PORT, __I2C_ADDR__, buff, 1, 1000);
	HAL_I2C_Master_Receive(&MS_I2C_PORT, __I2C_ADDR__, buff, 2, 1000);
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
	data->altm = (s32)(12700.0 * (1.0 - pow(((double)data->pres / 101325.0), 0.1903)) * 100.0);

	return MS_OK;
}

/***** end of file *****/
