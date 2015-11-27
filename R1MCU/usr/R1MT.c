#include "R1MT.h"
#include "clock_calendar.h"
#include "usart1.h"
#include "R1DEF.h"

uint8_t cmdBuffer[CMD_BYTE_LENGTH];  //�����������
uint8_t cmdRec[CMD_BYTE_LENGTH]; //�����������
uint8_t cSer_Flag; //���ڽ��յ�Ӧ���ź�

//���Ϳ��������֣����յ�Ӧ���źŷ���Ӧ���־����ʱ����-1
int8_t SendCmd(uint8_t * cmd)
{
			uint16_t i;
			//��ʱ
			Delay_us(2);

			//485����ʹ��
			USART485_WRITE;
	
			//����У����
			cmd[CMD_BYTE_LENGTH - 1] = cmd[0];
			for(i=1;i<CMD_BYTE_LENGTH - 1;i++)
			{
				cmd[CMD_BYTE_LENGTH - 1] ^= cmd[i];
			}
			cmd[CMD_BYTE_LENGTH - 1] &= 0x7F;
			
			//���㴮�ڽ��ջ������������״̬λ
// 			U2STAbits.OERR = 0;
			
			//��������
			for(i=0;i<CMD_BYTE_LENGTH;i++)
			{
				USART_SendData(USART1,cmd[i]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
			}
			USART485_READ;
			//�ȴ�����Ӧ���ź�
			i=0xffff;
			while(i>0)
			{
				if(cSer_Flag) //�յ�Ӧ���ź�
				{
					cSer_Flag = 0;
					return cmdRec[CMD_BYTE_PARAM_L]; //����Ӧ���źű�־
				}    
				i--;
			}
			return -1;  //��ʱ����-1
}
//���Ͳ�ѯ�����֣����յ�Ӧ���źŷ���1����ʱ����0
int8_t QueryCmd(uint8_t * cmd)
{
			if( SendCmd(cmd) == -1) return 0;
			else return 1;
}

//�ٶ���������
int8_t SetDriverSpeed(int8_t nLeft,int8_t nRight)
{
	if(nLeft < -63 || nLeft > 64 || nRight < -63 || nRight > 64)
		return -2;

	sR1Mower.LastLeftSpeed = nLeft;
	sR1Mower.LastRightSpeed = nRight;

	cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
	cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
	cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_DRV_FWD_SPEED;
	cmdBuffer[CMD_BYTE_PARAM_H] = nLeft + 63;
	cmdBuffer[CMD_BYTE_PARAM_L] = nRight + 63;
	return SendCmd(cmdBuffer);
}

//����ֹͣ���������nOn=1������nOn=0ֹͣ
int8_t EnableDriver(int8_t nOn)
{
	cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
	cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
	cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_DRV_ENABLE;
	cmdBuffer[CMD_BYTE_PARAM_H] = 0;
	cmdBuffer[CMD_BYTE_PARAM_L] = nOn;

	return SendCmd(cmdBuffer);
}

//��ݵ���ٶ���������
int8_t SetMowSpeed(uint8_t nSpeed)
{
		if(nSpeed > 0x7F) nSpeed = 0x7F;
		cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
		cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
		cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_MOW_SPEED;
		cmdBuffer[CMD_BYTE_PARAM_H] = 0;
		cmdBuffer[CMD_BYTE_PARAM_L] = nSpeed;

		return SendCmd(cmdBuffer);
}

//��ѯMTģ��״̬
int8_t QueryMTStatus(uint8_t cType)
{
	cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
	cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
	cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_QUERY_STATUS;
	cmdBuffer[CMD_BYTE_PARAM_H] = 0x00;
	cmdBuffer[CMD_BYTE_PARAM_L] = cType;
		
	return QueryCmd(cmdBuffer);
}
