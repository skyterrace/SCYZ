#include "stm32f10x.h"
#include "misc.h"
#include "eeprom.h"
#include "clock_calendar.h"
#include "key.h"
#include "string.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "R1DEF.h"
#include "math.h"
#include "behaviors.h"
#include "stdlib.h"
#include "stm32f10x_tim.h"
#include "stepmotor.h"

#define DEBUG_MODE


/*系统配置变量和函数定义*/

void RCC_Configuration(void); //系统时钟配置
void NVIC_Configuration(void); //中断配置
void GPIO_Config(void); //GPIO配置

/*系统配置定义结束*/

/*定时和节拍变量和函数定义*/
uint8_t TimeSecTick=0; //秒中断标志，=1则秒中断发生
/*定时和节拍变量和函数定义结束*/

/*串口操作变量和函数定义*/

// uint8_t TxBuffer1[30], RxBuffer1[30]; //串口1缓冲区
// uint8_t rec_f; //串口1接收标志，接收到数据则为1

/*串口操作变量和函数定义结束*/


/*其他变量和函数定义*/



//extern char strbuff[]; //显示缓冲区，在DISP.c中定义

/*其他变量和函数定义结束*/

void TIM3_Init(void);

int main(void)
{
	RCC_Configuration(); 
	GPIO_Config();
	NVIC_Configuration();
	
	Lcd_Init();
	Lcd_Clear(GRAY0);
	Key_Config();

	//看门狗初始化
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)  //如果上次的复位是看门复位的话
	{
// 		SetMowSpeed(0);
// 		SetDriverSpeed(0,0);
		//ShowStatusInfo(3,0,"Watch Dog!");
		Gui_DrawFont_GBK32(16,6,BLUE,GRAY0,"watch dog");  //显示
		//RCC_ClearFlag(); //后面RTC_Configuration里会清寄存器
		//while(!Key_Press(KEY_OK));
		Delay_ms(3000);
	}
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	/* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
	IWDG_SetReload(781);
	
	/* 喂狗*/
	IWDG_ReloadCounter();
	/* 使能狗狗*/
 	IWDG_Enable();
	
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  /* EEPROM Init */
  EE_Init();
	R1_Default();
	RTC_Configuration();  //如果没有配置过时钟，会将R1 Default值保存到EERom里，所以要放在R1_Default函数后面。
	EE_LoadWorkingTime();
	EE_LoadParams();


	TimeSecTick = 0;
	TIM3_Init();
	
	SM_Enable(0);
	SM_SetZero(0,0);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);   //清除投饵继电器
	sm_box_all_arrived = 0;
	
	Gui_DrawDefaultScreen(sR1Mower.WorkMode);  //显示

	while(1)
	{
	
		IWDG_ReloadCounter(); //喂狗
		
		
		//调试代码
// 		Delay_ms(1000);
// 		if(Key_Press(KEY_OK)) sR1Mower.BSStrength[0] = sR1Mower.BSStrength[1] = 30;
// 		else sR1Mower.BSStrength[0] = sR1Mower.BSStrength[1] = 1;
		
		//调试代码结束
		
// 		if(sR1Mower.WorkMode == 2)
// 		{
// 			GPIO_SetBits(GPIOA,GPIO_Pin_3);
// 		}
// 		else
// 		{
// 			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
// 		}
		//在C0上输出一个方波，代表运行周期
// 		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0) == 0) GPIO_SetBits(GPIOC,GPIO_Pin_0);
// 		else GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		
		if(R1_CheckEvent(R1_STOP)) R1_SwitchToBehavior(R1_STOP);
		if(R1_CheckEvent(R1_FAIL)) R1_SwitchToBehavior(R1_FAIL);
		if(R1_CheckEvent(R1_MANUAL)) R1_SwitchToBehavior(R1_MANUAL);
		if(R1_CheckEvent(R1_SETTING)) R1_SwitchToBehavior(R1_SETTING);
		if(R1_CheckEvent(R1_CHARGING)) R1_SwitchToBehavior(R1_CHARGING);
		if(R1_CheckEvent(R1_IDLE)) R1_SwitchToBehavior(R1_IDLE);
		if(R1_CheckEvent(R1_GOOUT)) R1_SwitchToBehavior(R1_GOOUT);
		if(R1_CheckEvent(R1_PREPARE)) R1_SwitchToBehavior(R1_PREPARE);
		if(R1_CheckEvent(R1_MOW)) R1_SwitchToBehavior(R1_MOW);
		if(R1_CheckEvent(R1_TURN)) R1_SwitchToBehavior(R1_TURN);
		if(R1_CheckEvent(R1_AVOID)) R1_SwitchToBehavior(R1_AVOID);
		if(R1_CheckEvent(R1_LOOK)) R1_SwitchToBehavior(R1_LOOK);
		if(R1_CheckEvent(R1_FOLLOW)) R1_SwitchToBehavior(R1_FOLLOW);
		
		R1_ExcecuteActions();
		
		

 		if(TimeSecTick)
		{
			TimeSecTick = 0;
			//ShowCurrentTime();
			R1_BehaviorTimeElapse();
		}
		
			//if(DEBUG_MODE)
			//显示界面信息
			{
				unsigned char *st;
				switch (sR1Mower.MCU_STATUS)
				{
					case  R1_STOP:  //停止状态
						st="STOP   ";
						break;
					case R1_CHARGING: //充电状态
						st="CHARGE ";
						break;
					case R1_SETTING: //时间设定状态
						st="SETTING";
						break;
					case R1_MANUAL: //手动控制状态
						st="MANUAL ";
						break;
					case R1_MOW: //割草状态
						st="MOVING ";
						break;
					case R1_AVOID: //避障状态
						st="AVOID  ";
						break;
					case R1_TURN: //调头状态
						st="TURN   ";
						break;
					case R1_FAIL: //故障停止状态
						st="FAIL   ";
						break;
					case R1_LOOK: //寻找导引线状态
						st="HOME   ";
						break;
					case R1_FOLLOW: //沿导线走状态
						st="FOLLOW ";
						break;
					case R1_GOOUT: //沿导线走状态
						st="GO OUT ";
						break;
					case R1_IDLE: //沿导线走状态
						st="IDLE  ";
						break;
					case R1_PREPARE: //沿导线走状态
						st="PREPAR ";
						break;
					default:
						st="UNKOWN ";
						break;

				}
				if(!R1_has_fail())
				{
					//ShowStatusInfo(2,0,"WORK: ");
					Gui_DrawFont_GBK32(16,6,BLUE,GRAY0,st);  //显示
					//ShowBatteryInfo(sR1Mower.Power);
				}
				else
				{
					//ShowStatusInfo(2,0,"ERROR:");
					//ShowStatusInfo(3,0,sR1Mower.FailInfo);
					Gui_DrawFont_GBK32(16,6,BLUE,GRAY0,"ERROR");  //显示
				}
				
#ifdef DEBUG_MODE
//				ShowDebugInfo(2,8,(unsigned char)(sR1Mower.FAIL_FLAG >> 8));
//				ShowDebugInfo(2,10,(unsigned char)(sR1Mower.FAIL_FLAG & 0xFF));
//				
//				ShowDebugInfo(2,17,(unsigned char)(sR1Mower.TurnDir));
//				
//				sprintf(strbuff,"%6dmV",sR1Mower.BatVoltage);
//				ShowStatusInfo(3,12,strbuff);

				//ShowStatusInfo(4,0,"X:");
				Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"X:");  //显示
				
//				sR1Mower.MT_STATUS_H = sm_loc_x>>24;
//				ShowDebugInfo(4,3,sR1Mower.MT_STATUS_H);
//				sR1Mower.MT_STATUS_L = sm_loc_x>>16;
//				ShowDebugInfo(4,6,sR1Mower.MT_STATUS_L);
				Gui_DrawFont_Num32_Int(48,44,RED,GRAY0,sm_loc_x/100,4);	
				
				//ShowStatusInfo(4,10,"SS:");
//				sR1Mower.SS_STATUS_H = sm_loc_x>>8;
//				ShowDebugInfo(4,9,sR1Mower.SS_STATUS_H);
//				sR1Mower.SS_STATUS_L = sm_loc_x&0xFF;
//				ShowDebugInfo(4,12,sR1Mower.SS_STATUS_L);
				//Gui_DrawFont_Num32_Int(48,44,RED,GRAY0,sm_loc_x,4);	
				
// 				ShowStatusInfo(5,0,"BS:");
// 				ShowDebugInfo(5,3,sR1Mower.BS_WI_STATUS);
// 				ShowDebugInfo(5,7,sR1Mower.BSStrength[0]>>8);
// 				ShowDebugInfo(5,9,sR1Mower.BSStrength[0] & 0x00FF);
// 				//ShowDebugInfo(5,9,sR1Mower.BSStrength[0]>>2 & 0x00FF);
// 				
// 				ShowDebugInfo(5,13,sR1Mower.BSStrength[1]>>8);
// 				ShowDebugInfo(5,15,sR1Mower.BSStrength[1] & 0x00FF);
// 				//ShowDebugInfo(5,15,sR1Mower.BSStrength[1]>>2 & 0x00FF);
				//ShowStatusInfo(5,0,"Y:");
				Gui_DrawFont_GBK16(16,86,BLUE,GRAY0,"Y:");  //显示
//				ShowDebugInfo(5,3,sm_loc_y>>24);
//				ShowDebugInfo(5,6,sm_loc_y>>16);
//				ShowDebugInfo(5,9,sm_loc_y>>8);
//				ShowDebugInfo(5,12,sm_loc_y & 0xFF);
				Gui_DrawFont_Num32_Int(48,80,RED,GRAY0,sm_loc_y/100,4);	
				
				if(sR1Mower.TurnDir == 1) Gui_DrawFont_GBK16(190,96,BLUE,GRAY0,"Y1");
				else if(sR1Mower.TurnDir == 2) Gui_DrawFont_GBK16(190,96,BLUE,GRAY0,"Y2");
				else Gui_DrawFont_GBK16(190,96,BLUE,GRAY0,"  ");

// 				ShowStatusInfo(6,0,"T:");
// 				ShowDebugInfo(6,2,sR1Mower.Temperature);
// 				
// 				sprintf(strbuff,"P:%03d",(int16_t)sR1Mower.PitchAngle);
// 				ShowStatusInfo(6,5,strbuff);
// 				
// 				sprintf(strbuff,"R:%03d",(int16_t)sR1Mower.RollAngle);
// 				ShowStatusInfo(6,10,strbuff);
// 				
// 				sprintf(strbuff,"Y:%04d",(int16_t)sR1Mower.YawAngle);
// 				ShowStatusInfo(6,15,strbuff);
// 				
// 				
// 				sprintf(strbuff,"%7.2f",sR1Mower.sHMC5883.angle);
// 				ShowStatusInfo(1,8,strbuff);
//				ShowStatusInfo(6,0,"Box:");
//				ShowDebugInfo(6,6,sm_box_loc_x);
//				ShowDebugInfo(6,10,sm_box_loc_y);
					Gui_DrawFont_GBK16(16,122,BLUE,GRAY0,"BOX:");  //显示
					Gui_DrawFont_Num32_Int(48,116,RED,GRAY0,sm_box_loc_x,2);	
					Gui_DrawFont_Num32_Int(164,116,RED,GRAY0,sm_box_loc_y,2);	
					
				
#endif
				//if(sR1Mower.MCU_STATUS!= R1_SETTING) ShowStatusInfo(1,0,st);
			}
		

	}
	
}
void RCC_Configuration(void)
{ 
        RCC_DeInit();

//        SystemInit();//源自system_stm32f10x.c文件,只需要调用此函数,则可完成RCC的配置.具体请看2_RCC
        RCC_HSEConfig(RCC_HSE_OFF);

        RCC_HSICmd(ENABLE);                        //打开内部时钟
        
        while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)        
        {        
        }

