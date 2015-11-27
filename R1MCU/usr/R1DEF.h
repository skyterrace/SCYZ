#ifndef __R1DEF_H
#define __R1DEF_H
#include "stm32f10x.h"
#include "HMC5883.h"

#define USE_MPU6050   //�����Ƿ���MPU6050�����б�ǣ�����ʹ��MMA7660��R1Mower_s��YawAngleʹ��HMC5883�ĵشŽǡ�

#define R1_BAT_VOL_AD_RATIO 11 //��ص�ѹAD�ɼ���ѹ��
#define R1_BAT_VOL_MAX 28000 //��ص�ѹ���ֵ����λmV
#define R1_BAT_VOL_MIN 22000 //��ص�ѹ��Сֵ����λmV


//����״̬
#define FAIL_FLAG_BS_TO 0x0001
#define FAIL_FLAG_SS_TO 0x0002
#define FAIL_FLAG_MT_TO 0x0004
#define FAIL_FLAG_MT_OT 0x0008
#define FAIL_FLAG_MT_OC 0x0010
#define FAIL_FLAG_BS_RF 0x0020 //BS���߲�����
#define FAIL_FLAG_BS_WI 0x0040 //BS���յ��¶�

#define FAIL_FLAG_HMC5883_TO 0x0100
#define FAIL_FLAG_MPU6050_TO 0x0080
#define FAIL_FLAG_MMA7660_TO 0x0200

//��������
#define BAT_LOW_LIMIT 50
#define MOW_SPEED_H 35	//���ٶ�
#define MOW_SPEED_M 25	//���ٶ�
#define MOW_SPEED_L 15	//���ٶ�
#define BLADE_SPEED 110 //��Ƭ�ٶ�

#define BS_STRENGTH_H 80
#define BS_STRENGTH_M 50
#define BS_STRENGTH_L 20


#define bitset(var,bitno) ((var) |= (1<<(bitno)))
#define bitclr(var,bitno) ((var) &= ~(1<<(bitno)))

#define setflag(var,bitno) ((var) |= (bitno))
#define clrflag(var,bitno) ((var) &= ~((bitno)))

struct R1_RunParam_s
{
  uint8_t DrvSpeedH;   //����������ٵ�
  uint8_t DrvSpeedM;	//����������ٵ�
  uint8_t DrvSpeedL;	//����������ٵ�
  uint8_t BladeSpeed;	//��ݵ���ٶ�
  uint8_t BsStrenghH;	//�߽��ź�ǿ�ȸ�
  uint8_t BsStrenghM;	//�߽��ź�ǿ����
	uint8_t BsStrenghL;	//�߽��ź�ǿ�ȵ�
	uint8_t PowerLimit;	//���ʣ������ٷֱȱ���ֵ
	float KP1;	//���ٵ���Pֵ
	float KD1;	//���ٵ���Dֵ
	float KI1;	//���ٵ���Iֵ
	float KP2;	//���ٵ���Pֵ
	float KD2;	//���ٵ���Dֵ
	float KI2;	//���ٵ���Iֵ
	uint16_t BehaviorTO;  //��һ��Ϊ��ʱʱ�䣬��λ���룬��������ʱ���������ǰ��Ϊ
	uint16_t MotionSense;  //��ײ�����ȣ���λ����������λmg������ֵ640��
};

struct R1_WorkTime_s
{
	uint8_t StartHour;
	uint8_t StartMin;
	uint8_t EndHour;
	uint8_t EndMin;
};


struct R1Mower_s
{
	uint8_t WorkMode; //��ǰ����ģʽ��0-����Զ���1-�ֶ���2-�ؼң�3-�����Զ�
	uint8_t Power; //ʣ������ٷֱ�
	//uint8_t LCDClosed; //LCD�Ƿ�ر� 1 �رգ�0-��
	int8_t Temperature;//��ǰ�¶ȣ���λ�����϶�
	uint16_t BatVoltage; //��ص�ѹ����λ������
	
