#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "stm32f10x_flash.h"
#include "git.h"

extern U32 addrShort1; // 1
extern U32 addrShort2; // 2


// �������������壬���ڴ�ȡ�ڲ�FLASH���ݣ�F103ϵ�ж������޸�
#ifdef STM32F10X_MD
#define STM32_FLASH_SECTOR_SIZE 2048 // �ڲ�FLASHҳ��С, ��λ��bytes ��(ע��STM32F10xxϵ���£�С�������洢������Ϊ1K, �������洢������Ϊ2K��
#else
#define STM32_FLASH_SECTOR_SIZE 1024             ������ // �ڲ�FLASHҳ��С, ��λ��bytes ��(ע��STM32F10xxϵ���£�С�������洢������Ϊ1K, �������洢������Ϊ2K��
#endif

#define STM32_FLASH_ADDR_BASE 0x08000000                  // оƬ�ڲ�FLASH��ַ(������������޸ģ�
static uint8_t sectorbufferTemp[STM32_FLASH_SECTOR_SIZE]; // ����һ���ڴ�ռ䣬���ã����ڲ�FLASHд��ʱ�����ݻ���

/*********************************************************************************
 * @Function	:  ��оƬ���ڲ�FLASH�д��ָ����������
 * @Input		:  writeAddr,д�����ݵĵ�ַ
 *							 *writeToBuffer,д������
 *							 numToWrite,д�����ݳ���
 * @Output		:  None
 * @Return		:  0_�ɹ���
 *         		 1_ʧ�ܣ���ַ��Χ����ȷ
 *         		 2_ʧ�ܣ�FLASH->SR:BSYæ��ʱ
 *         		 3_ʧ�ܣ�������ʱ
 * @Others		:  None
 * @Date			:  2022-08-23
 **********************************************************************************/
uint8_t System_WriteInteriorFlash(uint32_t writeAddr, uint8_t *writeToBuffer, uint16_t numToWrite);
/*********************************************************************************
 * @Function	:  ��оƬ���ڲ�FLASH���ȡָ����������
 * @Input		:  writeAddr,д�����ݵĵ�ַ
 *							 *writeToBuffer,д������
 *							 numToWrite,д�����ݳ���
 * @Output		:  None
 * @Return		:  0_�ɹ�
 *         		 1_ʧ�ܣ���ַС��FLASH��ַ
 *         		 2_ʧ�ܣ���ַ����FLASH���ֵ
 * @Others		:  None
 * @Date			:  2022-08-23
 **********************************************************************************/
uint8_t System_ReadInteriorFlash(uint32_t readAddr, uint8_t *pBuffer, uint16_t numToRead);
/*********************************************************************************
 * @Function	:  ����FALSH��д����
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  ��ʾ����ļ������ݶ�д����
 * @Date			:  2022-08-23
 **********************************************************************************/
void W_R_Test(void);
void W_Test(void);
// ��ȡ����
void R_Test(void);
#endif
