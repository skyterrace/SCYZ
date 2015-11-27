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
int16_t BehaviorTimeElapse=0;  //����ĳһ״̬������ʱ�䣬��λ�룬��SwitchToBehavior�����㣬�����������R1_BehaviorTimeElapse()��1��

bool R1_CheckEvent(uint16_t CheckStatus)
{
	switch (CheckStatus)
	{
		case R1_STOP:
			//���Ǵ�
			if(sR1Mower.MCU_STATUS & (R1_AUTOWORK | R1_IDLE | R1_CHARGING | R1_FAIL))
			{
				if(key_stop()) return TRUE; 
			}
			//��������ʱ
			if(sR1Mower.MCU_STATUS & R1_FOLLOW)
			{
				if(R1_is_charging()) return TRUE;
			}
				
			//�ֶ����ƽ���
			if(sR1Mower.MCU_STATUS & R1_MANUAL)
			{
				if(sR1Mower.WorkMode != 1) return TRUE;
			}
			//�������ý���
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
				if(!key_stop()) {sm_box_all_arrived = 0; return TRUE;} //ֹͣ״̬ʱ���Ǳ�����
			}
			
			if(sR1Mower.MCU_STATUS & R1_FAIL) //�������
			{
				if(!R1_has_fail() && BehaviorTimeElapse > 5) return TRUE;
			}
			
			if(sR1Mower.MCU_STATUS & R1_GOOUT)  //���빤����
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
			//����ʾ��
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
			//�Ͽ������
			break;
		
		case R1_GOOUT:

			break;
		
		case R1_PREPARE:
			SM_Enable(1);		
			//�ر���ʾ��
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
				GPIO_SetBits(GPIOA,GPIO_Pin_3);  //����Ͷ���̵���
				Delay_ms(500);
				GPIO_ResetBits(GPIOA,GPIO_Pin_3);  //���Ͷ���̵���
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
	//ǰ������
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
	//����ת
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
