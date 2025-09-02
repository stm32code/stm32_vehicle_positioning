#ifndef __MAX30102_H
#define __MAX30102_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////

#define IN_GPIO_PORT GPIOB               /* GPIO�˿� */
#define IN_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define IN_GPIO_PIN GPIO_Pin_7           /* ���ӵ�SCLʱ���ߵ�GPIO */

#define SCLK__GPIO_PORT GPIOB              /* GPIO�˿� */
#define SCLK_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define SCLK__GPIO_PIN GPIO_Pin_9          /* ���ӵ�SCLʱ���ߵ�GPIO */

#define DATA__GPIO_PORT GPIOB              /* GPIO�˿� */
#define DATA_GPIO_CLK RCC_APB2Periph_GPIOB /* GPIO�˿�ʱ�� */
#define DATA__GPIO_PIN GPIO_Pin_8          /* ���ӵ�SCLʱ���ߵ�GPIO */

// IO��������
#define Heart_IIC_SCL PBout(9) // SCL
#define Heart_IIC_SDA PBout(8) // SDA
#define Heart_READ_SDA PBin(8) // ����SDA

// IIC���в�������
void heart_IIC_Init(void);                 // ��ʼ��IIC��IO��
void heart_IIC_Start(void);                // ����IIC��ʼ�ź�
void heart_IIC_Stop(void);                 // ����IICֹͣ�ź�
void heart_IIC_Send_Byte(u8 txd);          // IIC����һ���ֽ�
u8 heart_IIC_Read_Byte(unsigned char ack); // IIC��ȡһ���ֽ�
u8 heart_IIC_Wait_Ack(void);               // IIC�ȴ�ACK�ź�
void heart_IIC_Ack(void);                  // IIC����ACK�ź�
void heart_IIC_NAck(void);                 // IIC������ACK�ź�

void heart_IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
void heart_IIC_Read_One_Byte(u8 daddr, u8 addr, u8 *data);

void IIC_WriteBytes(u8 WriteAddr, u8 *data, u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr, u8 *data, u8 dataLength);


#endif
