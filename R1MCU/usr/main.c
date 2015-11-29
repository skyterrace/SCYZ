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


/*ϵͳ���ñ����ͺ�������*/

void RCC_Configuration(void); //ϵͳʱ������
void NVIC_Configuration(void); //�ж�����
void GPIO_Config(void); //GPIO����

/*ϵͳ���ö������*/

/*��ʱ�ͽ��ı����ͺ�������*/
uint8_t TimeSecTick=0; //���жϱ�־��=1�����жϷ���
/*��ʱ�ͽ��ı����ͺ����������*/

/*���ڲ��������ͺ�������*/

// uint8_t TxBuffer1[30], RxBuffer1[30]; //����1������
// uint8_t rec_f; //����1���ձ�־�����յ�������Ϊ1

/*���ڲ��������ͺ����������*/


/*���������ͺ�������*/



//extern char strbuff[]; //��ʾ����������DISP.c�ж���

/*���������ͺ����������*/

void TIM3_Init(void);

int main(void)
{
	RCC_Configuration(); 
	GPIO_Config();
	NVIC_Configuration();
	
	Lcd_Init();
	Lcd_Clear(GRAY0);
	
	Key_Config();

	//���Ź���ʼ��
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)!=RESET)  //����ϴεĸ�λ�ǿ��Ÿ�λ�Ļ�
	{
// 		SetMowSpeed(0);
// 		SetDriverSpeed(0,0);
		//ShowStatusInfo(3,0,"Watch Dog!");
		Gui_DrawFont_GBK32(16,6,BLUE,GRAY0,"���ȣ�");  //��ʾ
		//RCC_ClearFlag(); //����RTC_Configuration�����Ĵ���
		//while(!Key_Press(KEY_OK));
		Delay_ms(3000);
	}
	/* д��0x5555,�����������Ĵ���д�빦�� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	/* ι��ʱ�� 5s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(781);
	
	/* ι��*/
	IWDG_ReloadCounter();
	/* ʹ�ܹ���*/
 	IWDG_Enable();
	
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock();

  /* EEPROM Init */
  EE_Init();
	R1_Default();
	RTC_Configuration();  //���û�����ù�ʱ�ӣ��ὫR1 Defaultֵ���浽EERom�����Ҫ����R1_Default�������档
	EE_LoadWorkingTime();
	EE_LoadParams();


	TimeSecTick = 0;
	TIM3_Init();
	
	SM_Enable(0);
	SM_SetZero(0,0);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);   //���Ͷ���̵���
	sm_box_all_arrived = 0;
	
	Gui_DrawDefaultScreen(sR1Mower.WorkMode);  //��ʾ

	while(1)
	{
	
		IWDG_ReloadCounter(); //ι��
		
		
		//���Դ���
// 		Delay_ms(1000);
// 		if(Key_Press(KEY_OK)) sR1Mower.BSStrength[0] = sR1Mower.BSStrength[1] = 30;
// 		else sR1Mower.BSStrength[0] = sR1Mower.BSStrength[1] = 1;
		
		//���Դ������
		
// 		if(sR1Mower.WorkMode == 2)
// 		{
// 			GPIO_SetBits(GPIOA,GPIO_Pin_3);
// 		}
// 		else
// 		{
// 			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
// 		}
		//��C0�����һ��������������������
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
			//��ʾ������Ϣ
			{
				unsigned char *st;
				switch (sR1Mower.MCU_STATUS)
				{
					case  R1_STOP:  //ֹͣ״̬
						st="STOP  ";
						break;
					case R1_CHARGING: //���״̬
						st="CHARGE";
						break;
					case R1_SETTING: //ʱ���趨״̬
						st="SETTING";
						break;
					case R1_MANUAL: //�ֶ�����״̬
						st="MANUAL";
						break;
					case R1_MOW: //���״̬
						st="MOVING";
						break;
					case R1_AVOID: //����״̬
						st="AVOID ";
						break;
					case R1_TURN: //��ͷ״̬
						st="TURN  ";
						break;
					case R1_FAIL: //����ֹͣ״̬
						st="FAIL  ";
						break;
					case R1_LOOK: //Ѱ�ҵ�����״̬
						st="HOME  ";
						break;
					case R1_FOLLOW: //�ص�����״̬
						st="FOLLOW";
						break;
					case R1_GOOUT: //�ص�����״̬
						st="GO OUT";
						break;
					case R1_IDLE: //�ص�����״̬
						st="IDLE  ";
						break;
					case R1_PREPARE: //�ص�����״̬
						st="PREPAR";
						break;
					default:
						st="UNKOWN";
						break;

				}
				if(!R1_has_fail())
				{
					//ShowStatusInfo(2,0,"WORK: ");
					Gui_DrawFont_GBK16(16,6,BLUE,GRAY0,st);  //��ʾ
					//ShowBatteryInfo(sR1Mower.Power);
				}
				else
				{
					//ShowStatusInfo(2,0,"ERROR:");
					//ShowStatusInfo(3,0,sR1Mower.FailInfo);
					Gui_DrawFont_GBK16(16,6,BLUE,GRAY0,"ERROR");  //��ʾ
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
				Gui_DrawFont_GBK16(16,24,BLUE,GRAY0,"X:");  //��ʾ
				
//				sR1Mower.MT_STATUS_H = sm_loc_x>>24;
//				ShowDebugInfo(4,3,sR1Mower.MT_STATUS_H);
//				sR1Mower.MT_STATUS_L = sm_loc_x>>16;
//				ShowDebugInfo(4,6,sR1Mower.MT_STATUS_L);
				//Gui_DrawFont_Num32_Int(48,24,RED,GRAY0,sm_loc_x>>16,4);	
				
				//ShowStatusInfo(4,10,"SS:");
//				sR1Mower.SS_STATUS_H = sm_loc_x>>8;
//				ShowDebugInfo(4,9,sR1Mower.SS_STATUS_H);
//				sR1Mower.SS_STATUS_L = sm_loc_x&0xFF;
//				ShowDebugInfo(4,12,sR1Mower.SS_STATUS_L);
				Gui_DrawFont_Num32_Int(48,24,RED,GRAY0,sm_loc_x,4);	
				
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
				Gui_DrawFont_GBK16(16,40,BLUE,GRAY0,"Y:");  //��ʾ
//				ShowDebugInfo(5,3,sm_loc_y>>24);
//				ShowDebugInfo(5,6,sm_loc_y>>16);
//				ShowDebugInfo(5,9,sm_loc_y>>8);
//				ShowDebugInfo(5,12,sm_loc_y & 0xFF);
				Gui_DrawFont_Num32_Int(48,40,RED,GRAY0,sm_loc_y,4);	

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
					Gui_DrawFont_GBK16(16,56,BLUE,GRAY0,"BOX:");  //��ʾ
					Gui_DrawFont_Num32_Int(80,56,RED,GRAY0,sm_box_loc_x,2);	
					Gui_DrawFont_Num32_Int(144,56,RED,GRAY0,sm_box_loc_y,2);	
					
				
#endif
				//if(sR1Mower.MCU_STATUS!= R1_SETTING) ShowStatusInfo(1,0,st);
			}
		

	}
	
}
void RCC_Configuration(void)
{ 
        RCC_DeInit();

//        SystemInit();//Դ��system_stm32f10x.c�ļ�,ֻ��Ҫ���ô˺���,������RCC������.�����뿴2_RCC
        RCC_HSEConfig(RCC_HSE_OFF);

        RCC_HSICmd(ENABLE);                        //���ڲ�ʱ��
        
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
        //PLL ��Ƶ
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);        //�ڲ�ʱ�ӱ�Ƶ*16
        RCC_PLLCmd(ENABLE);                        //ʹ�ܱ�Ƶ
                                                                                                         
				//�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);        

        /**************************************************
        ��ȡRCC����Ϣ,������
        ��ο�RCC_ClocksTypeDef�ṹ�������,��ʱ��������ɺ�,
        ���������ֵ��ֱ�ӷ�ӳ�������������ֵ�����Ƶ��
        ***************************************************/
                
        while(RCC_GetSYSCLKSource() != 0x08){}
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  /* Configure one bit for preemption priority */
  /* ���ȼ��� ˵������ռ���ȼ����õ�λ�����������ȼ����õ�λ��   ��������1�� 7 */    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		   
  

	  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;					//�����ⲿ�ж�Դ�����жϣ� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;												//ָ���ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;							
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;										//ָ����Ӧ���ȼ���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
	
	if (SysTick_Config(SYSCLK/100000))		   //ʱ�ӽ����ж�ʱ10usһ��  ���ڶ�ʱ�� ʱ��Ƶ��48MHz��/480����10usһ��
  { 
    /* Capture error */ 
    while (1);
  } 
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);	

	//����PA3Ϊ������Դ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	//���ò�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;				 //AIN1,AIN2,AIN3	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;			//AIN5,AIN6	     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���߷�ת�ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}





