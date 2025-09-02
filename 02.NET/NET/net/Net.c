// �����豸
#include "usart3.h"

// Э���ļ�
#include "Net.h"
#include "mqttkit.h"
#include "delay.h"

// Ӳ������
#include "usart.h"
#include "led.h"

//  C��
#include <string.h>
#include <stdio.h>

extern unsigned char esp8266_buf[256];

U8 Connect_Net;
extern int32_t n_sp02;		 // SPO2 value
extern int8_t ch_spo2_valid; // indicator to show if the SP02 calculation is valid
extern int32_t n_heart_rate; // heart rate value
extern int8_t ch_hr_valid;	 // indicator to show if the heart rate calculation is valid
//==========================================================
//	�������ƣ�	DevLink
//==========================================================
_Bool DevLink(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	unsigned char *dataPtr;

	_Bool status = 1;

	printf("PROID: %s	AUIF: %s	DEVID:%s\r\n", PROID, AUTH_INFO, DEVID);

	if (MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // �ϴ�ƽ̨

		dataPtr = ESP8266_GetIPD(250); // �ȴ�ƽ̨��Ӧ
		if (dataPtr != NULL)
		{
			if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch (MQTT_UnPacketConnectAck(dataPtr))
				{
				case 0:
					printf("Tips:	���ӳɹ�\r\n");
					status = 0;
					Data_init.Error_Time = 0;
					break;

				case 1:
					printf("WARN:	����ʧ�ܣ�Э�����\r\n");
					break;
				case 2:
					printf("WARN:	����ʧ�ܣ��Ƿ���clientid\r\n");
					break;
				case 3:
					printf("WARN:	����ʧ�ܣ�������ʧ��\r\n");
					break;
				case 4:
					printf("WARN:	����ʧ�ܣ��û������������\r\n");
					break;
				case 5:
					printf("WARN:	����ʧ�ܣ��Ƿ�����(����token�Ƿ�)\r\n");
					break;

				default:
					printf("ERR:	����ʧ�ܣ�δ֪����\r\n");
					break;
				}
			}
		}
		Data_init.Error_Time++;
		MQTT_DeleteBuffer(&mqttPacket); // ɾ��
	}
	else
	{
		Data_init.Error_Time++;
		printf("WARN:	MQTT_PacketConnect Failed\r\n");
	}
	if (Data_init.Error_Time > 2)
	{
		Sys_Restart(); // ��λ
	}
	return status;
}
unsigned char OneNet_FillBuf(char *buf)
{
	char text[256];
#if NET_SERVE == 0
	memset(text, 0, sizeof(text));
	strcpy(buf, ",;");
	memset(text, 0, sizeof(text));
	sprintf(text, "time,%d;", 25); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
//	memset(text, 0, sizeof(text));
//	sprintf(text, "humi,%.2f;", 55.5); // Temp����������һ�����ƣ�temper���¶�ֵ
//	strcat(buf, text);
#elif NET_SERVE == 1
	memset(text, 0, sizeof(text));
	strcpy(buf, "{");
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"method\":\"thing.service.property.set\",");
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"id\":\"1\",");
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"params\":{");
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "\"flage\":%d,", Data_init.Device_State);
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"hanger\":%d,", device_state_init.check_device);
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"light\":%d,", Data_init.light);
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"rain\":%d", Data_init.rain);
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

#endif
	// printf("%s\n", buf);
	return strlen(buf);
}

unsigned char MQTT_FillBuf(char *buf)
{
	char text[256];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");
	//��Ч����

	memset(text, 0, sizeof(text));
	sprintf(text, "\"lon\":\"%.6f\",",104.072747); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"lat\":\"%.6f\",", 30.578994); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "\"led\":\"%d\"",device_state_init.Key_State); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);
	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Pwd_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"time\":\"%d\"", 1); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Flage_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	sprintf(text, "\"check_device\":\"%d\"", device_state_init.check_device); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
