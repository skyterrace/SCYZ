#ifndef __ADC_H
#define __ADC_H

#define ADC1_DR_Address    ((u32)0x4001244C)

#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"

void ADC_Configuration(void); 
int8_t GetTemperature(void);
int16_t GetBatVoltage(void);

#endif /* __ADC_H */
