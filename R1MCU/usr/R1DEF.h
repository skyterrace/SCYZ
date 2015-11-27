#ifndef __R1DEF_H
#define __R1DEF_H
#include "stm32f10x.h"
#include "HMC5883.h"

#define USE_MPU6050   //设置是否用MPU6050检测倾斜角，否则使用MMA7660，R1Mower_s中YawAngle使用HMC5883的地磁角。

#define R1_BAT_VOL_AD_RATIO 11 //电池电压AD采集分压比
#define R1_BAT_VOL_MAX 28000 //电池电压最大值，单位mV
#define R1_BAT_VOL_MIN 22000 //电池电压最小值，单位mV


//错误状态
#define FAIL_FLAG_BS_TO 0x0001
#define FAIL_FLAG_SS_TO 0x0002
#define FAIL_FLAG_MT_TO 0x0004
#define FAIL_FLAG_MT_OT 0x0008
#define FAIL_FLAG_MT_OC 0x0010
#define FAIL_FLAG_BS_RF 0x0020 //BS无线不正常
#define FAIL_FLAG_BS_WI 0x0040 //BS接收电缆断

#define FAIL_FLAG_HMC5883_TO 0x0100
#define FAIL_FLAG_MPU6050_TO 0x0080
#define FAIL_FLAG_MMA7660_TO 0x0200

//常量定义
#define BAT_LOW_LIMIT 50
#define MOW_SPEED_H 35	//高速度
#define MOW_SPEED_M 25	//中速度
#define MOW_SPEED_L 15	//低速度
#define BLADE_SPEED 110 //刀片速度

#define BS_STRENGTH_H 80
#define BS_STRENGTH_M 50
#define BS_STRENGTH_L 20


#define bitset(var,bitno) ((var) |= (1<<(bitno)))
#define bitclr(var,bitno) ((var) &= ~(1<<(bitno)))

#define setflag(var,bitno) ((var) |= (bitno))
#define clrflag(var,bitno) ((var) &= ~((bitno)))

struct R1_RunParam_s
{
  uint8_t DrvSpeedH;   //驱动电机高速档
  uint8_t DrvSpeedM;	//驱动电机中速档
  uint8_t DrvSpeedL;	//驱动电机低速档
  uint8_t BladeSpeed;	//割草电机速度
  uint8_t BsStrenghH;	//边界信号强度高
  uint8_t BsStrenghM;	//边界信号强度中
	uint8_t BsStrenghL;	//边界信号强度低
	uint8_t PowerLimit;	//电池剩余电量百分比报警值
	float KP1;	//跟踪调整P值
	float KD1;	//跟踪调整D值
	float KI1;	//跟踪调整I值
	float KP2;	//跟踪调整P值
	float KD2;	//跟踪调整D值
	float KI2;	//跟踪调整I值
	uint16_t BehaviorTO;  //单一行为超时时间，单位：秒，即超过该时间则结束当前行为
	uint16_t MotionSense;  //碰撞灵敏度，单位：毫重力单位mg，典型值640；
};

struct R1_WorkTime_s
{
	uint8_t StartHour;
	uint8_t StartMin;
	uint8_t EndHour;
	uint8_t EndMin;
};


struct R1Mower_s
{
	uint8_t WorkMode; //当前工作模式，0-随机自动，1-手动，2-回家，3-往复自动
	uint8_t Power; //剩余电量百分比
	//uint8_t LCDClosed; //LCD是否关闭 1 关闭，0-打开
	int8_t Temperature;//当前温度，单位：摄氏度
	uint16_t BatVoltage; //电池电压，单位：毫伏
	
	uint8_t MT_STATUS_L; 	//MT状态位L。
												//D0急停状态，1为真；
												//D1暂停状态，1为真；
												//D2表示过流状态，1为真；
												//D3表示过热状态，1为真；
												//D4表示左轮正反转，0为正转，1为反转；
												//D5表示右轮正反转，0为正转，1为反转；
												//D6表示驱动电机工作状态，1为工作，0为停止。
												//D7=0始终为0

	uint8_t MT_STATUS_H; 	//MT状态位H
												// D0未定义
												// D1未定义
												// D2未定义
												// D3未定义
												// D4未定义
												// D5未定义
												// D6表示割草电机工作状态，1为工作，0为停止。
												// D7=0始终为0

	uint8_t SS_STATUS_L; 	//SS状态位L
												//D0~D3碰撞检测：D0前方；D1后方；D2左方；D3右方
												//D4表示离地状态，1为真
												//D5表示雨淋状态，1为真
												//D6表示有充电电压状态，1为真
	uint8_t SS_STATUS_H; //SS状态位H
	uint8_t SS_ERROR_L; //SS错误位L
											//D0表示I2C通讯故障，1为真
											//D1表示电池状态故障，1为真（如温度保护，健康状态）
											//D2表示重力传感器故障，1为真
											//D3表示航向传感器故障，1为真
											//D4表示碰撞传感器故障，1为真
											
// 	uint8_t BS_STATUS_L; //BS状态位L
// 	uint8_t BS_STATUS_H; //BS状态位H
	uint8_t BS_WI_STATUS; //边界电缆状态，D7~D4：电缆3~0的通断情况，1正常，0断开；D3~D0，电缆3~0与电缆0的相位关系，0同向，1反向
	

	uint16_t MCU_STATUS; //MCU16位状态字
	uint16_t FAIL_FLAG;  //故障标志,1有故障，0无故障
	
	int8_t LeftSpeed, RightSpeed;  //当前左右轮速度值 -63~64
	int8_t LastLeftSpeed, LastRightSpeed;  //上一次左右轮速度值 -63~64
	
	int16_t BSStrength[4]; //通道0~3的边界信号强度
	int16_t BorderStrength; //边界强度，根据上述通道强度来计算，调用UpdateBorderStrength()函数来更新。
	
	float RollAngle; //横滚角；单位：度
	float PitchAngle; //俯仰角；单位：度
	float TiltAngle;  //倾斜角；单位：度
	float YawAngle; //航向角；单位：度
	
	float TargetAngle; //目标航向角；单位：度
	uint8_t TurnDir; //转向控制，D0=0顺时针，=1逆时针；D1=0 固定角度 =1 随机角度；D2=0 固定角度180度，=1固定角度90度
	
	struct R1_RunParam_s sRunParam; //运行参数
	struct R1_WorkTime_s sWorkTime[7];  //周一到周日工作时间（时：分）
	
	struct HMC5883_s sHMC5883;
	struct MMA7660_s sMMA7660;
	
	char * FailInfo; //失败信息
	
};

extern struct R1Mower_s sR1Mower;

void UpdateBorderStrength(void);

void R1_Default(void);
bool R1_is_low_battery(void);
bool R1_has_fail(void);
bool R1_is_working_time(void);
bool R1_is_go_home(void);
bool R1_is_raining(void);
bool R1_is_charging(void);
bool R1_is_full_charging(void);
bool R1_is_crashing(void);
bool R1_is_out_area(void); //在边界外？
bool R1_is_near_middle_line(void);   //R1是否在中线附近
uint8_t R1_is_in_which_area(void); //R1在哪个线圈内，返回0，1


#endif /* __R1DEF_H */
