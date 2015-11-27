#include "HMC5883.h"
#include "math.h"
#include "R1DEF.h"

uint8_t HMC5883_Buffer_Tx[2];
uint8_t HMC5883_Buffer_Rx[6];

// extern uint8_t IIC_Buffer_Rx[25];

Status InitHMC5883(void)
{
		//�Ĵ���A��D7����Ϊ0��D6D5��ÿ�β��������ѡ�����ƽ������1-8��00=1; 01=2;10=4; 11=8(ȱʡ)��
	//D4D3D2�����������λ000~110�������ߣ�D1D0��������λ��00-������01��ƫѹ��10��ƫѹ
		HMC5883_Buffer_Tx[0]=0x00;
		HMC5883_Buffer_Tx[1]=0x10;
// 	Buffer_Tx1[1]=0x01;  //�Բ���
	  I2C_Master_BufferWrite(HMC5883_I2C, HMC5883_Buffer_Tx,2,Polling, HMC5883_ADDR);
	
			//�Ĵ���B���������ã���3λ��000~111���μ��٣���5λ����Ϊ0
		HMC5883_Buffer_Tx[0]=0x01;
		HMC5883_Buffer_Tx[1]=0x00;
	  I2C_Master_BufferWrite(HMC5883_I2C, HMC5883_Buffer_Tx,2,Polling, HMC5883_ADDR);
	
			//ģʽ�Ĵ�����D7~D2����Ϊ0��D1D0ģʽѡ��00����������01���β�����10��11����
		HMC5883_Buffer_Tx[0]=0x02;
		HMC5883_Buffer_Tx[1]=0x00; //��������
// 	Buffer_Tx1[1]=0x01; //���β���
	  return I2C_Master_BufferWrite(HMC5883_I2C, HMC5883_Buffer_Tx,2,Polling, HMC5883_ADDR);
	
}

Status HMC5883_GetData(struct HMC5883_s *var)
{
// 			uint8_t i;
// 		Buffer_Tx1[0]=0x03;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		x=Buffer_Rx1[0]<<8;
// 		
// 		Buffer_Tx1[0]=0x04;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		x |= Buffer_Rx1[0];

// 		Buffer_Tx1[0]=0x05;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;		
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		z=Buffer_Rx1[0]<<8;

// 		Buffer_Tx1[0]=0x06;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		z |= Buffer_Rx1[0];

// 		Buffer_Tx1[0]=0x07;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;		
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		y=Buffer_Rx1[0]<<8;

// 		Buffer_Tx1[0]=0x08;
// 	  if(I2C_Master_BufferWrite(I2C1, Buffer_Tx1,1,Polling, 0x3C) == Error)
// 			return Error;
// 		I2C_Master_BufferRead(I2C1,Buffer_Rx1,1,Polling, 0x3C);
// 		y |= Buffer_Rx1[0];
// 		
// 		return Success;
		HMC5883_Buffer_Tx[0]=0x03;
	  if(I2C_Master_BufferWrite(HMC5883_I2C, HMC5883_Buffer_Tx,1,Polling, HMC5883_ADDR) == Error)
			return Error;
		if(I2C_Master_BufferRead(HMC5883_I2C,HMC5883_Buffer_Rx,6,DMA, HMC5883_ADDR) == Error)
			return Error;
		

// 		for(i=0;i<6;i++)
// 		{
// 			HMC5883_Buffer_Rx[i] = IIC_Buffer_Rx[i];
// 		}
		
		var->x=HMC5883_Buffer_Rx[0]<<8 | HMC5883_Buffer_Rx[1];
		var->z=HMC5883_Buffer_Rx[2]<<8 | HMC5883_Buffer_Rx[3];
		var->y=HMC5883_Buffer_Rx[4]<<8 | HMC5883_Buffer_Rx[5];
		
		//���㱱���
		var->angle = atan2((double)(var->y),(double)(var->x)) * (180/3.1415926);
// 		if(var->angle>180) var->angle -=360;
		return Success;
}

//��ȡHMC 5883״̬�Ĵ�����
// D1����������Ĵ������档��������������Ĵ����ϵ�һЩ������ȫ�����ݱ�
// ��ȡʱ����λ��λ������λ��λʱ��������������Ĵ������������κ���
// �����ݽ����ᱻ��������Щ�Ĵ����У����Ƿ���������������֮һ�� һ��
// ����6���Ĵ����ѱ���ȡ��ģʽ�ı䣬����ģʽ�����仯�������������÷�
// ���仯��
// D0��׼������λ�������ݶ�д����6�����ݼĴ�������λ��λ����һ���򼸸���
// ��д������Ĵ����Ժ�����װ�ÿ�ʼ����������Ĵ���д������ʱ��λ��
// �������RDYλ������� RDYӦ�������״̬����250΢�롣 DRDY���ſɱ�
// ������Ϊһ�������״̬�Ĵ����ļ��װ��Ϊ�������ݡ�
Status HMC5883_GetStatus(uint8_t *status)
{
		HMC5883_Buffer_Tx[0]=0x09;
	  I2C_Master_BufferWrite(HMC5883_I2C, HMC5883_Buffer_Tx,1,Polling, HMC5883_ADDR);
	if(I2C_Master_BufferRead(HMC5883_I2C,HMC5883_Buffer_Rx,1,Polling, HMC5883_ADDR) == Success)
	{
		*status = HMC5883_Buffer_Rx[0];
		return Success;
	}
	else
	{	
		return Error;
	}
}

#ifdef USE_MPU6050

#else
Status InitMMA7660(void)
{
		HMC5883_Buffer_Tx[0]=0x07;
		HMC5883_Buffer_Tx[1]=0x01;
	  return I2C_Master_BufferWrite(I2C1, HMC5883_Buffer_Tx,2,Polling, 0x98);
}

Status MMA7660_GetData(struct MMA7660_s *var)
{
		float temp;
		HMC5883_Buffer_Tx[0]=0x00;
	  I2C_Master_BufferWrite(I2C1, HMC5883_Buffer_Tx,1,Polling, 0x98);
		if(I2C_Master_BufferRead(I2C1,HMC5883_Buffer_Rx,3,DMA, 0x98) == Error)
			return Error;
		
		var->x = HMC5883_Buffer_Rx[0];
		if(HMC5883_Buffer_Rx[0] & 0x20) var->x = var->x | 0xC0;
		var->y = HMC5883_Buffer_Rx[1];
		if(HMC5883_Buffer_Rx[1] & 0x20) var->y = var->y | 0xC0;
		var->z = HMC5883_Buffer_Rx[2];
		if(HMC5883_Buffer_Rx[2] & 0x20) var->z = var->z | 0xC0;
		
		temp = -(float)(var->x)/0x15;
		if(temp<-1) temp=-1;
		if(temp>1) temp = 1;
		var->PitchAngle = acos(temp) * (180/3.1415926)-90;
		
		temp = -(float)(var->y)/0x15;
		if(temp<-1) temp=-1;
		if(temp>1) temp = 1;
		var->RollAngle = acos(temp) * (180/3.1415926)-90;
		
		temp = -(float)(var->z)/0x15;
		if(temp<-1) temp=-1;
		if(temp>1) temp = 1;
		var->TiltAngle = acos(temp) * (180/3.1415926)-90;
		
		return Success;
}
#endif
