#include "timer.h"
#include "delay.h"
#include "task.h"
#include "usart.h"
#include "stdbool.h"

// �����豸����
#include "usart3.h"
#include "Net.h"
/*********************************************************************************
 * @Function	: ͨ�ö�ʱ����ѡ��TIM2-TIM5��
 *	@Author   : HR
 * @Date			: 2022-08-30
 **********************************************************************************/
#if GENERAL_TIM2
u32 time_2 = 0; // ms ��ʱ����
extern u8 Connect_Net;
#endif
#if GENERAL_TIM3
u32 time_3 = 0; // ms ��ʱ����
extern u8 Connect_Net;
#endif
#if GENERAL_TIM4
u32 time_4 = 0; // ms ��ʱ����
u8 time25ms = MY_FALSE;
extern u8 Connect_Net;

U16 Usart2_Time = 0;		// ms ��ʱ����
U32 TimeCount1ms = 0;		/** < 10���볬ʱ���� */
U8 bTimeout10ms = MY_FALSE; /** < 10���붨ʱ����ʱ��־ */

#endif
#if GENERAL_TIM5
U16 time_5 = 0; // ms ��ʱ����
#endif
#if GENERAL_TIM6
U16 time_6 = 0; // ms ��ʱ����
#endif
#if GENERAL_TIM7
U16 Usart1_Time = 0;		 // ms ��ʱ����
U16 Usart2_Time = 0;		 // ms ��ʱ����
U16 Usart3_Time = 0;		 // ms ��ʱ����
U16 Usart4_Time = 0;		 // ms ��ʱ����
U16 Usart5_Time = 0;		 // ms ��ʱ����
U8 bTimeout1s = MY_FALSE;	 /** < 1���Ӷ�ʱ����ʱ��־ */
U8 bTimeout2s = MY_FALSE;	 /** < 2���Ӷ�ʱ����ʱ��־ */
U8 bTimeout500ms = MY_FALSE; /** < 500���붨ʱ����ʱ��־ */
U8 bTimeout250ms = MY_FALSE; /** < 50���붨ʱ����ʱ��־ */
U8 bTimeout10ms = MY_FALSE;	 /** < 10���붨ʱ����ʱ��־ */
U32 TimeCount1ms = 0;		 /** < 10���볬ʱ���� */
#endif
/*********************************************************************************
 * @Function	: TIM��ʱ��ѡ��
 * @Input		: time,TIM��ʱ����TIM_2-TIM_5��
 * @Output		:	TIMģ��
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
static TIM_TypeDef *TimeModule_get(TimeModule time)
{
	TIM_TypeDef *timeTemp;
	switch ((u8)time) // UARTģ��ѡ��
	{
#if GENERAL_TIM2
	case TIM_2:
		timeTemp = TIM2;
		break;
#endif
#if GENERAL_TIM3
	case TIM_3:
		timeTemp = TIM3;
		break;
#endif
#if GENERAL_TIM4
	case TIM_4:
		timeTemp = TIM4;
		break;
#endif
#if GENERAL_TIM5
	case TIM_5:
		timeTemp = TIM5;
		break;
#endif
#if GENERAL_TIM6
	case TIM_6:
		timeTemp = TIM6;
		break;
#endif
#if GENERAL_TIM7
	case TIM_7:
		timeTemp = TIM7;
		break;
#endif
	}
	return timeTemp;
}
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
void GENERAL_TIM_NVIC_Config(TimeModule time, u8 PreemptionPriority, u8 SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж���Ϊ0
	// Usart NVIC ����
	switch ((u8)time) // �ж�ͨ������
	{
	case TIM_2:
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		break; // TIM2�ж�
	case TIM_3:
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		break;
	case TIM_4:
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		break;
		//	case TIM_5:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
		//		break;
		//	case TIM_6:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
		//		break;
		//	case TIM_7:
		//		NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
		//		break;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;			   // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							   // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);											   // ����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
/*********************************************************************************
 * @Function	: TIMʱ��ʹ��
 * @Input		: time,timeʱ�ӣ�TIM_1-TIM_5��
 * @Output		:	None
 * @Return		: None
 * @Others		:	None
 * @Date			: 2022-08-30
 **********************************************************************************/
