/**
  ******************************************************************************
  * @file USART/Interrupt/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "stm32f10x_usart.h"
#include "clock_calendar.h"
#include "R1MT.h"
//#include "NRF24L01.h"
//#include "I2CRoutines.h"
//#include "HMC5883.h"
#include "stepmotor.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


// extern void TimingDelay_Decrement(void);

extern uint8_t TimeSecTick;
// uint16_t SummerTimeCorrect;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
// 	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2) == 0) GPIO_SetBits(GPIOD,GPIO_Pin_2);
// 	else GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	

}


/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{

  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
	
	SM_Run();
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/



/**
  * @brief  This function handles RTC_IRQHandler .
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
//   uint8_t Month,Day;
//   uint16_t Year;
//   
//   Month = BKP_ReadBackupRegister(BKP_DR2);
//   Day = BKP_ReadBackupRegister(BKP_DR3);
//   Year = BKP_ReadBackupRegister(BKP_DR4);
  NVIC_ClearPendingIRQ(RTC_IRQn);
  RTC_ClearITPendingBit(RTC_IT_SEC);
	
	TimeSecTick = 1;
  	
  /* If counter is equal to 86399: one day was elapsed */
  /* This takes care of date change and resetting of counter in case of
  power on - Run mode/ Main supply switched on condition*/
  if(RTC_GetCounter() == 86399)
  {
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Reset counter value */
    RTC_SetCounter(0x0);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Increment the date */
    DateUpdate();
  }
	
// 	__NOP();

//   if((RTC_GetCounter()/3600 == 1)&&(((RTC_GetCounter()%3600)/60) == 59)&&
//      (((RTC_GetCounter()%3600)%60) == 59))
//   {
//     /* March Correction */
//     if((Month == 3) && (Day >24))
//     {
//       if(WeekDay(Year,Month,Day)==0)
//       {
//         if((SummerTimeCorrect & 0x8000) == 0x8000)
//         {
//           RTC_SetCounter(RTC_GetCounter() + 3601);
//          
//           /* Reset March correction flag */
//           SummerTimeCorrect &= 0x7FFF;
//          
//           /* Set October correction flag  */
//           SummerTimeCorrect |= 0x4000;
//           SummerTimeCorrect |= Year;
//           BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
//         }
//       }
//     }
//       /* October Correction */
//     if((Month == 10) && (Day >24))
//     {
//       if(WeekDay(Year,Month,Day)==0)
//       {
//         if((SummerTimeCorrect & 0x4000) == 0x4000)
//         {
//           RTC_SetCounter(RTC_GetCounter() - 3599);
//           
//           /* Reset October correction flag */
//           SummerTimeCorrect &= 0xBFFF;
//           
//           /* Set March correction flag  */
//           SummerTimeCorrect |= 0x8000;
//           SummerTimeCorrect |= Year;
//           BKP_WriteBackupRegister(BKP_DR7,SummerTimeCorrect);
//         }
//       }
//     }
//   }
}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
