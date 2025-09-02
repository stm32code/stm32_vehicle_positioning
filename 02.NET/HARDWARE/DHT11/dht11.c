#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

/**************************************************************************************
 * ��  �� : ��ʼ��DHT11�ź�����Ϊ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void DHT11_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);

	GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

/**************************************************************************************
 * ��  �� : ��ʼ��DHT11�ź�����Ϊ��������ģʽ
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

/**************************************************************************************
 * ��  �� : ʹDHT11-DATA���ű�Ϊ�������ģʽ
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

/**************************************************************************************
 * ��  �� : ��DHT11��ȡһ���ֽڣ�MSB����
 * ��  �� : ��
 * ����ֵ : uint8_t
 **************************************************************************************/
static uint8_t Read_Byte(void)
{
	uint8_t i, temp = 0;

	for (i = 0; i < 8; i++)
	{
		while (DHT11_DATA_IN() == Bit_RESET)
			; // ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����

		/*DHT11 ��23~27us�ĸߵ�ƽ��ʾ��0������68~74us�ĸߵ�ƽ��ʾ��1��  */
		delay_us(40); // ��ʱ40us,�����ʱ��Ҫ��������0������ʱ�伴��

		if (DHT11_DATA_IN() == Bit_SET) //  40us����Ϊ�ߵ�ƽ��ʾ���ݡ�1��
		{
			while (DHT11_DATA_IN() == Bit_SET)
				;								// �ȴ�����1�ĸߵ�ƽ����
			temp |= (uint8_t)(0x01 << (7 - i)); // �ѵ�7-iλ��1��MSB����
		}
		else // 40us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{
			temp &= (uint8_t) ~(0x01 << (7 - i)); // �ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}

/**************************************************************************************
 * ��  �� : һ�����������ݴ���Ϊ40bit����λ�ȳ�
 * ��  �� : 8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У���
 * ����ֵ : uint8_t
 **************************************************************************************/
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Mode_Out_PP(); // ���ģʽ

	DHT11_DATA_OUT(LOW); // ��������
	delay_ms(18);		 // ��ʱ18ms

	DHT11_DATA_OUT(HIGH); // �������� ������ʱ30us
	delay_us(30);		  // ��ʱ30us

	DHT11_Mode_IPU(); // ������Ϊ���� �жϴӻ���Ӧ�ź�

	if (DHT11_DATA_IN() == Bit_RESET) // �жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	{
		while (DHT11_DATA_IN() == Bit_RESET)
			; // ��ѯֱ���ӻ�������83us �͵�ƽ ��Ӧ�źŽ���
		while (DHT11_DATA_IN() == Bit_SET)
			; // ��ѯֱ���ӻ�������87us �ߵ�ƽ �����źŽ���

		/*��ʼ��������*/
		DHT11_Data->humi_int = Read_Byte();
		DHT11_Data->humi_deci = Read_Byte();
		DHT11_Data->temp_int = Read_Byte();
		DHT11_Data->temp_deci = Read_Byte();
		DHT11_Data->check_sum = Read_Byte();

		DHT11_Mode_Out_PP();  // ��ȡ���������Ÿ�Ϊ���ģʽ
		DHT11_DATA_OUT(HIGH); // ��������

		/*����ȡ�������Ƿ���ȷ*/
		if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
			return SUCCESS;
		else
			return ERROR;
	}
	else
	{
		return ERROR;
	}
}
