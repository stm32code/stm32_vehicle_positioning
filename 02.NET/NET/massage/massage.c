#include "massage.h"
// �����豸
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "git.h"

// ��ȡȫ�ֱ���
extern Data_TypeDef Data_init;                 // �豸����
extern Device_Satte_Typedef device_state_init; // �豸״̬

// ����json����
mySta massage_parse_json(char *message)
{

    cJSON *cjson_test = NULL; // ���json��ʽ
    cJSON *cjson_data = NULL; // ����

    // ������������
    char *cjson_device_id;
    u8 cjson_cmd, cjson_direction, massgae; // ָ��,����

    /* ��������JSO���� */
    cjson_test = cJSON_Parse(message);
    if (cjson_test == NULL)
    {
        // ����ʧ�� printf("parse fail.\n");
        return MY_FAIL;
    }

    /* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
    cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
    cjson_direction = cJSON_GetObjectItem(cjson_test, "direction")->valueint;

    /* ����Ƕ��json���� */
    cjson_data = cJSON_GetObjectItem(cjson_test, "data");
    cjson_device_id = cJSON_GetObjectItem(cjson_data, "device_id")->valuestring;

    if (cjson_direction == PLATFORM_SEND && cjson_device_id == Device_ID)
    {
        switch (cjson_cmd)
        {
        case 0x01: //
            // LED = ~LED;
            massgae = cJSON_GetObjectItem(cjson_data, "online")->valueint;
            break;
        case 0x02: // ������
            massgae = cJSON_GetObjectItem(cjson_data, "heart")->valueint;
            break;
        case 0x03: // ���ݰ�
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        case 0x04: // �����ϱ�Ӧ��
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        case 0x05: // ƽ̨�·�Ӧ��
            massgae = cJSON_GetObjectItem(cjson_data, "mag")->valueint;
            if (massgae)
            {
                /* code */
            }
            break;
        default:
            break;
        }
    }
    /* ���JSON����(��������)���������� */
    cJSON_Delete(cjson_test);
    cJSON_Delete(cjson_data);

    return MY_SUCCESSFUL;
}
// �豸������
mySta heart_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // ����
    char *data;

    usr = cJSON_CreateObject(); // ���������ݶ���
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x02));               // ���ڵ����������
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // ���ڵ�������ַ�

    /* ���һ��Ƕ�׵�JSON���ݣ����һ������ڵ㣩 */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString(Device_ID));
    cJSON_AddItemToObject(cjson_data, "heart", cJSON_CreateNumber(0x01));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // ��json��ʽ��ӡ�������ַ�����ʽ(����\r\n)
    // printf("%s", data);                 // ͨ�����ڴ�ӡ����
    ESP8266_SendData((unsigned char *)data, strlen(data)); // �������ݽڵ�

    /*********************/
    /*		�ͷ��ڴ�	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
// �豸���ݰ�
mySta data_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // ����
    char *data;

    usr = cJSON_CreateObject(); // ���������ݶ���
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x03));               // ���ڵ����������
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // ���ڵ�������ַ�

    /* ���һ��Ƕ�׵�JSON���ݣ����һ������ڵ㣩 */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString("12345678"));
    cJSON_AddItemToObject(cjson_data, "temp", cJSON_CreateNumber(Data_init.temperatuer));
    cJSON_AddItemToObject(cjson_data, "light", cJSON_CreateNumber(Data_init.light));
    cJSON_AddItemToObject(cjson_data, "led_state", cJSON_CreateNumber(device_state_init.led_state));
    cJSON_AddItemToObject(cjson_data, "hot_led_state", cJSON_CreateNumber(device_state_init.hot_led_state));
    cJSON_AddItemToObject(cjson_data, "fan_state", cJSON_CreateNumber(device_state_init.fan_state));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // ��json��ʽ��ӡ�������ַ�����ʽ(����\r\n)
    // printf("%s", data);                 // ͨ�����ڴ�ӡ����
    ESP8266_SendData((unsigned char *)data, strlen(data)); // �������ݽڵ�

    /*********************/
    /*		�ͷ��ڴ�	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
// ע���
mySta login_json_init()
{
    cJSON *usr = NULL;
    cJSON *cjson_data = NULL; // ����
    char *data;

    usr = cJSON_CreateObject(); // ���������ݶ���
    cjson_data = cJSON_CreateObject();

    cJSON_AddItemToObject(usr, "cmd", cJSON_CreateNumber(0x01));               // ���ڵ����������
    cJSON_AddItemToObject(usr, "direction", cJSON_CreateNumber(DERVICE_SEND)); // ���ڵ�������ַ�

    /* ���һ��Ƕ�׵�JSON���ݣ����һ������ڵ㣩 */
    cJSON_AddItemToObject(cjson_data, "device_id", cJSON_CreateString("12345678"));
    cJSON_AddItemToObject(cjson_data, "online", cJSON_CreateNumber(0x00));
    cJSON_AddItemToObject(usr, "data", cjson_data);

    data = cJSON_PrintUnformatted(usr); // ��json��ʽ��ӡ�������ַ�����ʽ(����\r\n)
    // printf("%s", data);                 // ͨ�����ڴ�ӡ����
    ESP8266_SendData((unsigned char *)data, strlen(data)); // �������ݽڵ�

    /*********************/
    /*		�ͷ��ڴ�	  */
    /*********************/
    // cJSON_Delete(cjson_data);
    cJSON_Delete(usr);

    // delay_ms(50);
    cJSON_free(data);

    return MY_SUCCESSFUL;
}
