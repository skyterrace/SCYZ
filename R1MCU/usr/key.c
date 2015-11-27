#include "key.h"
#include "R1DEF.h"
#include "DISP.h"
#include "clock_calendar.h"

void Key_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);	
	
  /* K3 配置按键中断线PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	  /* K3 配置按键中断线PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_8|GPIO_Pin_9;// |GPIO_Pin_9|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		  /* K3 配置按键中断线PC3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;// |GPIO_Pin_9|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					    //输入上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

bool Key_Press(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){		  //K3  
	  Delay_us(20000);										  
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){		  //按键消抖动
			return TRUE;
		}                                                                                                                                                                                      
	}
	return FALSE;
}

bool Key_Release(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){		  //K3  
	  Delay_us(20000);										  
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0){		  //按键消抖动
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==0);//是否松开按键	 
			return TRUE;
		}                                                                                                                                                                                      
	}
	return FALSE;
}


bool key_stop()
{
	unsigned char bKeyPress,i;
	bKeyPress = 0x0F;
	for(i=0;i<4;i++)
	{
		if(!READ_KEY_STOP)
		{
			bitclr(bKeyPress,i);
		}
	}
	if(bKeyPress == 0) return TRUE; //顶盖打开
	else return FALSE;
}

bool key_man()
{
	if(Key_Release(KEY_OK))
	{
		sR1Mower.WorkMode++;
		if(sR1Mower.WorkMode>3)sR1Mower.WorkMode=0;
		ShowDefaultScreen(sR1Mower.Power,sR1Mower.WorkMode);
	}

	return ((sR1Mower.WorkMode == 1) ? TRUE : FALSE);
}

bool key_set()
{
	return Key_Release(KEY_SET);
}
