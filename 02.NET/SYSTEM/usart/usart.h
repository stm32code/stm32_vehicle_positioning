#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#define USART_REC_LEN 200 // �����������ֽ��� 200
#define EN_USART1_RX 1    // ʹ�ܣ�1��/��ֹ��0������1����

extern u8 USART_RX_BUF[USART_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART_RX_STA;               // ����״̬���
// ����봮���жϽ��գ��벻Ҫע�����º궨��
void Usart1_Init(u32 bound);
/*********************************************************************************
* @Function	:  ����һ���ֽ�
* @Input		:  pUSARTx ������
                             ch ����
* @Output		:
* @Return		:  None
* @Others		:  None
* @Date			:  2022-07-30
**********************************************************************************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
/*********************************************************************************
* @Function	:  �����ַ���
* @Input		:  pUSARTx ������
                             str �ַ���
* @Output		:  None
* @Return		:  None
* @Others		:  None
* @Date			:  2022-07-30
**********************************************************************************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
/*********************************************************************************
* @Function	:  ����һ��16λ��
* @Input		:  pUSARTx ������
                             ch 16λ��
* @Output		:  None
* @Return		:  None
* @Others		:  None
* @Date			:  2022-07-30
**********************************************************************************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch);
/*********************************************************************************
* @Function	:  ����8λ������
* @Input		:  pUSARTx ������
                             array ����
                             num ���鳤��
* @Output		:  None
* @Return		:  None
* @Others		:  None
* @Date			:  2022-07-30
**********************************************************************************/
void Usart_SendArray(USART_TypeDef *pUSARTx, char *array, uint16_t num);

#endif