static void GENERAL_TIMClock_Enable(TimeModule time)
{
	switch ((u8)time)
	{
	case TIM_2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	case TIM_3:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	case TIM_4:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	case TIM_5:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	case TIM_6:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	case TIM_7:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
		break; // ͨ�ö�ʱ��2ʱ��ʹ��
	}
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef;
 *-----------------------------------------------------------------------------
 */
static void GENERAL_TIM_Mode_Config(TimeModule time)
{
	TIM_TypeDef *timeTemp;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	timeTemp = TimeModule_get(time); // TIMEʱ��ѡ��
	switch ((u8)time)
	{
	case TIM_2:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM2_Period; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM2_Prescaler;
		break; // ʱ��Ԥ��Ƶ��
	case TIM_3:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM3_Period - 1; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM3_Prescaler - 1;
		break; // ʱ��Ԥ��Ƶ��
	case TIM_4:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM4_Period; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM4_Prescaler;
		break; // ʱ��Ԥ��Ƶ��
	case TIM_5:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM5_Period - 1; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM5_Prescaler - 1;
		break; // ʱ��Ԥ��Ƶ��
	case TIM_6:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM6_Period - 1; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM6_Prescaler - 1;
		break; // ʱ��Ԥ��Ƶ��
	case TIM_7:
		TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM7_Period - 1; // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
		TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM7_Prescaler - 1;
		break; // ʱ��Ԥ��Ƶ��
	}
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(timeTemp, &TIM_TimeBaseStructure);
	// ����������жϱ�־λ
	TIM_ClearFlag(timeTemp, TIM_FLAG_Update);
	// �����������ж�
	TIM_ITConfig(timeTemp, TIM_IT_Update, ENABLE);
	// ʹ�ܼ�����
	TIM_Cmd(timeTemp, DISABLE); // �رն�ʱ��
}
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
void GENERAL_TIM_Init(TimeModule time, u8 PreemptionPriority, u8 SubPriority)
{
	GENERAL_TIMClock_Enable(time);									// TIMʹ�ܻ�
	GENERAL_TIM_NVIC_Config(time, PreemptionPriority, SubPriority); // TIM�ж����ȼ�����
	GENERAL_TIM_Mode_Config(time);									// TIM��ʼ��
}


/*********************************************************************************
 * @Function	:  TIMER4��ʱ���жϷ���
 * @Input		:  None
 * @Output		:	 None
 * @Return		:  None
 * @Others		:	 None
 * @Date			:  2022-08-30
 **********************************************************************************/
#if GENERAL_TIM4
void TIM4_IRQHandler(void) // TIM4�ж�
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) // ���TIM3�����жϷ������
	{

		time_4++; // ʱ���¼��
		timer_ticks();
#if KEY_OPEN
		if (time_4 % 25 == 0)
		{
			time25ms = MY_TRUE;
		}
#endif
#if USART2_OPEN
		// ���������ض�ʱ��
		if (time_4 % 10 == 0)
		{
			TimeCount1ms++;
			if (bTimeout10ms == MY_TRUE)
			{

				if (Usart2_Time > 0)
				{
					Usart2_Time++; // ��������ռ���ж�
				}

				// ���������ض�ʱ��
				if (Usart2_Time > 10)
				{
					bTimeout10ms = MY_FALSE;
				}
			}
		}
#endif

		// һ�붨ʱ��
		if (time_4 % 1000 == 0)
		{
			time_4 = 0;
			Data_init.Time++;
			// 20S �������ݵ�ƽ̨
			if (Connect_Net && Data_init.Time % 20 == 0 && Data_init.App == 0) {
					// Data_init.App = 2;
			}
#ifdef NETWORK_CHAEK
			if (Connect_Net > 0)
			{
				Connect_Net--;
			}
#endif
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // ���TIMx�����жϱ�־
	}
}
#endif