//        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//        FLASH_SetLatency(FLASH_Latency_2);
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //APB2
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //APB1
        RCC_PCLK1Config(RCC_HCLK_Div1);
        //PLL 倍频
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);        //内部时钟倍频*16
        RCC_PLLCmd(ENABLE);                        //使能倍频
                                                                                                         
				//等待指定的 RCC 标志位设置成功 等待PLL初始化成功
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        

        /**************************************************
        获取RCC的信息,调试用
        请参考RCC_ClocksTypeDef结构体的内容,当时钟配置完成后,
        里面变量的值就直接反映了器件各个部分的运行频率
        ***************************************************/
                
        while(RCC_GetSYSCLKSource() != 0x08){}
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* 优先级组 说明了抢占优先级所用的位数，和子优先级所用的位数   在这里是1， 7 */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		   
  

	  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//配置外部中断源（秒中断） 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;												//指定中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;										//指定响应优先级别
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	if (SysTick_Config(SYSCLK/100000))		   //时钟节拍中断时10us一次  用于定时。 时钟频率48MHz，/480，则10us一次
  { 
    /* Capture error */ 
    while (1);
  } 
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	

	//配置PA3为驱动电源开关线
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	//配置步进电机输出线
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //AIN1,AIN2,AIN3	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;			//AIN5,AIN6	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //口线翻转速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}





