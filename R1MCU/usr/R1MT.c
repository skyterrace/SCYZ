#include "R1MT.h"
#include "clock_calendar.h"
#include "usart1.h"
#include "R1DEF.h"

uint8_t cmdBuffer[CMD_BYTE_LENGTH];  //发送命令缓冲区
uint8_t cmdRec[CMD_BYTE_LENGTH]; //接收命令缓冲区
uint8_t cSer_Flag; //串口接收到应答信号

//发送控制命令字，接收到应到信号返回应答标志，超时返回-1
int8_t SendCmd(uint8_t * cmd)
{
			uint16_t i;
			//延时
			Delay_us(2);

			//485发送使能
			USART485_WRITE;
	
			//计算校验字
			cmd[CMD_BYTE_LENGTH - 1] = cmd[0];
			for(i=1;i<CMD_BYTE_LENGTH - 1;i++)
			{
				cmd[CMD_BYTE_LENGTH - 1] ^= cmd[i];
			}
			cmd[CMD_BYTE_LENGTH - 1] &= 0x7F;
			
			//清零串口接收缓冲区溢出错误状态位
// 			U2STAbits.OERR = 0;
			
			//发送命令
			for(i=0;i<CMD_BYTE_LENGTH;i++)
			{
				USART_SendData(USART1,cmd[i]);
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
			}
			USART485_READ;
			//等待接收应答信号
			i=0xffff;
			while(i>0)
			{
				if(cSer_Flag) //收到应答信号
				{
					cSer_Flag = 0;
					return cmdRec[CMD_BYTE_PARAM_L]; //返回应答信号标志
				}    
				i--;
			}
			return -1;  //超时返回-1
}
//发送查询命令字，接收到应到信号返回1，超时返回0
int8_t QueryCmd(uint8_t * cmd)
{
			if( SendCmd(cmd) == -1) return 0;
			else return 1;
}

//速度设置命令
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

//启动停止电机驱动，nOn=1启动，nOn=0停止
int8_t EnableDriver(int8_t nOn)
{
	cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
	cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
	cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_DRV_ENABLE;
	cmdBuffer[CMD_BYTE_PARAM_H] = 0;
	cmdBuffer[CMD_BYTE_PARAM_L] = nOn;

	return SendCmd(cmdBuffer);
}

//割草电机速度设置命令
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

//查询MT模块状态
int8_t QueryMTStatus(uint8_t cType)
{
	cmdBuffer[CMD_BYTE_TAR] = MT_ADDR;
	cmdBuffer[CMD_BYTE_SRC] = CC_ADDR;
	cmdBuffer[CMD_BYTE_TYPE] = MT_CMD_QUERY_STATUS;
	cmdBuffer[CMD_BYTE_PARAM_H] = 0x00;
	cmdBuffer[CMD_BYTE_PARAM_L] = cType;
		
	return QueryCmd(cmdBuffer);
}
