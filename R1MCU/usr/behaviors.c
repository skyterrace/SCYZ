#include "behaviors.h"
#include "R1DEF.h"
#include "key.h"
#include "math.h"
#include "DISP.h"
#include "clock_calendar.h"
#include "stepmotor.h"

float fol_e,fol_delta_e,fol_inte_e,fol_delta_v;
float kp,kd;
uint16_t BS_Max;
uint8_t cFollowCheckCount;
uint8_t cLookEnterArea0;
int16_t BehaviorTimeElapse=0;  //进入某一状态持续的时间，单位秒，在SwitchToBehavior中清零，由主程序调用R1_BehaviorTimeElapse()加1，

bool R1_CheckEvent(uint16_t CheckStatus)
{
	switch (CheckStatus)
	{
		case R1_STOP:
			//顶盖打开
			if(sR1Mower.MCU_STATUS & (R1_AUTOWORK | R1_IDLE | R1_CHARGING | R1_FAIL))
			{
				if(key_stop()) return TRUE; 
			}
			//进入充电座时
			if(sR1Mower.MCU_STATUS & R1_FOLLOW)
			{
				if(R1_is_charging()) return TRUE;
			}
				
			//手动控制结束
			if(sR1Mower.MCU_STATUS & R1_MANUAL)
			{
				if(sR1Mower.WorkMode != 1) return TRUE;
			}
			//参数设置结束
			if(sR1Mower.MCU_STATUS & R1_SETTING)
			{
				return TRUE;
			}
			break;
		
		case R1_FAIL:
			if(sR1Mower.MCU_STATUS & R1_AUTOWORK)
			{
				if(R1_has_fail()) 
					return TRUE;
			}
			
			if(sR1Mower.MCU_STATUS & R1_MANUAL)
			{
				if(R1_has_fail()) 
					return TRUE;
			}
			
			break;
		
		case R1_MANUAL:
			if(sR1Mower.MCU_STATUS & R1_STOP)
			{
				if(key_stop() && key_man()) return TRUE;
			}
			break;
		
		case R1_SETTING:
			if(sR1Mower.MCU_STATUS & R1_STOP)
			{
				if(key_set()) return TRUE;
			}
			break;
		
		case R1_CHARGING:
			if(sR1Mower.MCU_STATUS & R1_STOP)
			{
				if(R1_is_charging()) return TRUE;
			}
			break;
		
		case R1_IDLE:
			if(sR1Mower.MCU_STATUS & R1_CHARGING)
			{
				if(R1_is_full_charging()) return TRUE;
			}
			break;
		
		case R1_GOOUT:
			if(sR1Mower.MCU_STATUS & R1_IDLE)
			{
				if(R1_is_working_time()) return TRUE;
			}
			break;
		
		case R1_PREPARE:
			
			if(sR1Mower.MCU_STATUS & R1_STOP)
			{
				if(!key_stop()) {sm_box_all_arrived = 0; return TRUE;} //停止状态时顶盖被合上
			}
			
			if(sR1Mower.MCU_STATUS & R1_FAIL) //故障清除
			{
				if(!R1_has_fail() && BehaviorTimeElapse > 5) return TRUE;
			}
			
			if(sR1Mower.MCU_STATUS & R1_GOOUT)  //进入工作区
			{
				if(BehaviorTimeElapse > 5) return TRUE;
			}
			if(sR1Mower.MCU_STATUS & R1_MOW && sm_loc_arrived==1)  //arrived a box location
			{
				//if(BehaviorTimeElapse > 5) 
					return TRUE;
			}
			break;
		
		case R1_MOW:
			if(sR1Mower.MCU_STATUS & R1_PREPARE)
			{
				if(!R1_has_fail() && BehaviorTimeElapse > 3)
				{
					return TRUE;
				}
			}
			
			break;
		
		case R1_TURN:
			break;
		
		case R1_AVOID:
// 			if(sR1Mower.MCU_STATUS & (R1_MOW|R1_TURN|R1_LOOK|R1_FOLLOW))
// 			{
// 				if(sR1Mower.SS_STATUS_L & 0x0F) return TRUE;
// 			}
			break;
		
		case R1_LOOK:
			if(sR1Mower.MCU_STATUS & R1_MOW)
			{
				//if(!R1_is_working_time() || R1_is_raining() || R1_is_low_battery() || R1_is_go_home()) return TRUE;
				if(sm_box_all_arrived == 1) return TRUE; 
				if(!R1_is_working_time() || R1_is_go_home()) return TRUE;
			}
			else if(sR1Mower.MCU_STATUS & R1_PREPARE)
			{
				//if(!R1_is_working_time() || R1_is_raining() || R1_is_low_battery() || R1_is_go_home()) return TRUE;
				if(sm_box_all_arrived == 1) return TRUE; 
			}
// 			if(sR1Mower.MCU_STATUS & R1_FOLLOW)
// 			{
// 				//if(sR1Mower.BSStrength[1]/4 < sR1Mower.sRunParam.BsStrenghL) return TRUE;
// 			}
			break;
		
		case R1_FOLLOW:
			if(sR1Mower.MCU_STATUS & R1_LOOK)
			{
				//if(sR1Mower.BSStrength[1]/4 > sR1Mower.sRunParam.BsStrenghL) return TRUE;
// 				if(R1_is_near_middle_line() && sR1Mower.BSStrength[R1_is_in_which_area()] < BS_Max)
// 				{
// 					cFollowCheckCount++;
// 					if(cFollowCheckCount > 6) return TRUE;
// 				}
// 				else
// 				{
// 					cFollowCheckCount = 0;
// 				}
			}
			break;
		
		default:
			break;		
	}
	return FALSE;
}

