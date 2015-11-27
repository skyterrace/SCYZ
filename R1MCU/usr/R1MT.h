#ifndef __R1MT_H
#define __R1MT_H
#include "stm32f10x.h"

//485读写端口定义
#define USART485_READ GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#define USART485_WRITE GPIO_SetBits(GPIOA,GPIO_Pin_8);


//符号定义
#define CMD_BYTE_LENGTH 6   //指令长度
#define CMD_BYTE_TAR 0 //目标地址位
#define CMD_BYTE_SRC 1 //源地址位
#define CMD_BYTE_TYPE 2 //命令类型位
#define CMD_BYTE_PARAM_H 3 //参数高8位
#define CMD_BYTE_PARAM_L 4 //参数低8位
#define CMD_BYTE_CHK 5 		//校验位

#define MT_ADDR 0x82   //运控系统地址
#define CC_ADDR 0x81    //总控系统地址
#define BS_ADDR 0x84	//边界系统地址
#define SS_ADDR 0x85	//边界系统地址

//MT速度控制命令
#define MT_CMD_DRV_LEFT_FWD_SPEED 0x21
#define MT_CMD_DRV_LEFT_REV_SPEED 0x29
#define MT_CMD_DRV_RIGHT_FWD_SPEED 0x22
#define MT_CMD_DRV_RIGHT_REV_SPEED 0x2A
#define MT_CMD_DRV_FWD_SPEED 0x24
#define MT_CMD_DRV_REV_SPEED 0x2C
#define MT_CMD_MOW_SPEED 0x31
#define MT_CMD_DRV_ENABLE 0x32

//MT状态设置命令
#define MT_CMD_SET_GEAR_RATIO	0x01
#define MT_CMD_SET_WHEELE_RADIUS	0x02
#define MT_CMD_SET_AXIAL_LENGTH	0x03
#define MT_CMD_SET_FOLLOW_WHEEL_DISTANCE	0x04
#define MT_CMD_SET_MAX_SPEED	0x05
#define MT_CMD_SET_SPEED_STEP	0x06

//MT状态查询命令
#define MT_CMD_QUERY_STATUS	0x10
#define MT_CMD_QUERY_GEAR_RATIO	0x11
#define MT_CMD_QUERY_WHEELE_RADIUS	0x12
#define MT_CMD_QUERY_AXIAL_LENGTH	0x13
#define MT_CMD_QUERY_FOLLOW_WHEEL_DISTANCE	0x14
#define MT_CMD_QUERY_MAX_SPEED	0x15
#define MT_CMD_QUERY_SPEED	0x16
#define MT_CMD_QUERY_STATUS_VALUE	0x1A

int8_t SendCmd(uint8_t * cmd);
int8_t QueryCmd(uint8_t * cmd);

int8_t SetDriverSpeed(int8_t nLeft,int8_t nRight); //速度范围(-63~+64)
int8_t SetMowSpeed(uint8_t nSpeed); //设置割草电机速度，速度范围(0~127)
int8_t EnableDriver(int8_t nOn); //nOn = 1 启动， =0 停止
int8_t QueryMTStatus(uint8_t cType); //查询MT状态

extern uint8_t cSer_Flag;//串口接收到应答信号
extern uint8_t cmdRec[CMD_BYTE_LENGTH]; //接收命令缓冲区
#endif
