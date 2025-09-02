#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "git.h"
/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define State_GPIO_PORT GPIOC               /* GPIO�˿� */
#define State_GPIO_CLK RCC_APB2Periph_GPIOC /* GPIO�˿�ʱ�� */
#define State_GPIO_PIN GPIO_Pin_13


#define LED1_GPIO_PORT GPIOA               /* GPIO�˿� */
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOA /* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN GPIO_Pin_0

#define LED2_GPIO_PORT GPIOB               /* GPIO�˿� */
#define LED2_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define LED2_GPIO_PIN GPIO_Pin_7

#define LED3_GPIO_PORT GPIOB               /* GPIO�˿� */
#define LED3_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define LED3_GPIO_PIN GPIO_Pin_8

#define LED4_GPIO_PORT GPIOB               /* GPIO�˿� */
#define LED4_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define LED4_GPIO_PIN GPIO_Pin_9

#define State PCout(13) // LED�ӿ�
#define BEEP PAout(0)   // LED�ӿ�

#define LED1 PBout(4)   // LED�ӿ�
#define LED2 PBout(7)   // LED�ӿ�
#define LED3 PBout(8)   // LED�ӿ�
#define LED4 PBout(9)   // LED�ӿ�

#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)

#define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_ON digitalHi(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF digitalLo(LED2_GPIO_PORT, LED2_GPIO_PIN)

#define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_ON digitalHi(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF digitalLo(LED3_GPIO_PORT, LED3_GPIO_PIN)

#define LED4_TOGGLE digitalToggle(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_ON digitalHi(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_OFF digitalLo(LED4_GPIO_PORT, LED4_GPIO_PIN)

#define OFF 0
#define ON 1

void LED_Init(void); // ��ʼ��
/**
 * @brief  MQTT����LEDģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_LED(U8 Cmd);

void System_PB34_setIO(void);
#endif
