#include "R1DEF.h"
// #include "R1_MCU.h"
#include "clock_calendar.h"
#include "behaviors.h"

#include "stepmotor.h"
struct R1Mower_s sR1Mower;

int8_t nGoOutCount=0;

void R1_Default(void)
{
	uint8_t i;
	sR1Mower.WorkMode = 0;
	sR1Mower.MCU_STATUS = R1_STOP;
	sR1Mower.Power = 100;
	sR1Mower.Temperature = 25;
	sR1Mower.MT_STATUS_H = 0;
	sR1Mower.MT_STATUS_L = 0;
	
	sR1Mower.FAIL_FLAG = 0;
	
	for(i=0;i<7;i++)
	{
		sR1Mower.sWorkTime[i].StartHour = 9;
		sR1Mower.sWorkTime[i].StartMin = 0;
		sR1Mower.sWorkTime[i].EndHour = 15;
		sR1Mower.sWorkTime[i].EndMin = 0;
	}
	
	sR1Mower.sRunParam.PowerLimit = BAT_LOW_LIMIT;
	
	//sR1Mower.sRunParam.BladeSpeed = BLADE_SPEED;
	sR1Mower.sRunParam.BladeSpeed = STEP_MOTOR_FREQ/100;
	
	sR1Mower.sRunParam.BsStrenghH = BS_STRENGTH_H;
	sR1Mower.sRunParam.BsStrenghM = BS_STRENGTH_M;
	//sR1Mower.sRunParam.BsStrenghL = BS_STRENGTH_L;
	sR1Mower.sRunParam.BsStrenghL = BOX_Y_COUNT;
	
	sR1Mower.sRunParam.DrvSpeedH = MOW_SPEED_H;
	sR1Mower.sRunParam.DrvSpeedM = MOW_SPEED_M;
	//sR1Mower.sRunParam.DrvSpeedL = MOW_SPEED_L;
	sR1Mower.sRunParam.DrvSpeedL = BOX_X_COUNT;
	
	//sR1Mower.sRunParam.KP1 = 0.4;
	sR1Mower.sRunParam.KP1 = BOX_X_LEN/1000.0;
	//sR1Mower.sRunParam.KD1 = 0.6;
	sR1Mower.sRunParam.KD1 = BOX_Y_LEN/1000.0;
	sR1Mower.sRunParam.KI1 = 0.0;
	
	sR1Mower.sRunParam.KP2 = 0.4;
	sR1Mower.sRunParam.KD2 = 0.6;
	sR1Mower.sRunParam.KI2 = 0.0;
	
	sR1Mower.TargetAngle = 0;
	sR1Mower.TurnDir = 0;
	
	sR1Mower.sRunParam.MotionSense = 1280;
	sR1Mower.sRunParam.BehaviorTO = 10;
	
}

bool R1_is_low_battery(void)
{
	return ((sR1Mower.Power < sR1Mower.sRunParam.PowerLimit) ? TRUE : FALSE);
}

bool R1_has_fail(void)
{
// 	bool nRet = TRUE;

// 	if(sR1Mower.FAIL_FLAG & (FAIL_FLAG_SS_TO|FAIL_FLAG_HMC5883_TO|FAIL_FLAG_MPU6050_TO|FAIL_FLAG_MMA7660_TO))
// 	{
// 		sR1Mower.FailInfo = "SS Timeout ";
// 	}
// 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_MT_TO)
// 	{
// 		sR1Mower.FailInfo = "MT Timeout ";
// 	}
// 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_MT_OT)
// 	{
// 		sR1Mower.FailInfo = "MT OverTemp";
// 	}
// 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_MT_OC)
// 	{
// 		sR1Mower.FailInfo = "MT OverCurr";
// 	}
// // 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_BS_TO)
// // 	{
// // 		sR1Mower.FailInfo = "BS Timeout ";
// // 	}
// 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_BS_WI)
// 	{
// 		sR1Mower.FailInfo = "BS Wire    ";
// 	}
// 	else if(sR1Mower.FAIL_FLAG & FAIL_FLAG_BS_RF)
// 	{
// 		sR1Mower.FailInfo = "BS RF      ";  
// 	}
// 	else if(sR1Mower.TiltAngle>27) //R1是否倾覆，横滚或者俯仰角度大于27度，则认为倾覆
// 	{
// 		sR1Mower.FailInfo = "Over Turn  ";
// 	}
// 	else if(R1_is_out_area()) //R1跑到边界之外
// 	{
// 		sR1Mower.FailInfo = "Out Area   ";
// 	}
// 	else
// 	{
// 		sR1Mower.FailInfo = "NO ERROR   ";
// 		nRet = FALSE;
// 	}

// 	return nRet;
 return FALSE;
}

