#ifndef __TFT_H
#define __TFT_H

#define Bank1_LCD_D    ((uint32_t)0x60020000)    //显示区数据地址	  
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //显示区指令地址

#include "stm32f10x_fsmc.h"

void TFT_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);
void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);
void lcd_wr_zf(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 *chr); 
unsigned char *num_pub(unsigned int a);
void TFT_Write_Char(u16 StartX, u16 StartY, u16 X, u16 Y, u16 Color, u8 Dir, u8 chr);
void TFT_ShowText(uint8_t Line,uint8_t Col,uint16_t Color,char *Str);
void TFT_SetCursor(uint8_t Line,uint8_t Col,uint16_t Color);
void TFT_Clear(void);

extern unsigned char zm9[];
extern unsigned char zm8[];
extern unsigned char zm7[];
extern unsigned char zm6[];
extern unsigned char zm5[];
extern unsigned char zm4[];
extern unsigned char zm3[];
extern unsigned char zm2[];
extern unsigned char zm1[];
extern unsigned char zm0[];
extern unsigned char ASCII_Font[];
extern unsigned char ASCII1832[];
// extern unsigned char a1[];

extern unsigned char zf2[];
extern unsigned char zf3[];	
// extern unsigned char pic[];
// extern unsigned char a2[];
// extern unsigned char a3[];

unsigned int LCD_RD_data(void);
void lcd_rst(void);
void Delay(__IO uint32_t nCount);
void FSMC_LCD_Init(void);

#endif /* __TFT_H */
