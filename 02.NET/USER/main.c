#include "git.h"

// ���ʲ�����ʼ��
uint32_t aun_ir_buffer[500];  // IR LED sensor data
int32_t n_ir_buffer_length;   // data length
uint32_t aun_red_buffer[500]; // Red LED sensor data
int32_t n_sp02;               // SPO2 value
int8_t ch_spo2_valid;         // indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;         // heart rate value
int8_t ch_hr_valid;           // indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
// variables to calculate the on-board LED brightness that reflects the heartbeats
uint32_t un_min, un_max, un_prev_data;
int i;
int32_t n_brightness;
float f_temp;
u8 temp_num = 0;
u8 temp[6];
char str[50];
u8 dis_hr = 0, dis_spo2 = 0;

#define MAX_BRIGHTNESS 255
// �����ʱ���趨
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;
u8 Connect_Net;
// ��ȡȫ�ֱ���
const char *topics[] = {S_TOPIC_NAME};

// Ӳ����ʼ��
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    HZ = GB16_NUM();                                // ����
    delay_init();                                   // ��ʱ������ʼ��
    GENERAL_TIM_Init(TIM_4, 0, 1);
    Usart1_Init(115200); // ����1��ʼ��Ϊ115200
		Usart2_Init(9600);   // GPS
    Usart3_Init(9600); // ����3������GA10��
    System_PB34_setIO();
    LED_Init();
    Key_GPIO_Config(); // key
		DHT11_GPIO_Config(); // ��ʪ��
	  RELAY_GPIO_Config(); // ������ ����
		Level_GPIO_Config(); // ����
		ADCx_Init(); // adc
	
		relay1out = 0;
		Beepout = 0;
	
#if OLED // OLED�ļ�����
    OLED_Init();
    OLED_ColorTurn(0);   // 0������ʾ��1 ��ɫ��ʾ
    OLED_DisplayTurn(0); // 0������ʾ 1 ��Ļ��ת��ʾ
#endif
	
			
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // ��ʼ����ֵ

#if OLED // OLED�ļ�����
    OLED_Clear();
#endif
}
// �����ʼ��
void Net_Init()
{

#if OLED // OLED�ļ�����
    char str[50];
    OLED_Clear();
    // дOLED����
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "---Net Break---");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
#endif
    GA10_Init(); // ��ʼ��GPRSģ��

    Connect_Net = 60; // �����ɹ�
#if OLED              // OLED�ļ�����
    OLED_Clear();
#endif
}

// ����1
void task1(void)
{
	//1�������
 	Automation_Close();
	//��λ�ɹ�����������
	if (device_state_init.location_state == 1)
	{
		W_Test();
	}

	Update_oled_massage();   // ����OLED
}
// ����2
void task2(void)
{
    Read_Data(&Data_init);   // ���´���������
    Update_device_massage(); // �����豸
    State = ~State;
}
// ����3
void task3(void)
{
	
	if (Data_init.App == 0) {
		Data_init.App = 1;
	}
	//��λ�ɹ�����������
	if (device_state_init.location_state == 1)
	{
		W_Test();
	}
}
// �����ʼ��
void SoftWare_Init(void)
{
    // ��ʱ����ʼ��
    timer_init(&task1_id, task1, 1000, 1); // 1sִ��һ��
    timer_init(&task2_id, task2, 250, 1);  // 250msִ��һ��
    timer_init(&task3_id, task3, 5000, 1); // 5sִ��һ��

    timer_start(&task1_id);
    timer_start(&task2_id);
    timer_start(&task3_id);
	
	
}
// ������
int main(void)
{

    char *dataPtr = NULL;
    SoftWare_Init(); // �����ʼ��
    Hardware_Init(); // Ӳ����ʼ��
    // ������ʾ
    BEEP = 1;
    delay_ms(100);
    BEEP = 0;
    Net_Init(); // �����ʼ
    TIM_Cmd(TIM4, ENABLE); // ʹ�ܼ�����
    while (1) {

        // �߳�
        timer_loop(); // ��ʱ��ִ��
        // ���ڽ����ж�
        dataPtr = (char*)ESP8266_GetIPD(0);
        if (dataPtr != NULL) {
            massage_ga10_json(dataPtr); // ��������
        }
#if KEY_OPEN
				// �������
				if(time25ms == MY_TRUE){
						Check_Key_ON_OFF();
						time25ms = MY_FALSE;
				}
#endif
    }
}