void R1_SwitchToBehavior(uint16_t R1Status)
{
	switch (R1Status)
	{
		case R1_STOP:
			//打开显示屏
			SM_Enable(0);
			break;
		
		case R1_FAIL:
			SM_Enable(0);
			break;
		
		case R1_MANUAL:
			SM_Enable(1);
		sR1Mower.TurnDir = 0;
			break;
		
		case R1_SETTING:
			break;
		
		case R1_CHARGING:
			break;
		
		case R1_IDLE:
			//断开充电器
			break;
		
		case R1_GOOUT:

			break;
		
		case R1_PREPARE:
			SM_Enable(1);		
			//关闭显示屏
			break;
		
		case R1_MOW:
			sm_box_all_arrived = 0;
			break;
		
		case R1_TURN:

			break;
		
		case R1_AVOID:

			break;
		
		case R1_LOOK:
			if((sm_loc_x-0)>0)
			{
				sm_x_dir=0;
				sm_loc_x_need =  sm_loc_x;
			}
			else if((sm_loc_x-0)<0)
			{
				sm_x_dir=1;
				sm_loc_x_need =  -sm_loc_x;
			}
			
			if((sm_loc_y-0)>0)
			{
				sm_y_dir=0;
				sm_loc_y_need =  sm_loc_y;
			}
			else if((sm_loc_y-0)<0)
			{
				sm_y_dir=1;
				sm_loc_y_need =  -sm_loc_y;
			}

			sm_loc_arrived = 0;
			break;
		
		case R1_FOLLOW:

			break;
		
		default:
			return;
	}
	sR1Mower.MCU_STATUS = R1Status;
	BehaviorTimeElapse = 0;
	return;
}

void R1_ExcecuteActions(void)
{
	switch (sR1Mower.MCU_STATUS)
	{
		case R1_STOP:
			key_man();
			break;
		
		case R1_FAIL:
			break;
		
		case R1_MANUAL:
			R1_ManualControl();
			key_man();

			break;
		
		case R1_SETTING:
			SetCurrentDateTime();
			SetWorkingTime();
			SetParam();
			DispClear();
			TIM3_Init();

  		ShowDefaultScreen(sR1Mower.Power,sR1Mower.WorkMode);
			break;
		
		case R1_CHARGING:
			break;
		
		case R1_IDLE:
			break;
		
		case R1_GOOUT:

			break;
		
		case R1_PREPARE:
			break;
		
		case R1_MOW:
			if(sm_loc_arrived == 1)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);  //设置投饵继电器
				Delay_ms(500);
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);  //清除投饵继电器
				//SM_Goto_Box(2,1);
				Delay_ms(1000);
				SM_Goto_Next();
			}
			break;
		
		case R1_TURN:
			break;
		
		case R1_AVOID:
			break;
		
		case R1_LOOK:
			if(sm_loc_arrived) {sm_box_loc_x=sm_box_loc_y=0;}
			break;
		
		case R1_FOLLOW:

			break;
		
		default:
			break;		
	}
	return;
}

void R1_ManualControl(void)
{
	//前进后退
	if(!READ_KEY_UP)
	{
		if(sR1Mower.TurnDir == 1)
			sm_loc_y1_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		else if (sR1Mower.TurnDir == 2)
			sm_loc_y2_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		else
			sm_loc_y_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		sm_y_dir = 1;
	}
	else if(!READ_KEY_DOWN)
	{
		if(sR1Mower.TurnDir == 1)
			sm_loc_y1_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		else if (sR1Mower.TurnDir == 2)
			sm_loc_y2_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		else
			sm_loc_y_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		sm_y_dir = 0;
	}
	//左右转
	else if(!READ_KEY_LEFT)
	{
		sm_loc_x_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		sm_x_dir = 0;
	}
	else if(!READ_KEY_RIGHT)
	{
		sm_loc_x_need = sR1Mower.sRunParam.BladeSpeed*100/10;
		sm_x_dir = 1;
	}
	else
	{

	}
	if(Key_Release(KEY_SET))
	{
		if(READ_KEY_STOP) 
		{
			SM_SetZero(0,0);
		}
		else
		{
			sR1Mower.TurnDir++;
			if(sR1Mower.TurnDir>2)sR1Mower.TurnDir=0;
		}
	}
	else
	{

	}

}

void R1_BehaviorTimeElapse(void)
{
	BehaviorTimeElapse++;
}
