#ifndef __R1MT_H
#define __R1MT_H
#include "stm32f10x.h"

//485��д�˿ڶ���
#define USART485_READ GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#define USART485_WRITE GPIO_SetBits(GPIOA,GPIO_Pin_8);


//���Ŷ���
#define CMD_BYTE_LENGTH 6   //ָ���
#define CMD_BYTE_TAR 0 //Ŀ���ַλ
#define CMD_BYTE_SRC 1 //Դ��ַλ
#define CMD_BYTE_TYPE 2 //��������λ
#define CMD_BYTE_PARAM_H 3 //������8λ
#define CMD_BYTE_PARAM_L 4 //������8λ
#define CMD_BYTE_CHK 5 		//У��λ

#define MT_ADDR 0x82   //�˿�ϵͳ��ַ
#define CC_ADDR 0x81    //�ܿ�ϵͳ��ַ
#define BS_ADDR 0x84	//�߽�ϵͳ��ַ
#define SS_ADDR 0x85	//�߽�ϵͳ��ַ

//MT�ٶȿ�������
#define MT_CMD_DRV_LEFT_FWD_SPEED 0x21
#define MT_CMD_DRV_LEFT_REV_SPEED 0x29
#define MT_CMD_DRV_RIGHT_FWD_SPEED 0x22
#define MT_CMD_DRV_RIGHT_REV_SPEED 0x2A
#define MT_CMD_DRV_FWD_SPEED 0x24
#define MT_CMD_DRV_REV_SPEED 0x2C
#define MT_CMD_MOW_SPEED 0x31
#define MT_CMD_DRV_ENABLE 0x32

//MT״̬��������
#define MT_CMD_SET_GEAR_RATIO	0x01
#define MT_CMD_SET_WHEELE_RADIUS	0x02
#define MT_CMD_SET_AXIAL_LENGTH	0x03
#define MT_CMD_SET_FOLLOW_WHEEL_DISTANCE	0x04
#define MT_CMD_SET_MAX_SPEED	0x05
#define MT_CMD_SET_SPEED_STEP	0x06

//MT״̬��ѯ����
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

int8_t SetDriverSpeed(int8_t nLeft,int8_t nRight); //�ٶȷ�Χ(-63~+64)
int8_t SetMowSpeed(uint8_t nSpeed); //���ø�ݵ���ٶȣ��ٶȷ�Χ(0~127)
int8_t EnableDriver(int8_t nOn); //nOn = 1 ������ =0 ֹͣ
int8_t QueryMTStatus(uint8_t cType); //��ѯMT״̬

extern uint8_t cSer_Flag;//���ڽ��յ�Ӧ���ź�
extern uint8_t cmdRec[CMD_BYTE_LENGTH]; //�����������
#endif
