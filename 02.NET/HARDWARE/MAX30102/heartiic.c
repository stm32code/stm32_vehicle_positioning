#include "heartiic.h"
#include "delay.h"

// ��ʼ��IIC
void heart_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DATA_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DATA__GPIO_PIN; // PC.11  CE
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
	GPIO_Init(DATA__GPIO_PORT, &GPIO_InitStructure); // ��ʼ��GPIOC.11

	GPIO_InitStructure.GPIO_Pin = SCLK__GPIO_PIN; // PC.12  SCLK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
	GPIO_Init(SCLK__GPIO_PORT, &GPIO_InitStructure); // ��ʼ��GPIOC.12

	Heart_IIC_SCL = 1;
	Heart_IIC_SDA = 1;
}
// ����˫��I/O�˿�Ϊ���̬
static void Heart_SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DATA_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DATA__GPIO_PIN; // PC.10  DATA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DATA__GPIO_PORT, &GPIO_InitStructure); // ��ʼ��GPIOC.10
}
// ����˫��I/O�˿�Ϊ����̬
static void Heart_SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DATA_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DATA__GPIO_PIN; // PC.10 DATA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DATA__GPIO_PORT, &GPIO_InitStructure); // ��ʼ��GPIOC.10
}

// ����IIC��ʼ�ź�
void heart_IIC_Start(void)
{
	Heart_SDA_OUT(); // sda�����
	Heart_IIC_SDA = 1;
	Heart_IIC_SCL = 1;
	delay_us(4);
	Heart_IIC_SDA = 0; // START:when CLK is high,DATA change form high to low
	delay_us(4);
	Heart_IIC_SCL = 0; // ǯסI2C���ߣ�׼�����ͻ��������
}
// ����IICֹͣ�ź�
void heart_IIC_Stop(void)
{
	Heart_SDA_OUT(); // sda�����
	Heart_IIC_SCL = 0;
	Heart_IIC_SDA = 0; // STOP:when CLK is high DATA change form low to high
	delay_us(4);
	Heart_IIC_SCL = 1;
	Heart_IIC_SDA = 1; // ����I2C���߽����ź�
	delay_us(4);
}
// �ȴ�Ӧ���źŵ���
// ����ֵ��1������Ӧ��ʧ��
//         0������Ӧ��ɹ�
u8 heart_IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	Heart_SDA_IN(); // SDA����Ϊ����
	Heart_IIC_SDA = 1;
	delay_us(1);
	Heart_IIC_SCL = 1;
	delay_us(1);
	while (Heart_READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			heart_IIC_Stop();
			return 1;
		}
	}
	Heart_IIC_SCL = 0; // ʱ�����0
	return 0;
}
// ����ACKӦ��
void heart_IIC_Ack(void)
{
	Heart_IIC_SCL = 0;
	Heart_SDA_OUT();
	Heart_IIC_SDA = 0;
	delay_us(2);
	Heart_IIC_SCL = 1;
	delay_us(2);
	Heart_IIC_SCL = 0;
}
// ������ACKӦ��
void heart_IIC_NAck(void)
{
	Heart_IIC_SCL = 0;
	Heart_SDA_OUT();
	Heart_IIC_SDA = 1;
	delay_us(2);
	Heart_IIC_SCL = 1;
	delay_us(2);
	Heart_IIC_SCL = 0;
}
// IIC����һ���ֽ�
// ���شӻ�����Ӧ��
// 1����Ӧ��
// 0����Ӧ��
void heart_IIC_Send_Byte(u8 txd)
{
	u8 t;
	Heart_SDA_OUT();
	Heart_IIC_SCL = 0; // ����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		Heart_IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		delay_us(2); // ��TEA5767��������ʱ���Ǳ����
		Heart_IIC_SCL = 1;
		delay_us(2);
		Heart_IIC_SCL = 0;
		delay_us(2);
	}
}
// ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 heart_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	Heart_SDA_IN(); // SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		Heart_IIC_SCL = 0;
		delay_us(2);
		Heart_IIC_SCL = 1;
		receive <<= 1;
		if (Heart_READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		heart_IIC_NAck(); // ����nACK
	else
		heart_IIC_Ack(); // ����ACK
	return receive;
}

void IIC_WriteBytes(u8 WriteAddr, u8 *data, u8 dataLength)
{
	u8 i;
	heart_IIC_Start();

	heart_IIC_Send_Byte(WriteAddr); // ����д����
	heart_IIC_Wait_Ack();

	for (i = 0; i < dataLength; i++)
	{
		heart_IIC_Send_Byte(data[i]);
		heart_IIC_Wait_Ack();
	}
	heart_IIC_Stop(); // ����һ��ֹͣ����
	delay_ms(10);
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr, u8 *data, u8 dataLength)
{
	u8 i;
	heart_IIC_Start();

	heart_IIC_Send_Byte(deviceAddr); // ����д����
	heart_IIC_Wait_Ack();
	heart_IIC_Send_Byte(writeAddr);
	heart_IIC_Wait_Ack();
	heart_IIC_Send_Byte(deviceAddr | 0X01); // �������ģʽ
	heart_IIC_Wait_Ack();

	for (i = 0; i < dataLength - 1; i++)
	{
		data[i] = heart_IIC_Read_Byte(1);
	}
	data[dataLength - 1] = heart_IIC_Read_Byte(0);
	heart_IIC_Stop(); // ����һ��ֹͣ����
	delay_ms(10);
}

void heart_IIC_Read_One_Byte(u8 daddr, u8 addr, u8 *data)
{
	heart_IIC_Start();

	heart_IIC_Send_Byte(daddr); // ����д����
	heart_IIC_Wait_Ack();
	heart_IIC_Send_Byte(addr); // ���͵�ַ
	heart_IIC_Wait_Ack();
	heart_IIC_Start();
	heart_IIC_Send_Byte(daddr | 0X01); // �������ģʽ
	heart_IIC_Wait_Ack();
	*data = heart_IIC_Read_Byte(0);
	heart_IIC_Stop(); // ����һ��ֹͣ����
}

void heart_IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data)
{
	heart_IIC_Start();

	heart_IIC_Send_Byte(daddr); // ����д����
	heart_IIC_Wait_Ack();
	heart_IIC_Send_Byte(addr); // ���͵�ַ
	heart_IIC_Wait_Ack();
	heart_IIC_Send_Byte(data); // �����ֽ�
	heart_IIC_Wait_Ack();
	heart_IIC_Stop(); // ����һ��ֹͣ����
	delay_ms(10);
}