bool R1_is_working_time(void)  //用到clock_calendar.c中的函数和变量定义
{
	uint16_t nTime,nStart,nEnd,nWeekDay;
	nWeekDay = WeekDay(s_DateStructVar.Year,s_DateStructVar.Month,s_DateStructVar.Day);
	nTime = (s_TimeStructVar.HourHigh*10+s_TimeStructVar.HourLow)*60+(s_TimeStructVar.MinHigh*10+s_TimeStructVar.MinLow);
	nStart = sR1Mower.sWorkTime[nWeekDay-1].StartHour*60+sR1Mower.sWorkTime[nWeekDay-1].StartMin;
	nEnd = sR1Mower.sWorkTime[nWeekDay-1].EndHour*60+sR1Mower.sWorkTime[nWeekDay-1].EndMin;
	if(nTime > nStart && nTime < nEnd)	return TRUE;
	else return FALSE;
}

bool R1_is_go_home()
{
	return ((sR1Mower.WorkMode == 2) ? TRUE : FALSE);
}

bool R1_is_raining()
{
	return ((sR1Mower.SS_STATUS_L & 0x20) ? TRUE : FALSE);
}
bool R1_is_charging()
{
	return FALSE;
}	
bool R1_is_full_charging(void)
{
	return FALSE;
}

bool R1_is_crashing()
{
	return ((sR1Mower.SS_STATUS_L  & 0x0F) ? TRUE:FALSE);
}	

//判断是否位于工作区域之外，如果连续检测到20次异相，则认为在边界之外。
bool R1_is_out_area(void)
{
	//if(sR1Mower.WorkMode == 3) return FALSE;
	if(sR1Mower.BS_WI_STATUS & 0x03)
		nGoOutCount++;
	else
		nGoOutCount=0;
	//return FALSE;
	//return ((sR1Mower.BS_WI_STATUS & 0x03) ? TRUE:FALSE);
	if(nGoOutCount>20)
	{
		nGoOutCount=0;
		return TRUE;
	}
	else return FALSE;
}

//根据多个电缆信号的强度确定边界信号强度
void UpdateBorderStrength()
{
	if(sR1Mower.BSStrength[0]>sR1Mower.BSStrength[1])
	{
		sR1Mower.BorderStrength = sR1Mower.BSStrength[0] >>2 ;
		if((sR1Mower.BSStrength[0]-sR1Mower.BSStrength[1]) < 4*sR1Mower.sRunParam.BsStrenghM/2 )
			sR1Mower.BorderStrength = (sR1Mower.BSStrength[0]-sR1Mower.BSStrength[1]) >> 2;
	}
	else
	{
		sR1Mower.BorderStrength = sR1Mower.BSStrength[1] >> 2;
		if((sR1Mower.BSStrength[1]-sR1Mower.BSStrength[0]) < 4*sR1Mower.sRunParam.BsStrenghM/2)
			sR1Mower.BorderStrength = (sR1Mower.BSStrength[1]-sR1Mower.BSStrength[0]) >> 2;
	}
	
}

//判断是否在中线附近，如果电缆0和电缆1的信号强度都大于信号强度最小设定值，则返回真，否者返回假
//*4是因为信号强度取高八位，舍弃低二位
bool R1_is_near_middle_line(void)
{
	return (sR1Mower.BSStrength[0]> 4*sR1Mower.sRunParam.BsStrenghL && sR1Mower.BSStrength[1]> 4*sR1Mower.sRunParam.BsStrenghL) ? TRUE : FALSE;
}

//可判断是在哪个电缆区域，一般来讲位于信号强度大的那个区域。
uint8_t R1_is_in_which_area(void)
{
	if(sR1Mower.BSStrength[0] > sR1Mower.BSStrength[1]) return 0;
	else return 1;
}
