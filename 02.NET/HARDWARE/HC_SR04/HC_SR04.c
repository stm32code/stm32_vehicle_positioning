#include "HC_SR04.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////

u16 TIM3_UPDATA = 0;
u32 temp1 = 0;

void HC_SR04_IO2_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;			  // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  // �����趨������ʼ��GPIOB3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			  // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  // �����趨������ʼ��GPIOB4
}
u16 Get_SR04_Distance2(void)
{

	SR04_Trlg2 = 1; // �����ź��Ǹߵ�ƽ���壬��ȴ���10us
	delay_us(20);
	SR04_Trlg2 = 0;
	while (!SR04_Echo2)
		;					 // �ȴ��ߵ�ƽ
	TIM_SetCounter(TIM3, 0); // ���������ֵ
	TIM3_UPDATA = 0;		 // �����������
	while (SR04_Echo2)
		;																			 // �ȴ��͵�ƽ
	TIM_Cmd(TIM3, DISABLE);															 // ��ʱ�رն�ʱ������֤������ȷ��
	temp1 = (int)(((double)(TIM_GetCounter(TIM3) + (7200 * TIM3_UPDATA))) / 72 / 2); // �õ��ߵ�ƽ��ʱ�䣬��λus����ʱ���ļ���ֵ���������ֵ���Ǹߵ�ƽ��ʱ�䣩������2�Ǽ��㵥�̵�ʱ��
	// ��7200* TIM2_UPDATA����Ϊ�����ʱ��us������ת��Ϊ100* TIM2_UPDATA,��Ϊ�ڹ�ʽ���������72�����������ΪTIM2_UPDATA��100us��
	TIM_Cmd(TIM3, ENABLE);
	return temp1;
}
void TIM3_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ʹ��TIM2ʱ��

	// ��ʼ����ʱ��3
	TIM_TimeBaseStructure.TIM_Period = arr;						// �趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				// ����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	// �жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			  // TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // ��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // �����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ��������ж� ,

	// TIM_Cmd(TIM3, ENABLE); // ʹ�ܶ�ʱ��2
}
// ��ʱ��3�жϷ������
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) // ���ָ����TIM�жϷ������:TIM �ж�Դ
	{
		TIM3_UPDATA++;								// �������źźܳ�ʱ����¼���������ÿ��һ�δ����100us
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ����жϱ�־λ
	}
}
