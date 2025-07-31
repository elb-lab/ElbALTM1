/*
 * ms5611.h
 *
 *  Created on: Jan 28, 2025
 *      Author: BaracchiF
 */

#ifndef _MS5611_H_
#define _MS5611_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Typedef -------------------------------------------------------------------*/

typedef enum ms5611_stat_ {
	MS_OK = 0,
	MS_BUSY,
	MS_ERROR,
} ms5611_stat;

typedef struct ms5611_data_ {
	s32	pres;
	s32 temp;
	s32 altm;
} ms5611_data;

/* Macro ---------------------------------------------------------------------*/
/* Defines -------------------------------------------------------------------*/
/* functions -----------------------------------------------------------------*/

ms5611_stat	MS5611_Init(void);
ms5611_stat	MS5611_ReadData(ms5611_data *data);


#ifdef __cplusplus
}
#endif



#endif /* _MS5611_H_ */
