#include "git.h"

// 心率参数初始化
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
// 软件定时器设定
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;
extern u8 time25ms;
u8 Connect_Net;
// 获取全局变量
const char *topics[] = {S_TOPIC_NAME};

// 硬件初始化
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    HZ = GB16_NUM();                                // 字数
    delay_init();                                   // 延时函数初始化
    GENERAL_TIM_Init(TIM_4, 0, 1);
    Usart1_Init(115200); // 串口1初始化为115200
		Usart2_Init(9600);   // GPS
    Usart3_Init(9600); // 串口3，驱动GA10用
    System_PB34_setIO();
    LED_Init();
    Key_GPIO_Config(); // key
		DHT11_GPIO_Config(); // 温湿度
	  RELAY_GPIO_Config(); // 蜂鸣器 风扇
		Level_GPIO_Config(); // 火焰
		ADCx_Init(); // adc
	
		relay1out = 0;
		Beepout = 0;
	
#if OLED // OLED文件存在
    OLED_Init();
    OLED_ColorTurn(0);   // 0正常显示，1 反色显示
    OLED_DisplayTurn(0); // 0正常显示 1 屏幕翻转显示
#endif
	
			
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // 初始化阈值

#if OLED // OLED文件存在
    OLED_Clear();
#endif
}
// 网络初始化
void Net_Init()
{

#if OLED // OLED文件存在
    char str[50];
    OLED_Clear();
    // 写OLED内容
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "---Net Break---");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
#endif
    GA10_Init(); // 初始化GPRS模块

    Connect_Net = 60; // 入网成功
#if OLED              // OLED文件存在
    OLED_Clear();
#endif
}

// 任务1
void task1(void)
{
	//1秒计算器
 	Automation_Close();
	//定位成功，保存数据
	if (device_state_init.location_state == 1)
	{
		W_Test();
	}

	Update_oled_massage();   // 更新OLED
}
// 任务2
void task2(void)
{
    Read_Data(&Data_init);   // 更新传感器数据
    Update_device_massage(); // 更新设备
    State = ~State;
}
// 任务3
void task3(void)
{
	
	if (Data_init.App == 0) {
		Data_init.App = 1;
	}
	//定位成功，保存数据
	if (device_state_init.location_state == 1)
	{
		W_Test();
	}
}
// 软件初始化
void SoftWare_Init(void)
{
    // 定时器初始化
    timer_init(&task1_id, task1, 1000, 1); // 1s执行一次
    timer_init(&task2_id, task2, 250, 1);  // 250ms执行一次
    timer_init(&task3_id, task3, 5000, 1); // 5s执行一次

    timer_start(&task1_id);
    timer_start(&task2_id);
    timer_start(&task3_id);
	
	
}
// 主函数
int main(void)
{

    char *dataPtr = NULL;
    SoftWare_Init(); // 软件初始化
    Hardware_Init(); // 硬件初始化
    // 启动提示
    BEEP = 1;
    delay_ms(100);
    BEEP = 0;
    Net_Init(); // 网络初始
    TIM_Cmd(TIM4, ENABLE); // 使能计数器
    while (1) {

        // 线程
        timer_loop(); // 定时器执行
        // 串口接收判断
        dataPtr = (char*)ESP8266_GetIPD(0);
        if (dataPtr != NULL) {
            massage_ga10_json(dataPtr); // 接收命令
        }
#if KEY_OPEN
				// 按键监测
				if(time25ms == MY_TRUE){
						Check_Key_ON_OFF();
						time25ms = MY_FALSE;
				}
#endif
    }
}

