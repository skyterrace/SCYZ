#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H
#include "stm32f10x.h"
#define SYSCLK 48000000  //使用内部时钟，48MHz，在Rcc_Configuration中配置


#define STEP_MOTOR_FREQ 6400  //divided by 100 and save in sR1Mower.sRunParam.BladeSpeed

#define MOTOR_X_PULSE_SET GPIO_SetBits(GPIOC,GPIO_Pin_1);
#define MOTOR_X_PULSE_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_1);

#define MOTOR_X_DIR_FWD GPIO_SetBits(GPIOB,GPIO_Pin_1);
#define MOTOR_X_DIR_REV GPIO_ResetBits(GPIOB,GPIO_Pin_1);

#define MOTOR_Y1_PULSE_SET GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define MOTOR_Y1_PULSE_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_2);

#define MOTOR_Y1_DIR_FWD GPIO_SetBits(GPIOB,GPIO_Pin_2);
#define MOTOR_Y1_DIR_REV GPIO_ResetBits(GPIOB,GPIO_Pin_2);

#define MOTOR_Y2_PULSE_SET GPIO_SetBits(GPIOC,GPIO_Pin_3);
#define MOTOR_Y2_PULSE_RESET GPIO_ResetBits(GPIOC,GPIO_Pin_3);

#define MOTOR_Y2_DIR_FWD GPIO_SetBits(GPIOB,GPIO_Pin_2);
#define MOTOR_Y2_DIR_REV GPIO_ResetBits(GPIOB,GPIO_Pin_2);

//尺寸参数
#define MOTOR_X_GEAR_COUNT 20   //axis x gear count
#define MOTOR_X_GEAR_DIS 8   //unit:mm
#define MOTOR_X_GEAR_RATIO 10

#define MOTOR_Y_GEAR_COUNT 20
#define MOTOR_Y_GEAR_DIS 12.7  //unit:mm
#define MOTOR_Y_GEAR_RATIO 10

#define BOX_X_COUNT 2 //saved in sR1Mower.sRunParam.DrvSpeedL
#define BOX_Y_COUNT 9 //saved in sR1Mower.sRunParam.BsStrenghL
#define BOX_X_LEN 500 //unit:mm, saved in sR1Mower.sRunParam.KP1 = BOX_X_LEN/200.0;
#define BOX_Y_LEN 300 //unit:mm, saved in sR1Mower.sRunParam.KD1 = BOX_Y_LEN/200.0;

extern __IO int32_t sm_loc_x, sm_loc_y;
extern __IO uint32_t sm_loc_x_need, sm_loc_y_need, sm_loc_y1_need, sm_loc_y2_need;
extern __IO uint8_t sm_x_dir,sm_y_dir;
extern __IO uint8_t sm_loc_arrived, sm_box_all_arrived;
extern __IO uint8_t sm_box_loc_x,sm_box_loc_y;

void SM_Enable(unsigned char bEnabled);
void SM_Run(void);
void SM_SetZero(int32_t x,int32_t y);
void SM_Goto_Box(uint8_t box_x,uint8_t box_y);
void SM_Goto_Next(void);
void TIM3_Init(void);
#endif
