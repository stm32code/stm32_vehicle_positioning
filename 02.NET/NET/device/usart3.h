#ifndef _ESP8266_H_
#define _ESP8266_H_
/*-------------------------------------------------*/
/*                                                 */
/*              ����Wifi���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/
#include "git.h"
// ��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

#define REV_OK 0   // ������ɱ�־
#define REV_WAIT 1 // ����δ��ɱ�־

void ESP8266_Init(void);
void Usart3_Init(unsigned int baud);
void ESP8266_Clear(void);

void ESP8266_SendData(unsigned char *data, unsigned short len);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);
void Usart3_SendString(unsigned char *str, unsigned short len);

_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time);
_Bool ESP8266_WaitRecive(void);

#endif
