#ifndef __BEHAVIORS_H
#define __BEHAVIORS_H
#include "stm32f10x.h"
//定义状态
#define R1_STOP 0x0001
#define R1_FAIL 0x0002
#define R1_MANUAL 0x0004
#define R1_SETTING 0x0008
#define R1_CHARGING 0x0010
#define R1_IDLE 0x0020
#define R1_GOOUT 0x0040
#define R1_PREPARE 0x0080
#define R1_MOW 0x0100
#define R1_TURN 0x0200
#define R1_AVOID 0x0400
#define R1_LOOK 0x0800
#define R1_FOLLOW 0x1000

#define R1_AUTOWORK (R1_GOOUT|R1_PREPARE|R1_MOW|R1_TURN|R1_AVOID|R1_LOOK|R1_FOLLOW)
//定义状态结束

bool R1_CheckEvent(uint16_t R1Status);
void R1_SwitchToBehavior(uint16_t R1Status);
void R1_ExcecuteActions(void);
void R1_ManualControl(void); //手动控制
void R1_BehaviorTimeElapse(void);

#endif