unsigned char Mqtt_Fan_Info(char *buf)
{
	char text[128];
	memset(text, 0, sizeof(text));

	strcpy(buf, "{");

	memset(text, 0, sizeof(text));
	// sprintf(text, "\"Fan\":\"%d\"", device_state_init.fan_state); // Temp����������һ�����ƣ�temper���¶�ֵ
	strcat(buf, text);

	memset(text, 0, sizeof(text));
	sprintf(text, "}");
	strcat(buf, text);

	return strlen(buf);
}
//==========================================================
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//==========================================================
void SendData(void)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	char buf[256];

	short body_len = 0, i = 0;
	memset(buf, 0, sizeof(buf));
	// ���
	body_len = OneNet_FillBuf(buf); // ��ȡ��ǰ��Ҫ���͵����������ܳ���

	if (body_len)
	{

#if NET_SERVE == 0
		// ���
		if (MQTT_PacketSaveData(DEVID, body_len, NULL, 5, &mqttPacket) == 0) // ���
		{
			for (; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];

			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // �ϴ����ݵ�ƽ̨

			MQTT_DeleteBuffer(&mqttPacket); // ɾ��
		}
		else
			printf("WARN:	EDP_NewBuffer Failed\r\n");
#elif NET_SERVE == 1
		// ���
		if (MQTT_PacketPublish(MQTT_PUBLISH_ID, P_TOPIC_CMD, buf, body_len, MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0)
		{
			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
			MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
		}
		else
			printf("WARN\r\n");
#endif
	}
}
//==========================================================

//	�������ܣ�	�ϴ����ݵ�ƽ̨

