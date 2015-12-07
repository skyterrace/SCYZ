#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define KEY_UP GPIOD,GPIO_Pin_2
#define KEY_DOWN GPIOC,GPIO_Pin_12
#define KEY_LEFT GPIOB,GPIO_Pin_9
#define KEY_RIGHT GPIOB,GPIO_Pin_8
#define KEY_SET GPIOC,GPIO_Pin_11
#define KEY_OK GPIOB,GPIO_Pin_0
#define KEY_STOP GPIOC,GPIO_Pin_4

#define READ_KEY_UP GPIO_ReadInputDataBit(KEY_UP)
#define READ_KEY_DOWN GPIO_ReadInputDataBit(KEY_DOWN)
#define READ_KEY_LEFT GPIO_ReadInputDataBit(KEY_LEFT)
#define READ_KEY_RIGHT GPIO_ReadInputDataBit(KEY_RIGHT)
#define READ_KEY_SET GPIO_ReadInputDataBit(KEY_SET)
#define READ_KEY_OK GPIO_ReadInputDataBit(KEY_OK)
#define READ_KEY_STOP GPIO_ReadInputDataBit(KEY_STOP)

void Key_Config(void);
bool Key_Press(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
bool Key_Release(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

bool key_stop(void);
bool key_man(void);
bool key_set(void);

#endif /* __KEY_H */
