//R1ͷ�ļ�

#ifndef R1_MCU_H
#define R1_MCU_H

//λ���ú�λ�������
// #define bitset(var,bitno) ((var) |= (1<<(bitno)))
// #define bitclr(var,bitno) ((var) &= ~(1<<(bitno)))


#define setflag(var,bitno) ((var) |= (bitno))
#define clrflag(var,bitno) ((var) &= ~((bitno)))




//BS��������
#define BS_CMD_SET_SIG_GEN 0x11
#define BS_CMD_SET_RF_ON 0x12

//BS״̬��ѯ����
#define BS_CMD_QUERY_STATUS 0x20
#define BS_CMD_QUERY_STATUS_CH12 0x21
#define BS_CMD_QUERY_STATUS_CH34 0x22
#define BS_CMD_QUERY_STATUS_VALUE 0x41

//SS��������
#define SS_CMD_SET_BAT_LOW 0x11

//SS״̬��ѯ����
#define SS_CMD_QUERY_STATUS 0x20
#define SS_CMD_QUERY_STATUS_VALUE 0x21
#define SS_CMD_QUERY_ERROR_VALUE 0x22
#define SS_CMD_QUERY_ATTITUDE_ROLL 0x23
#define SS_CMD_QUERY_ATTITUDE_HEADING 0x24
#define SS_CMD_QUERY_TEMP 0x25



//MCU״̬����
#define MCU_STA_CHA 0x0001  //16λ״̬��-���
#define MCU_STA_MOW 0x0002	//16λ״̬��-�Զ����
#define MCU_STA_OBS 0x0004	//16λ״̬��-����
#define MCU_STA_TUR 0x0008	//16λ״̬��-��ͷ
#define MCU_STA_LOO 0x0010	//16λ״̬��-Ѱ�ҵ�����
#define MCU_STA_FOL 0x0020	//16λ״̬��-�ص�����
#define MCU_STA_STP 0x0040	//16λ״̬��-ֹͣ
#define MCU_STA_SET 0x0080	//16λ״̬��-����ʱ��͹���ʱ��
#define MCU_STA_MAN 0x0100	//16λ״̬��-�ֶ�
#define MCU_STA_FAL 0x0200	//16λ״̬��-����ֹͣ

#define MOW_SPEED_H 35	//���ٶ�
#define MOW_SPEED_M 25	//���ٶ�
#define MOW_SPEED_L 15	//���ٶ�
#define BLADE_SPEED 110 //��Ƭ�ٶ�

#define BS_STRENGTH_H 80
#define BS_STRENGTH_M 50
#define BS_STRENGTH_L 20

//����״̬
#define FAIL_FLAG_BS_TO 0x0001
#define FAIL_FLAG_SS_TO 0x0002
#define FAIL_FLAG_MT_TO 0x0004
#define FAIL_FLAG_MT_OT 0x0008
#define FAIL_FLAG_MT_OC 0x0010
#define FAIL_FLAG_BS_RF 0x0020 //BS���߲�����
#define FAIL_FLAG_BS_WI 0x0040 //BS���յ��¶�

//��������
#define BAT_LOW_LIMIT 50

//��������
#define ACTION_STOP 0x00 //ֹͣ
#define ACTION_FWD 0x01	//ǰ��
#define ACTION_BACK 0x02	//����
#define ACTION_TL 0x04	//��ת
#define ACTION_TR 0x08	//��ת

//����ΪSTM32�汾�������

#include "stm32f10x.h"
struct Param_s
{
  uint8_t DrvSpeedH;
  uint8_t DrvSpeedM;
  uint8_t DrvSpeedL;
  uint8_t MowSpeed;
  uint8_t BsStrenghH;
  uint8_t BsStrenghM;
	uint8_t BsStrenghL;
	uint8_t PowerLimit;
	float KP1;
	float KD1;
	float KI1;
	float KP2;
	float KD2;
	float KI2;
};
#endif