//==========================================================
void SendMqtt(U8 Cmd)
{
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���
	char buf[254];
	short body_len = 0;
	memset(buf, 0, sizeof(buf));
	switch (Cmd)
	{
	case 1:
		body_len = MQTT_FillBuf(buf); // ������
		break;
	case 2:
		body_len = Mqtt_Pwd_Info(buf); // ������
		break;
	case 3:
		body_len = Mqtt_Flage_Info(buf); // ������
		break;
	case 4:
		body_len = Mqtt_Flage_Info(buf); // ������
		break;
	default:
		break;
	}

	if (body_len)
	{
		// ���
		if (MQTT_PacketPublish(MQTT_PUBLISH_ID, P_TOPIC_NAME, buf, body_len, MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0)
		{
			ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
			MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
		}
		else
			printf("WARN\r\n");
	}
}
//==========================================================

//	�������ܣ�	ƽ̨�������ݼ��

//==========================================================
void RevPro(unsigned char *cmd)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	char *req_payload = NULL;
	char *cmdid_topic = NULL;

	unsigned short topic_len = 0;
	unsigned short req_len = 0;

	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;

	short result = 0;

	//---------------------------------------------����һ����ȡ������������---------------------------------------------
	type = MQTT_UnPacketRecv(cmd);
	switch (type)
	{
		//---------------------------------------------����������ú�������-------------------------------------------------
	case MQTT_PKT_CMD: // �����·�

		result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len); // ���topic����Ϣ��
		if (result == 0)
		{
			printf("cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);

			if (MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0) // ����ظ����
			{
				printf("Tips:	Send CmdResp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
			}
		}

		break;

	case MQTT_PKT_PUBLISH: // ���յ�Publish��Ϣ

		result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
		if (result == 0)
		{
			printf("topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				   cmdid_topic, topic_len, req_payload, req_len);

			switch (qos)
			{
			case 1: // �յ�publish��qosΪ1���豸��Ҫ�ظ�Ack

				if (MQTT_PacketPublishAck(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishAck\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
					MQTT_DeleteBuffer(&mqttPacket);
				}

				break;

			case 2: // �յ�publish��qosΪ2���豸�Ȼظ�Rec
					// ƽ̨�ظ�Rel���豸�ٻظ�Comp
				if (MQTT_PacketPublishRec(pkt_id, &mqttPacket) == 0)
				{
					printf("Tips:	Send PublishRec\r\n");
					ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
					MQTT_DeleteBuffer(&mqttPacket);
				}
				break;

			default:
				break;
			}
		}

		break;

	case MQTT_PKT_PUBACK: // ����Publish��Ϣ��ƽ̨�ظ���Ack

		if (MQTT_UnPacketPublishAck(cmd) == 0)
			printf("Tips:	MQTT Publish Send OK\r\n");
		Connect_Net = 60; // �����ɹ�
		break;

	case MQTT_PKT_PUBREC: // ����Publish��Ϣ��ƽ̨�ظ���Rec���豸��ظ�Rel��Ϣ

		if (MQTT_UnPacketPublishRec(cmd) == 0)
		{
			printf("Tips:	Rev PublishRec\r\n");
			if (MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishRel\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBREL: // �յ�Publish��Ϣ���豸�ظ�Rec��ƽ̨�ظ���Rel���豸���ٻظ�Comp

		if (MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
		{
			printf("Tips:	Rev PublishRel\r\n");
			if (MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
			{
				printf("Tips:	Send PublishComp\r\n");
				ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
				MQTT_DeleteBuffer(&mqttPacket);
			}
		}

		break;

	case MQTT_PKT_PUBCOMP: // ����Publish��Ϣ��ƽ̨����Rec���豸�ظ�Rel��ƽ̨�ٷ��ص�Comp

		if (MQTT_UnPacketPublishComp(cmd) == 0)
		{
			printf("Tips:	Rev PublishComp\r\n");
		}

		break;

	case MQTT_PKT_SUBACK: // ����Subscribe��Ϣ��Ack

		if (MQTT_UnPacketSubscribe(cmd) == 0)
			printf("Tips:	MQTT Subscribe OK\r\n");
		else
			printf("Tips:	MQTT Subscribe Err\r\n");

		break;

	case MQTT_PKT_UNSUBACK: // ����UnSubscribe��Ϣ��Ack

		if (MQTT_UnPacketUnSubscribe(cmd) == 0)
			printf("Tips:	MQTT UnSubscribe OK\r\n");
		else
			printf("Tips:	MQTT UnSubscribe Err\r\n");

		break;

	default:
		result = -1;
		break;
	}
	//---------------------------------------------�������������---------------------------------------------

	ESP8266_Clear(); // ��ջ���

	if (result == -1)
		return;
	// ����json
	if (req_payload != NULL)
	{
		massage_parse_json((char *)req_payload);
	}
	//---------------------------------------------�����ģ��ͷ��ڴ�---------------------------------------------
	if (type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
}

//==========================================================
//	�������ܣ�	�����ж�
//==========================================================
void Link_OneNet(u8 Link)
{
	if (!Link)
	{
		ESP8266_Init();			 // ��ʼ��ESP8266
		while (DevLink()) // ����OneNET
			delay_ms(500);
	}
}
//==========================================================
//	�������ƣ�	Subscribe
//
//	�������ܣ�	����
//
//	��ڲ�����	topics�����ĵ�topic
//				topic_cnt��topic����
//
//	���ز�����	SEND_TYPE_OK-�ɹ�	SEND_TYPE_SUBSCRIBE-��Ҫ�ط�
//
//	˵����
//==========================================================
_Bool Subscribe(const char *topics[], unsigned char topic_cnt)
{

	unsigned char i = 0;
	unsigned char *dataPtr;
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	for (; i < topic_cnt; i++)
		printf("Subscribe Topic: %s\r\n", topics[i]);

	//---------------------------------------------����һ�����---------------------------------------------
	if (MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL2, topics, topic_cnt, &mqttPacket) == 0)
	{
		//---------------------------------------------���������������-----------------------------------------
		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
		dataPtr = ESP8266_GetIPD(500);						 // �ȴ�ƽ̨��Ӧ
		if (MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_SUBACK)
		{
			//---------------------------------------------��������ɾ��---------------------------------------------
			printf("Tips:	MQTT Subscribe OK\r\n");
			MQTT_DeleteBuffer(&mqttPacket); // ɾ��
			Data_init.Error_Time = 0;
			return 0;
		}
		else
		{
			//---------------------------------------------��������ɾ��---------------------------------------------
			printf("Tips:	MQTT Subscribe Err\r\n");
			MQTT_DeleteBuffer(&mqttPacket); // ɾ��
			if (++Data_init.Error_Time > 2)
			{
				Sys_Restart(); // ��λ
			}
			return 1;
		}
	}

	return 1;
}

//==========================================================

//	�������ܣ�	������Ϣ

//==========================================================
_Bool Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0}; // Э���

	// printf( "Publish Topic: %s, Msg: %s\r\n", topic, msg);

	if (MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) != 1)
	{

		ESP8266_SendData(mqttPacket._data, mqttPacket._len); // ��ƽ̨���Ͷ�������
		MQTT_DeleteBuffer(&mqttPacket);						 // ɾ��
	}
	return 0;
}
