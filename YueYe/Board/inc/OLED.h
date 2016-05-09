/*****************************************************************************/
//文件：OLED.h
/*****************************************************************************/
#ifndef __OLED_H__
#define __OLED_H__

#include "common.h"

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

#define PIN_IN        0
#define PIN_OUT       1


#define White 0
#define Black 1
#define Image_W 80
#define Image_H 60



void OLED_WrDat(unsigned char Data);
void OLED_WrCmd(unsigned char Data);
void OLED_SetPos(unsigned char X, unsigned char Y);
void OLED_Fill(unsigned char ucData);
void SetAddressingMode(unsigned char ucData);
void SetColumnAddress(unsigned char a, unsigned char b);
void SetStartLine(unsigned char ucData);
void SetContrastControl(unsigned char ucData);
void SetChargePump(unsigned char ucData);
void SetSegmentRemap(unsigned char ucData);
void SetEntireDisplay(unsigned char ucData);
void SetInverseDisplay(unsigned char ucData);
void SetMultiplexRatio(unsigned char ucData);
void SetDisplayOnOff(unsigned char ucData);
void SetStartPage(unsigned char ucData);
void SetCommonRemap(unsigned char ucData);
void SetDisplayOffset(unsigned char ucData);
void SetDisplayClock(unsigned char ucData);
void SetPrechargePeriod(unsigned char ucData);
void SetCommonConfig(unsigned char ucData);
void SetVCOMH(unsigned char ucData);
void SetNop(void);
void OLED_init(void);

void OLED_6x8Char(unsigned char X1, unsigned char Y1, unsigned char Data1);
void OLED_6x8Str(unsigned char X1, unsigned char Y1, unsigned char *ucStr,unsigned char state);
void OLED_8x16Char(unsigned char X1, unsigned char Y1, unsigned char Data1);
void OLED_16x16Char(unsigned char X1,unsigned char Y1,unsigned char c[2]);
void OLED_PutString(unsigned char X1,unsigned char Y1,unsigned char *s); 
void OLED_image(void);


//自己加的函数
void OLED_xs_number(unsigned char X1,unsigned char Y1,int num);
void OLED_7number(char String[8],int32 num);
void OLED_3number(char String[5],int16 num);
void OLED_5number(char String[7],int16 num);
void OLED_PrintImage(uint8 picture[Image_H][Image_W]);
void Show_Data(uint8 Mid_farCount,uint8 R_count,uint8 R_Count,int turn_error);

#endif  