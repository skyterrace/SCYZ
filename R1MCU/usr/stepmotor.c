#include "stepmotor.h"
#include "R1DEF.h"

__IO int32_t sm_loc_x, sm_loc_y;

__IO uint32_t sm_loc_x_need, sm_loc_y_need, sm_loc_y1_need, sm_loc_y2_need;;

__IO uint8_t stepmotor_flag;
__IO uint8_t sm_loc_arrived, sm_box_all_arrived;

__IO uint8_t sm_box_loc_x,sm_box_loc_y;

__IO uint8_t sm_x_dir,sm_y_dir;  //direction of step motor, 1 is increase, 0 is decrease

void SM_SetZero(int32_t x,int32_t y)
{
		sm_loc_x = x;
		sm_loc_y = y;
		sm_box_loc_x=0;
		sm_box_loc_y=0;
}

void SM_Enable(unsigned char bEnabled)
{
	if(bEnabled)
	{
		stepmotor_flag = 0;
		sm_x_dir=sm_y_dir=0;
		sm_loc_x_need=sm_loc_y_need=sm_loc_y1_need=sm_loc_y2_need=0;
		sm_loc_arrived = 0;
		TIM_Cmd(TIM3, ENABLE);  																							//计数器使能，开始工作	
	}
	else
	{
		TIM_Cmd(TIM3, DISABLE); 
		MOTOR_X_PULSE_RESET
		MOTOR_Y1_PULSE_RESET
		MOTOR_Y2_PULSE_RESET
	}
}

void SM_Run(void)
{
	if(stepmotor_flag)
	{
		if(sm_loc_x_need>0)
		{
			sm_loc_arrived = 0;
			if(sm_x_dir) 
			{
				MOTOR_X_DIR_FWD
				sm_loc_x++;
			}
			else
			{
				MOTOR_X_DIR_REV
				sm_loc_x--;
			}
			MOTOR_X_PULSE_SET
			sm_loc_x_need--;
		}
		
		if(sm_loc_y_need>0)
		{
			sm_loc_arrived = 0;
			if(sm_y_dir) 
			{
				MOTOR_Y1_DIR_FWD
				MOTOR_Y2_DIR_FWD
				sm_loc_y++;
			}
			else
			{
				MOTOR_Y1_DIR_REV
				MOTOR_Y2_DIR_REV
				sm_loc_y--;
			}
			MOTOR_Y1_PULSE_SET
			MOTOR_Y2_PULSE_SET
			sm_loc_y_need--;
		}
		
		if(sm_loc_y1_need>0)
		{
			if(sm_y_dir) 
			{
				MOTOR_Y1_DIR_FWD
			}
			else
			{
				MOTOR_Y1_DIR_REV
			}
			MOTOR_Y1_PULSE_SET
			sm_loc_y1_need--;
		}
		
		if(sm_loc_y2_need>0)
		{
			if(sm_y_dir) 
			{
				MOTOR_Y2_DIR_FWD
			}
			else
			{
				MOTOR_Y2_DIR_REV
			}
			MOTOR_Y2_PULSE_SET
			sm_loc_y2_need--;
		}
		stepmotor_flag = 0;
	}
	else
	{
		MOTOR_X_PULSE_RESET
		MOTOR_Y1_PULSE_RESET
		MOTOR_Y2_PULSE_RESET
		stepmotor_flag = 1;
	}
	if(sm_loc_x_need==0 && sm_loc_y_need==0)
		sm_loc_arrived = 1;

}

void SM_Goto_Box(uint8_t box_x,uint8_t box_y)
{
	int32_t temp;

	temp = sR1Mower.sRunParam.BladeSpeed*100*MOTOR_X_GEAR_RATIO*(box_x * sR1Mower.sRunParam.KP1*200)/(MOTOR_X_GEAR_COUNT*MOTOR_X_GEAR_DIS);
	temp = temp-sm_loc_x;
	if(temp>0)
	{
		sm_loc_x_need = temp;
		sm_x_dir = 1;
	}
	else
	{
		sm_loc_x_need = -temp;
		sm_x_dir = 0;
	}

	temp = sR1Mower.sRunParam.BladeSpeed*100*MOTOR_Y_GEAR_RATIO*(box_y * sR1Mower.sRunParam.KD1*200)/(MOTOR_Y_GEAR_COUNT*MOTOR_Y_GEAR_DIS);
	temp = temp-sm_loc_y;
	if(temp>0)
	{
		sm_loc_y_need = temp;
		sm_y_dir = 1;
	}
	else
	{
		sm_loc_y_need = -temp;
		sm_y_dir = 0;
	}
	
	sm_box_loc_x = box_x;
	sm_box_loc_y = box_y;
	
	sm_loc_arrived = 0;
}

void SM_Goto_Next(void)
{
	int8_t box_x,box_y;
	box_x=sm_box_loc_x;
	box_y=sm_box_loc_y;
	if(sm_box_loc_y%2 == 0)
	{
		box_x = sm_box_loc_x + 1;
		if(box_x>=sR1Mower.sRunParam.DrvSpeedL)
		{
			box_y = sm_box_loc_y + 1;
			box_x = sR1Mower.sRunParam.DrvSpeedL - 1;
		}
	}
	else
	{
		box_x = sm_box_loc_x - 1;
		if(box_x<0)
		{
			box_y = sm_box_loc_y + 1;
			box_x = 0;
		}
	}
	
	if( box_y >=  sR1Mower.sRunParam.BsStrenghL)
	{
		box_y = sR1Mower.sRunParam.BsStrenghL - 1;
		sm_box_all_arrived = 1;
	}
	
	SM_Goto_Box(box_x,box_y);
	
}

void TIM3_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  TIM_TimeBaseStructure.TIM_Period = (SYSCLK/4)/(sR1Mower.sRunParam.BladeSpeed*100*2) - 1;												//自动装载的计数值，由于计数是从0开始的，计数10000次后为9999
  
  TIM_TimeBaseStructure.TIM_Prescaler = 3;											// 这个就是预分频系数，当由于为0时表示不分频所以要减1

  TIM_TimeBaseStructure.TIM_ClockDivision = 0;													
  
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;						//向上计数
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);												//初始化定时器5

  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);														//清除TIM5溢出中断标志
 
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);														//打开溢出中断

//   TIM_Cmd(TIM3, ENABLE);  																							//计数器使能，开始工作	
}
