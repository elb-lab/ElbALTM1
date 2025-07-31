#ifndef	__FONT_H__
#define	__FONT_H__

#include "stm32_mcu.h"
#include "stm32_type.h"

typedef struct {
	u16 width;
	u16	height;
	u16 offset;
} FONT_CHAR_INFO;

typedef struct {
	u16 c_height;
	u8	c_start;
	u8	c_end;
	const FONT_CHAR_INFO	*c_descr;
	const u8	*c_bmp;
} FONT_INFO;

// Font data for Lucida Console 10pt
extern const u8 lucidaConsole_10ptBitmaps[];
extern const FONT_INFO lucidaConsole_10ptFontInfo;
extern const FONT_CHAR_INFO lucidaConsole_10ptDescriptors[];


#endif	/* __FONT_H__ */
