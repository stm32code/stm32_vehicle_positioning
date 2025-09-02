#include "flash.h"
#include "usart.h"
extern Device_Satte_Typedef device_state_init; // �豸״̬
/*********************************************************************************
 * @Function	:  �ڲ�FLASHд��ʱ���ȴ�����
 * @Input		:  timeOut,�ȴ�BSY��־����
 * @Output		:  None
 * @Return		:  0����
 *         		 1��æ
 * @Others		:  None
 * @Date			:  2022-08-23
 **********************************************************************************/
// ���ã��ڲ�FLASHд��ʱ���ȴ����У�BSYλ��־:1æ
static uint8_t waitForFlashBSY(uint32_t timeOut)
{
    while ((FLASH->SR & 0x01) && (timeOut-- != 0x00))
        ; // �ȴ�BSY��־����
    if (timeOut == 0)
        return 1; // ʧ�ܣ�����1, �ȴ���ʱ����

    return 0; // ���������أ�
}
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
uint8_t System_WriteInteriorFlash(uint32_t writeAddr, uint8_t *writeToBuffer, uint16_t numToWrite)
{
    uint16_t flashSize = *(uint16_t *)(0x1FFFF7E0); // ��ȡоƬFLASH��С;���Ĵ���ֵΪоƬ����ǰд���FLASH��С��ֻ������λ��KByte

    uint32_t addrOff = writeAddr - STM32_FLASH_ADDR_BASE; // ȥ��0x08000000���ʵ��ƫ�Ƶ�ַ
    uint32_t secPos = addrOff / STM32_FLASH_SECTOR_SIZE;
    ;                                                      // ������ַ,����ʼ��ַ�ڵڼ�������
    uint16_t secOff = addrOff % STM32_FLASH_SECTOR_SIZE;   // ��ʼ��ʼƫ���ֽ���: �����������ĵڼ��ֽڴ��
    uint16_t secRemain = STM32_FLASH_SECTOR_SIZE - secOff; // ��������Ҫд����ֽ��� ,�����жϹ�����������µ�����

    // �жϵ�ַ��Ч��
    if (writeAddr < STM32_FLASH_ADDR_BASE)
        return 1; // ������ĵ�ַ��С��FLASH����С��ַ�����˳�������1_��ַʧ��
    if (writeAddr > (STM32_FLASH_ADDR_BASE + (flashSize * 1024)))
        return 1; // ������ĵ�ַ������FLASH������ַ�����˳�, ����1_��ַʧ��

    // 0_����FLASH
    FLASH->KEYR = ((uint32_t)0x45670123);
    FLASH->KEYR = ((uint32_t)0xCDEF89AB);

    if (numToWrite <= secRemain)
        secRemain = numToWrite;
    while (1)
    {
        // 1_��ȡ��ǰҳ������
        if (waitForFlashBSY(0x00888888))
            return 2;                                                                                                                  // ʧ�ܣ�����:2, ʧ��ԭ��FLASH->SR:BSYæ��ʱ
        System_ReadInteriorFlash(secPos * STM32_FLASH_SECTOR_SIZE + STM32_FLASH_ADDR_BASE, sectorbufferTemp, STM32_FLASH_SECTOR_SIZE); // ��ȡ�������ݵ�����

        // 2_����ָ��ҳ(����)
        if (waitForFlashBSY(0x00888888))
            return 2;                                                         // ʧ�ܣ�����:2, ʧ��ԭ��FLASH->SR:BSYæ��ʱ
        FLASH->CR |= 1 << 1;                                                  // PER:ѡ��ҳ������λ2MERΪȫ����
        FLASH->AR = STM32_FLASH_ADDR_BASE + secPos * STM32_FLASH_SECTOR_SIZE; // ��дҪ������ҳ��ַ
        FLASH->CR |= 0x40;                                                    // STRT:д1ʱ����һ�β���������
        if (waitForFlashBSY(0x00888888))
            return 2;                        // ʧ�ܣ�����:��, ʧ��ԭ�򣺲�����ʱ
        FLASH->CR &= ((uint32_t)0x00001FFD); // �ر�ҳ��������

        for (uint16_t i = 0; i < secRemain; i++) // ԭʼ����д�뻺��
            sectorbufferTemp[secOff + i] = writeToBuffer[i];

        for (uint16_t i = 0; i < STM32_FLASH_SECTOR_SIZE / 2; i++)
        { // ��������д��оƬFLASH
            if (waitForFlashBSY(0x00888888))
                return 2;                                                                                                                                   // ʧ�ܣ�����:2, ʧ��ԭ��FLASH->SR:BSYæ��ʱ
            FLASH->CR |= 0x01 << 0;                                                                                                                         // PG: ���
            *(uint16_t *)(STM32_FLASH_ADDR_BASE + secPos * STM32_FLASH_SECTOR_SIZE + i * 2) = (sectorbufferTemp[i * 2 + 1] << 8) | sectorbufferTemp[i * 2]; // ��������д���豸

            if (waitForFlashBSY(0x00888888))
                return 2;                        // ʧ�ܣ�����:2, ʧ��ԭ��FLASH->SR:BSYæ��ʱ
            FLASH->CR &= ((uint32_t)0x00001FFE); // �رձ��
        }

        if (secRemain == numToWrite)
        {
            break; // ��ȫ��д��
        }
        else
        {                                                                                                // δд��
            writeToBuffer += secRemain;                                                                  // ԭʼ����ָ��ƫ��
            secPos++;                                                                                    // ������
            secOff = 0;                                                                                  // ��ƫ��λ,������������ʼ��ַ
            numToWrite -= secRemain;                                                                     // ʣ��δд�ֽ���
            secRemain = (numToWrite > STM32_FLASH_SECTOR_SIZE) ? (STM32_FLASH_SECTOR_SIZE) : numToWrite; // ����������д���ֽ���
        }
    }
    FLASH->CR |= 1 << 7; // LOCK:��������

    return 0;
}
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
uint8_t System_ReadInteriorFlash(uint32_t readAddr, uint8_t *pBuffer, uint16_t numToRead)
{
    // ��ȡоƬFLASH��С
    uint16_t flashSize = *(uint16_t *)(0x1FFFF7E0); // ��ȡоƬFLASH��С;���Ĵ���ֵΪоƬ����ǰд���FLASH��С��ֻ����λ��KByte

    // �жϵ�ַ��Ч��
    if (readAddr < STM32_FLASH_ADDR_BASE)
        return 1; // ������ĵ�ַ��С��FLASH����С��ַ�����˳�
    if (readAddr > (STM32_FLASH_ADDR_BASE + (flashSize * 1024)))
        return 2; // ������ĵ�ַ������FLASH������ַ�����˳�

    // ��ʼ����
    while (numToRead--)
    {
        *pBuffer = *(__IO uint8_t *)readAddr;
        pBuffer++;  // ָ�����һ�����ݳ���
        readAddr++; // ƫ��һ������λ
    }

    return 0; // �ɹ�������0;
}
// д������
U32 addrShort1 = 0x0800C000 + 10 * 16; // 1
U32 addrShort2 = 0x0800C000 + 11 * 16; // 2
U32 addrShort3 = 0x0800C000 + 12 * 16; // 3
U32 addrShort4 = 0x0800C000 + 13 * 16; // 4
U32 addrShort5 = 0x0800C000 + 14 * 16; // 5
U32 addrShort6 = 0x0800C000 + 15 * 16; // 6
void W_Test(void)
{

    System_WriteInteriorFlash(addrShort1, (uint8_t *)&device_state_init.Drug1, sizeof(device_state_init.Drug1)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort1, (uint8_t *)&device_state_init.Drug1, sizeof(device_state_init.Drug1));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("1��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort1, device_state_init.Drug1, device_state_init.Drug1);

    System_WriteInteriorFlash(addrShort2, (uint8_t *)&device_state_init.Drug2, sizeof(device_state_init.Drug2)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort2, (uint8_t *)&device_state_init.Drug2, sizeof(device_state_init.Drug2));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("2��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort3, device_state_init.Drug2, device_state_init.Drug2);

    System_WriteInteriorFlash(addrShort3, (uint8_t *)&device_state_init.Drug3, sizeof(device_state_init.Drug3)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort3, (uint8_t *)&device_state_init.Drug3, sizeof(device_state_init.Drug3));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("3��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort2, device_state_init.Drug3, device_state_init.Drug3);

    System_WriteInteriorFlash(addrShort4, (uint8_t *)&device_state_init.Drug4, sizeof(device_state_init.Drug4)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort4, (uint8_t *)&device_state_init.Drug4, sizeof(device_state_init.Drug4));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("4��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort4, device_state_init.Drug4, device_state_init.Drug4);

    System_WriteInteriorFlash(addrShort5, (uint8_t *)&device_state_init.Drug5, sizeof(device_state_init.Drug5)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort5, (uint8_t *)&device_state_init.Drug5, sizeof(device_state_init.Drug5));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("5��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort5, device_state_init.Drug5, device_state_init.Drug5);

    System_WriteInteriorFlash(addrShort6, (uint8_t *)&device_state_init.Drug6, sizeof(device_state_init.Drug6)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort6, (uint8_t *)&device_state_init.Drug6, sizeof(device_state_init.Drug6));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("6��ַ:0x%X д��:%d  ��ȡ:%d\r\n", addrShort6, device_state_init.Drug6, device_state_init.Drug6);
}
// ��ȡ����
void R_Test(void)
{
    // System_WriteInteriorFlash(addrShort1, (uint8_t *)&device_state_init.Drug1, sizeof(device_state_init.Drug1)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort1, (uint8_t *)&device_state_init.Drug1, sizeof(device_state_init.Drug1)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("1��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort1, device_state_init.Drug1, device_state_init.Drug1);

    //  System_WriteInteriorFlash(addrShort2, (uint8_t *)&device_state_init.Drug2, sizeof(device_state_init.Drug2)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort2, (uint8_t *)&device_state_init.Drug2, sizeof(device_state_init.Drug2)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("2��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort3, device_state_init.Drug2, device_state_init.Drug2);

    System_WriteInteriorFlash(addrShort3, (uint8_t *)&device_state_init.Drug3, sizeof(device_state_init.Drug3)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort3, (uint8_t *)&device_state_init.Drug3, sizeof(device_state_init.Drug3));  // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("3��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort2, device_state_init.Drug3, device_state_init.Drug3);

    // System_WriteInteriorFlash(addrShort4, (uint8_t *)&device_state_init.Drug4, sizeof(device_state_init.Drug4)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort4, (uint8_t *)&device_state_init.Drug4, sizeof(device_state_init.Drug4)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("4��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort4, device_state_init.Drug4, device_state_init.Drug4);

    // System_WriteInteriorFlash(addrShort5, (uint8_t *)&device_state_init.Drug5, sizeof(device_state_init.Drug5)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort5, (uint8_t *)&device_state_init.Drug5, sizeof(device_state_init.Drug5)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("5��ַ:0x%X д��:%d  ��ȡ:%d \r\n", addrShort5, device_state_init.Drug5, device_state_init.Drug5);

    // System_WriteInteriorFlash(addrShort6, (uint8_t *)&device_state_init.Drug6, sizeof(device_state_init.Drug6)); // �������ݣ��������Զ�������������д��
    System_ReadInteriorFlash(addrShort6, (uint8_t *)&device_state_init.Drug6, sizeof(device_state_init.Drug6)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("6��ַ:0x%X д��:%d  ��ȡ:%d\r\n", addrShort6, device_state_init.Drug6, device_state_init.Drug6);
}
/*********************************************************************************
 * @Function	:  ����FALSH��д����
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  ��ʾ����ļ������ݶ�д����
 * @Date			:  2022-08-23
 **********************************************************************************/
void W_R_Test(void)
{

    // // ����1���ַ�
    // uint32_t addrChar = 0x08000000 + 38 * 1024; // ע���ַ������ż�����ҵ�ַ�ξ���������FLASHβ�������⸲���˴��������
    // char writeChar = 'F';
    // System_WriteInteriorFlash(addrChar, (uint8_t *)&writeChar, 1); // �������ݣ��������Զ�������������д��
    // char readChar = 0;
    // System_ReadInteriorFlash(addrChar, (uint8_t *)&readChar, 1); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    // printf("����char     ��ַ:0x%X    д��:%c       ��ȡ:%c \r\n", addrChar, writeChar, readChar);
    printf("\r\n>>>>��ʼ�����ڲ�FLASH��д��������ת����\r\n");
    // ����2�������ŵ�16λ
    uint32_t addrShort = 0x08000000 + 2 * 1024; // ע���ַ������ż�����ҵ�ַ�ξ���������FLASHβ�������⸲���˴��������
    short writeShort = -888;
    System_WriteInteriorFlash(addrShort, (uint8_t *)&writeShort, sizeof(writeShort)); // �������ݣ��������Զ�������������д��
    short readShort = 0;
    System_ReadInteriorFlash(addrShort, (uint8_t *)&readShort, sizeof(writeShort)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    printf("����short    ��ַ:0x%X    д��:%d    ��ȡ:%d \r\n", addrShort, writeShort, readShort);

    // // ����3���޷��ŵ�32λ
    // uint32_t addrUint = 0x08000000 + 254 * 1024; // ע���ַ������ż�����ҵ�ַ�ξ���������FLASHβ�������⸲���˴��������
    // uint32_t writeUint = 0x12345678;
    // System_WriteInteriorFlash(addrUint, (uint8_t *)&writeUint, 4); // �������ݣ��������Զ�������������д��
    // uint32_t readUint = 0;
    // System_ReadInteriorFlash(addrUint, (uint8_t *)&readUint, 4); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    // printf("����uint     ��ַ:0x%X    д��:0x%X     ��ȡ:0x%X \r\n", addrUint, writeUint, readUint);

    // // ����4���ַ���
    // uint32_t addrString = 0x08000000 + 166 * 1024 - 6; // ע���ַ������ż�����ҵ�ַ�ξ���������FLASHβ�������⸲���˴��������
    // char writeString[100] = "��߹! ȥ����?";
    // System_WriteInteriorFlash(addrString, (uint8_t *)writeString, sizeof(writeString)); // �������ݣ��������Զ�������������д��
    // char readString[100];
    // System_ReadInteriorFlash(addrString, (uint8_t *)readString, sizeof(readString)); // ��ȡ���ݣ��������Զ������ֶ�ȡ
    // printf("����string   ��ַ:0x%X��  д��:%s  ��ȡ:%s \r\n", addrString, writeString, readString);
}
