#include "exti.h"

extern Device_Satte_Typedef device_state_init; // �豸״̬
extern U8 MG996R_TEST;                         // ���ٲ���

/**
 * @brief  ����Ƕ�������жϿ�����NVIC
 * @param  ��
 * @retval ��
 */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* �����ж�Դ������2������ʹ������������� */
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * @param  ��
 * @retval ��
 */
void EXTI_Key_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /*��������GPIO�ڵ�ʱ��*/
  RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK, ENABLE);

  /* ���� NVIC �ж�*/
  NVIC_Configuration();

  /*--------------------------KEY1����-----------------------------*/
  /* ѡ�񰴼��õ���GPIO */
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* ����Ϊ�������� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

  /* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;

  /* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /*--------------------------KEY2����-----------------------------*/
  /* ѡ�񰴼��õ���GPIO */
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* ����Ϊ�������� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

  /* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;

  /* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  /* �½����ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void KEY1_IRQHandler(void)
{
  // ȷ���Ƿ������EXTI Line�ж�
  if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
  {
    if (!device_state_init.open)
    {
      device_state_init.open = 1;
      MG996R_TEST = 5;
    }
    // ����жϱ�־λ
    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
  }
  // ȷ���Ƿ������EXTI Line�ж�
  if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
  {
    if (device_state_init.open)
    {
      device_state_init.open = 0;
      device_state_init.check_open = 0;
    }
    // ����жϱ�־λ
    EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
  }
}

// void KEY2_IRQHandler(void)
//{
//
// }

/*********************************************END OF FILE**********************/
