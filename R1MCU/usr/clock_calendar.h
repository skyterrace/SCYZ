/**
  ******************************************************************************
  * @file RTC/inc/clock_calendar.h 
  * @author  MCD Application Team
  * @version  V2.0.0
  * @date  04/27/2009
  * @brief  This files contains the Clock Calendar functions prototypes
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CLOCK_CALENDAR_H
#define __CLOCK_CALENDAR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* Exported types ------------------------------------------------------------*/
/* Time Structure definition */
struct Time_s
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
};
extern struct Time_s s_TimeStructVar;

/* Alarm Structure definition */
struct AlarmTime_s
{
  uint8_t SecLow;
  uint8_t SecHigh;
  uint8_t MinLow;
  uint8_t MinHigh;
  uint8_t HourLow;
  uint8_t HourHigh;
};
extern struct AlarmTime_s s_AlarmStructVar;

/* Date Structure definition */
struct Date_s
{
  uint8_t Month;
  uint8_t Day;
  uint16_t Year;
};
extern struct Date_s s_DateStructVar;

/* Alarm Date Structure definition */
struct AlarmDate_s
{
  uint8_t Month;
  uint8_t Day;
  uint16_t Year;
};
extern struct AlarmDate_s s_AlarmDateStructVar;


/* Exported constants --------------------------------------------------------*/
#define BATTERY_REMOVED 98
#define BATTERY_RESTORED 99
#define SECONDS_IN_DAY 86399
#define CONFIGURATION_DONE 0xAAAA
#define CONFIGURATION_RESET 0x0000
#define OCTOBER_FLAG_SET 0x4000
#define MARCH_FLAG_SET 0x8000
#define DEFAULT_DAY 1
#define DEFAULT_MONTH 1
#define DEFAULT_YEAR 2012
#define DEFAULT_HOURS 9
#define DEFAULT_MINUTES 1
#define DEFAULT_SECONDS 0
#define LEAP 1
#define NOT_LEAP 0

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
// void CalendarInit(void);
void RTC_Configuration(void);
void SetTime(uint8_t,uint8_t,uint8_t);
void SetAlarm(uint8_t,uint8_t,uint8_t);
void SetDate(uint8_t,uint8_t,uint16_t);
// void RTC_Application(void);
void DateUpdate(void);
uint16_t WeekDay(uint16_t,uint8_t,uint8_t);
uint8_t CheckLeap(uint16_t);
void CalculateTime(void);
// void DisplayDate(void);
// void DisplayTime(void);
// void DisplayAlarm(void);
// void RTC_NVIC_Configuration(void);
// void ApplicationInit(void);
// void GPIO_Configuration(void);
// void DelayLowPower(__IO uint32_t nCount);
// void SysTickConfig(void);
void CheckForDaysElapsed(void);
// void SummerTimeCorrection(void);
// void Tamper_NVIC_Configuration(void);
// void ManualClockCalibration(void);
// void AutoClockCalibration(void);
// void ReturnFromStopMode(void);

//定时延时程序
static __IO uint32_t TimingDelay; //SysTick计数变量
static __IO uint32_t TimeStamp_ms; //毫秒绝对值
void TimingDelay_Decrement(void);//获取节拍程序
void Delay_us(__IO uint32_t nTime);//定时延时程序 10us为单位
void Delay_ms(__IO uint32_t nTime);//定时延时程序 1ms为单位
uint32_t GetTimeStamp_ms(void);
//定时延时程序结束

#endif /* __CLOCK_CALENDAR_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

