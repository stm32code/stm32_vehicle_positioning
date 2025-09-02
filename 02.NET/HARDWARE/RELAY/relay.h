#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define BEEP_GPIO_PORT GPIOB
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN GPIO_Pin_7

#define RELAY1_GPIO_PORT GPIOB
#define RELAY1_GPIO_CLK RCC_APB2Periph_GPIOB
#define RELAY1_GPIO_PIN GPIO_Pin_8

#define RELAY2_GPIO_PORT GPIOB
#define RELAY2_GPIO_CLK RCC_APB2Periph_GPIOB
#define RELAY2_GPIO_PIN GPIO_Pin_9

#define Beepout PBout(7)   // BEEP�ӿ�
#define Beepin PBin(7)   // BEEP�ӿ�

#define relay1in PBin(8)


#define relay1out PBout(8) // �ӿ�


void BeepNo(u16 tiem);
void RELAY_GPIO_Config(void);

#endif /* __RELAY_H */
