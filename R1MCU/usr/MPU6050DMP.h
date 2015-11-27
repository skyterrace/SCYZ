#ifndef __MPU6050DMP_H
#define __MPU6050DMP_H

/*��ʹ��ǰ������Ԥ������Ŷ����ж���MPU6050��EMPL_TARGET_STM32*/


#include "stm32f10x.h"
#include "I2CRoutines.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

//****************************************
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	MPU6050_CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	MPU6050_Addr	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
#define MPU6050_I2C I2C1 //MPU6050 I2C�˿�

/* MPU6050 Structure definition */
struct MPU6050_RawData_s
{
  int16_t ax;
  int16_t ay;
  int16_t az;
	int16_t temperature;
  int16_t gx;
  int16_t gy;
  int16_t gz;
};

int stm32_i2c_write(unsigned char slave_addr,
                     unsigned char reg_addr,
                     unsigned char length,
                     unsigned char const *data);
int stm32_i2c_read(unsigned char slave_addr,
                    unsigned char reg_addr,
                    unsigned char length,
                    unsigned char *data);

//��ʱ��ʱ����
/*
��main.c��NVIC_Configuration������
����SysTick_Config(720)��ÿ10us����һ��SysTick
��stm32f10x_it.c�е�SysTick_Handler����SysTick�жϣ�
ִ��TimingDelay_Decrement();
����ÿ100��10usʹTimeStamp_ms��1
*/
// static __IO uint32_t TimingDelay; //SysTick��������
// static __IO uint32_t TimeStamp_ms=0; //�������ֵ

// void TimingDelay_Decrement(void);//��ȡ���ĳ���


// void Delay_us(__IO uint32_t nTime);//��ʱ��ʱ���� 10usΪ��λ
// void Delay_ms(__IO uint32_t nTime);//��ʱ��ʱ���� 1msΪ��λ

int stm32_get_clock_ms(unsigned long *count); //��ȡʱ�Ӻ�����
//��ʱ��ʱ�������


unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx);
unsigned short inv_row_2_scale(const signed char *row);
void mpu6050_run_self_test(void);

//��̬�ṹ����
struct dmpGravity_s
{
  float x;
	float y;
	float z;
};

struct dmpQuaternion_s
{
	float w;
  float x;
	float y;
	float z;
};

struct dmpEuler_s
{
	float psi;
  float theta;
	float phi;
};

struct dmpYawPitchRoll_s
{
	float yaw;
  float pitch;
	float roll;
};

//��̬��ȡ��������
int dmpGetGravity(struct dmpGravity_s *v, struct dmpQuaternion_s *q);
int dmpGetEuler(struct dmpEuler_s *data, struct dmpQuaternion_s *q);
int dmpGetYawPitchRoll(struct dmpYawPitchRoll_s *data, struct dmpQuaternion_s *q, struct dmpGravity_s *gravity);

//Ϊmain��������
int InitMPU6050(void);
uint16_t MPU6050_GetData(uint8_t REG_Address);
int MPU6050_ReadRawData(struct MPU6050_RawData_s *data);
int MPU6050_Read_Ext_Sens_Data(uint8_t RegAddr,uint8_t *buff, uint8_t length);

int MPU6050GetYawPitchRoll(struct dmpYawPitchRoll_s *var);
int InitMPU6050DMP(void);
		
#endif
