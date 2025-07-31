/**
  ******************************************************************************
  * @file           : mpu6050.h
  * @brief          : MPU-6050 driver
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MPU_H__
#define __MPU_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/

typedef enum {
	Z_AXIS = 0,
	Y_AXIS,
	X_AXIS
} MPU_ALLIGN;

typedef struct {
	s32	accx;
	s32	accy;
	s32	accz;
	s32	temp;
	s32	gyrox;
	s32	gyroy;
	s32	gyroz;
	s32 mod;
} mpu_struct;

/* Macro ---------------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* functions -----------------------------------------------------------------*/

void	MPU_Init(void);
u8		MPU_ReadData(mpu_struct *dat);
u16		MPU_GetSpeed(void);
u16		MPU_SpeedCalc(mpu_struct *dat);
MPU_ALLIGN	MPU_GetAllignAxis(void);

#ifdef __cplusplus
}
#endif

#endif /* __MPU_H__ */

/**
  * @}
  */ 

/**
* @}
*/ 
/****END OF FILE****/
