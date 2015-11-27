#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "stdarg.h"

void SPI2_Config(void);
unsigned char SPI2_WriteByte(unsigned char data);
void LCD_Config(void);
void WriteLCD(uint8_t start, uint8_t ddata);
void delay(uint16_t n);
void LCD_Show(void);
void LCD_ShowText(uint8_t x,uint8_t y, char *text);
#endif /* __LCD_H */
