#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
/**************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ����Ҫ�ĸ���ʱ����ʱ��ֻ��Ҫ������ĺ궨��ĳ�1����
#define GENERAL_TIM2 0
#define GENERAL_TIM3 0
#define GENERAL_TIM4 1
#define GENERAL_TIM5 0
#define GENERAL_TIM6 0
#define GENERAL_TIM7 0
/*********************************************************************************
 * @Function	: TIMEʵ��
 **********************************************************************************/
typedef enum
{
	TIM_2,
	TIM_3,
	TIM_4,
	TIM_5,
	TIM_6,
	TIM_7,
	TIME_NUM
} TimeModule;
/*********************************************************************************
 * @Function	: �̶���Ƶ��
 **********************************************************************************/
#define GENERAL_TIM2_Period (1000) // 1msһ���ж�
#define GENERAL_TIM2_Prescaler 72  // ʱ��Ԥ��Ƶ��

#define GENERAL_TIM3_Period (1000) // 1msһ���ж�
#define GENERAL_TIM3_Prescaler 72	   // ʱ��Ԥ��Ƶ��

#define GENERAL_TIM4_Period (1000) // 1msһ���ж�
#define GENERAL_TIM4_Prescaler 72	   // ʱ��Ԥ��Ƶ��

#define GENERAL_TIM5_Period (1000 - 1) // 1msһ���ж�
#define GENERAL_TIM5_Prescaler 71	   // ʱ��Ԥ��Ƶ��

#define GENERAL_TIM6_Period 90	  // 100usһ���ж�
#define GENERAL_TIM6_Prescaler 80 // ʱ��Ԥ��Ƶ��

#define GENERAL_TIM7_Period (1000) // 1msһ���ж�
#define GENERAL_TIM7_Prescaler 72  // ʱ��Ԥ��Ƶ��
/*********************************************************************************
 * @Function	: TIM2-5 ��ʼ��
 * @Input		: time,TIM��ʱ����TIM_2-TIM_5��
 *						PreemptionPriority����ռ���ȼ�
 *						SubPriority�������ȼ�
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
void GENERAL_TIM_Init(TimeModule time, u8 PreemptionPriority, u8 SubPriority);
/*********************************************************************************
 * @Function	: TIM��ʱ���жϳ�ʼ��
 * @Input		: time,TIM��ʱ����TIM_2-TIM_5��
 *						PreemptionPriority����ռ���ȼ�
 *						SubPriority�������ȼ�
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
void GENERAL_TIM_NVIC_Config(TimeModule time, u8 PreemptionPriority, u8 SubPriority);
#endif
