#ifndef _MASSAGE__H_
#define _MASSAGE__H_

#include "stm32f10x_it.h"
#include "cjson.h"
#include "string.h"
#include <stdio.h>
#include "git.h"
// �����豸���
#define Device_ID "12345678"

// ����json����
mySta massage_parse_json(char *message);
// �豸������
mySta heart_json_init(void);
// ע���
mySta login_json_init(void);
// �豸���ݰ�
mySta data_json_init(void);
// ����OLED��ʾ��������
mySta Update_oled_massage(void);
// �����豸״̬
mySta Update_device_massage(Device_Satte_Typedef *device_state);

#endif