	uint8_t MT_STATUS_L; 	//MT״̬λL��
												//D0��ͣ״̬��1Ϊ�棻
												//D1��ͣ״̬��1Ϊ�棻
												//D2��ʾ����״̬��1Ϊ�棻
												//D3��ʾ����״̬��1Ϊ�棻
												//D4��ʾ��������ת��0Ϊ��ת��1Ϊ��ת��
												//D5��ʾ��������ת��0Ϊ��ת��1Ϊ��ת��
												//D6��ʾ�����������״̬��1Ϊ������0Ϊֹͣ��
												//D7=0ʼ��Ϊ0

	uint8_t MT_STATUS_H; 	//MT״̬λH
												// D0δ����
												// D1δ����
												// D2δ����
												// D3δ����
												// D4δ����
												// D5δ����
												// D6��ʾ��ݵ������״̬��1Ϊ������0Ϊֹͣ��
												// D7=0ʼ��Ϊ0

	uint8_t SS_STATUS_L; 	//SS״̬λL
												//D0~D3��ײ��⣺D0ǰ����D1�󷽣�D2�󷽣�D3�ҷ�
												//D4��ʾ���״̬��1Ϊ��
												//D5��ʾ����״̬��1Ϊ��
												//D6��ʾ�г���ѹ״̬��1Ϊ��
	uint8_t SS_STATUS_H; //SS״̬λH
	uint8_t SS_ERROR_L; //SS����λL
											//D0��ʾI2CͨѶ���ϣ�1Ϊ��
											//D1��ʾ���״̬���ϣ�1Ϊ�棨���¶ȱ���������״̬��
											//D2��ʾ�������������ϣ�1Ϊ��
											//D3��ʾ���򴫸������ϣ�1Ϊ��
											//D4��ʾ��ײ���������ϣ�1Ϊ��
											
// 	uint8_t BS_STATUS_L; //BS״̬λL
// 	uint8_t BS_STATUS_H; //BS״̬λH
	uint8_t BS_WI_STATUS; //�߽����״̬��D7~D4������3~0��ͨ�������1������0�Ͽ���D3~D0������3~0�����0����λ��ϵ��0ͬ��1����
	

	uint16_t MCU_STATUS; //MCU16λ״̬��
	uint16_t FAIL_FLAG;  //���ϱ�־,1�й��ϣ�0�޹���
	
	int8_t LeftSpeed, RightSpeed;  //��ǰ�������ٶ�ֵ -63~64
	int8_t LastLeftSpeed, LastRightSpeed;  //��һ���������ٶ�ֵ -63~64
	
	int16_t BSStrength[4]; //ͨ��0~3�ı߽��ź�ǿ��
	int16_t BorderStrength; //�߽�ǿ�ȣ���������ͨ��ǿ�������㣬����UpdateBorderStrength()���������¡�
	
	float RollAngle; //����ǣ���λ����
	float PitchAngle; //�����ǣ���λ����
	float TiltAngle;  //��б�ǣ���λ����
	float YawAngle; //����ǣ���λ����
	
	float TargetAngle; //Ŀ�꺽��ǣ���λ����
	uint8_t TurnDir; //ת����ƣ�D0=0˳ʱ�룬=1��ʱ�룻D1=0 �̶��Ƕ� =1 ����Ƕȣ�D2=0 �̶��Ƕ�180�ȣ�=1�̶��Ƕ�90��
	
	struct R1_RunParam_s sRunParam; //���в���
	struct R1_WorkTime_s sWorkTime[7];  //��һ�����չ���ʱ�䣨ʱ���֣�
	
	struct HMC5883_s sHMC5883;
	struct MMA7660_s sMMA7660;
	
	char * FailInfo; //ʧ����Ϣ
	
};

extern struct R1Mower_s sR1Mower;

void UpdateBorderStrength(void);

void R1_Default(void);
bool R1_is_low_battery(void);
bool R1_has_fail(void);
bool R1_is_working_time(void);
bool R1_is_go_home(void);
bool R1_is_raining(void);
bool R1_is_charging(void);
bool R1_is_full_charging(void);
bool R1_is_crashing(void);
bool R1_is_out_area(void); //�ڱ߽��⣿
bool R1_is_near_middle_line(void);   //R1�Ƿ������߸���
uint8_t R1_is_in_which_area(void); //R1���ĸ���Ȧ�ڣ�����0��1


#endif /* __R1DEF_H */
