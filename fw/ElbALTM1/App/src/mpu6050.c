/**
  ******************************************************************************
  * @file           : mpu6050.c
  * @brief          : MPU-6050 driver
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#include "sys.h"
#include "mpu6050.h"
#include "tmr.h"
#include "i2c.h"

/* Private typedef -----------------------------------------------------------*/

typedef enum {
	DERV = 0,	// derivata
	ACCEL,		// integrale x accellerazione
	SPEED			// integrale x velocita'
} MBU_ELAB;

/* Private define ------------------------------------------------------------*/

#define __I2C_WITH_DMA__

#define MPU_I2C_ADDRESS		(u8)0xD0

#define MPU_NR_SAMPLES		10
#define MPU_SAMP_DELAY		100 / MPU_NR_SAMPLES
#define MPU_BUFF_LEN			10

#define DELAY_CHANGE_ALLIGN		3000

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

double	mpu_vel = 0.0;
mpu_struct	mpu_buffer[MPU_BUFF_LEN] = {0};
mpu_struct	mpu_media = {0};
u16					mpu_pnt = 0;
MPU_ALLIGN	mpu_allign = Z_AXIS;
tmr					tmr_allign = 0;
tmr					tmr_vel = 5000;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : MPU_WriteReg
* Description    : Scrittua registro.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u8	MPU_WriteReg(u8 reg, u8 data)
{
	HAL_StatusTypeDef	res;
#ifndef	__I2C_WITH_DMA__
	while ((res = HAL_I2C_Mem_Write(&hi2c2,MPU_I2C_ADDRESS,reg,I2C_MEMADD_SIZE_8BIT,&data,1,1000)) == HAL_BUSY);
#else
	hi2c1.State = HAL_I2C_STATE_READY;
	while((res = HAL_I2C_Mem_Write_DMA(&hi2c2, MPU_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1)) == HAL_BUSY);
#endif
	return (res);
}

/*******************************************************************************
* Function Name  : MPU_ReadBuffer
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
static
u8	MPU_ReadBuffer(u8 reg, u8	*buf, u16 len)
{
	HAL_StatusTypeDef	res;
#ifndef	__I2C_WITH_DMA__
	while ((res = HAL_I2C_Mem_Read(&hi2c2,MPU_I2C_ADDRESS,reg,I2C_MEMADD_SIZE_8BIT,buf,len,1000)) == HAL_BUSY);
#else
	hi2c1.State = HAL_I2C_STATE_READY;
	while ((res = HAL_I2C_Mem_Write_DMA(&hi2c2, MPU_I2C_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, buf, len)) == HAL_BUSY);
#endif
	return (res);

}

/*******************************************************************************
* Function Name  : MPU_Init
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

void	MPU_Init(void)
{
	u32	wait;

	mpu_vel = 0;
	wait = 10000;
	while(--wait);
	//MPU_WriteReg(0x1C,0x08);	// +/-4G range
	MPU_WriteReg(0x6b,0x00);	// wake-up the MPU-6050

	tmrStart(tmr_vel, 5000);
}

/*******************************************************************************
* Function Name  : MPU_ReadData
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u8	MPU_ReadData(mpu_struct *dat)
{
	u8	tdat[14];
	double dv;

	memset(tdat, 0x0, 14);
	if (MPU_ReadBuffer(0x3B, tdat, 14) != 0)	return 1;

	mpu_media.accx -= mpu_buffer[mpu_pnt].accx;
	mpu_buffer[mpu_pnt].accx = (s32)((s16)(tdat[0] << 8 | tdat[1]));
	mpu_media.accx += mpu_buffer[mpu_pnt].accx;
	dat->accx = mpu_media.accx / (s32)MPU_BUFF_LEN;

	mpu_media.accy -= mpu_buffer[mpu_pnt].accy;
	mpu_buffer[mpu_pnt].accy = (s32)((s16)(tdat[2] << 8 | tdat[3]));
	mpu_media.accy += mpu_buffer[mpu_pnt].accy;
	dat->accy = mpu_media.accy / (s32)MPU_BUFF_LEN;

	mpu_media.accz -= mpu_buffer[mpu_pnt].accz;
	mpu_buffer[mpu_pnt].accz = (s32)((s16)(tdat[4] << 8 | tdat[5]));
	mpu_media.accz += mpu_buffer[mpu_pnt].accz;
	dat->accz = mpu_media.accz / (s32)MPU_BUFF_LEN;

	mpu_media.temp -= mpu_buffer[mpu_pnt].temp;
	mpu_buffer[mpu_pnt].temp = (s32)((s16)(tdat[6] << 8 | tdat[7]));
	mpu_media.temp += mpu_buffer[mpu_pnt].temp;
	dat->temp = mpu_media.temp / (s32)MPU_BUFF_LEN;

	mpu_media.gyrox -= mpu_buffer[mpu_pnt].gyrox;
	mpu_buffer[mpu_pnt].gyrox = (s32)((s16)(tdat[8] << 8 | tdat[9]));
	mpu_media.gyrox += mpu_buffer[mpu_pnt].gyrox;
	dat->gyrox = mpu_media.gyrox / (s32)MPU_BUFF_LEN;

	mpu_media.gyroy -= mpu_buffer[mpu_pnt].gyroy;
	mpu_buffer[mpu_pnt].gyroy = (s32)((s16)(tdat[10] << 8 | tdat[11]));
	mpu_media.gyroy += mpu_buffer[mpu_pnt].gyroy;
	dat->gyroy = mpu_media.gyroy / (s32)MPU_BUFF_LEN;

	mpu_media.gyroz -= mpu_buffer[mpu_pnt].gyroz;
	mpu_buffer[mpu_pnt].gyroz = (s32)((s16)(tdat[12] << 8 | tdat[13]));
	mpu_media.gyroz += mpu_buffer[mpu_pnt].gyroz;
	dat->gyroz = mpu_media.gyroz / (s32)MPU_BUFF_LEN;

/*
	dat->accx = (double)((s16)(tdat[0] << 8 | tdat[1]));
	dat->accy = (double)((s16)(tdat[2] << 8 | tdat[3]));
	dat->accz = (double)((s16)(tdat[4] << 8 | tdat[5]));
	dat->temp = (double)((s16)(tdat[6] << 8 | tdat[7]));
	dat->gyrox = (double)((s16)(tdat[8] << 8 | tdat[9]));
	dat->gyroy = (double)((s16)(tdat[10] << 8 | tdat[11]));
	dat->gyroz = (double)((s16)(tdat[12] << 8 | tdat[13]));
*/

	dat->mod = (s16)(sqrt(dat->accx*dat->accx + dat->accy*dat->accy + dat->accz*dat->accz) + 0.5);


	if (mpu_allign == Z_AXIS)
	{
		dv = (double)dat->accy / (double)dat->mod;
		if (dv <= -0.9)
		{
			if (!tmr_allign)
			{
				mpu_allign = Y_AXIS;
				tmr_allign = DELAY_CHANGE_ALLIGN;
			}
		}
		else
		{
			tmrStart(tmr_allign, DELAY_CHANGE_ALLIGN);
		}
	}
	else if (mpu_allign == Y_AXIS)
	{
		dv = (double)dat->accz / (double)dat->mod;
		if (dv >= 0.9)
		{
			if (!tmr_allign)
			{
				mpu_allign = Z_AXIS;
				tmr_allign = DELAY_CHANGE_ALLIGN;
			}
		}
		else
		{
			tmrStart(tmr_allign, DELAY_CHANGE_ALLIGN);
		}
	}

	++mpu_pnt;
	mpu_pnt %= MPU_BUFF_LEN;

	return 0;
}

