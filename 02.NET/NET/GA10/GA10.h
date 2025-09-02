#ifndef __GA6_H_
#define __GA6_H_

// ��Ƭ��ͷ�ļ�
#include "stm32f10x.h"
#include "git.h"
#include "cjson.h"
#include "string.h"
#include <stdio.h>
// ��ʼ��
void GA10_Init(void);
// ����Ϣ
void GA10_SendData(unsigned char *data, unsigned short len);
// ������
_Bool GA10_SendCmd(char *cmd, char *res, u16 time);
// ����json����
void massage_ga10_json(char *message);
// ������Ϣ
void Mqtt_Pub(u8 cmd);

#endif


