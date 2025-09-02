#include "level.h"
#include "delay.h"

/*********************************************************************************
 * @Function	:  ��ʼ�����Ƶ�IO
 **********************************************************************************/
void Level_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*���������˿ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(LEVEL1_GPIO_CLK, ENABLE);

    // ѡ�񰴼�������
    GPIO_InitStructure.GPIO_Pin = LEVEL1_GPIO_PIN;
    // ���ð���������Ϊ��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // ʹ�ýṹ���ʼ������
    GPIO_Init(LEVEL1_GPIO_PORT, &GPIO_InitStructure);


}