/*******************************************************************************
* Function Name  : MPU_SpeedCalc
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/
double	vel_deriv;
double	acc;
double	vel;
double	diff;

u16	MPU_SpeedCalc(mpu_struct *dat)
{
	double mod;
	double ax, ay, az;

	/* conversione da livelli in m/s2 */
	ax = ((double)dat->accx * 9.81) / (double)0x2000;
	ay = ((double)dat->accy * 9.81) / (double)0x2000;
	az = ((double)dat->accz * 9.81) / (double)0x2000;
	/* calcolo modulo */
	mod = sqrt(ax*ax + ay*ay + az*az);
	/* cacolo della derivata per eliminare la componente
	 * continua generata dalla forza gravitazionale
	 */
	diff = mod - vel_deriv;
	if (diff > -0.1 && diff < 0.1)
		diff = 0;
	vel_deriv = mod;


	if (tmr_vel == 0)
	{
		/* calcolo integrale dei valori differenziali per ricavare
		 * l'accellerazione pura
		 */
		acc += diff;
		/* calcolo integrale dei valori differenziali per ricavare
		 * la velocita'
		 */
		vel += acc;
		/* cnnversione da m/s2 a km/h */
		mpu_vel = 27.778 * vel;
		if (mpu_vel < 0.0)		mpu_vel = 0.0;
	}
	else
	{
		acc = vel = mpu_vel = 0.0;
	}

	return (u16)(mpu_vel);
}

/*******************************************************************************
* Function Name  : MPU_GetSpeed
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

u16	MPU_GetSpeed(void)
{
	return (u16)mpu_vel;
}

/*******************************************************************************
* Function Name  : MPU_GetAllignAxis
* Description    : None.
* Input          : None.
* Output         : None.
* Return         : None
*******************************************************************************/

MPU_ALLIGN	MPU_GetAllignAxis(void)
{
	return (mpu_allign);
}

/***** end of file *****/
