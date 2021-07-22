#ifndef W25QXX_H
#define W25QXX_H

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "spi.h"

// W25Q128����˵����
// ��С������λΪһ��Sector(4KB)
// 16��SectorΪһ��Block(64KB)
// W25Q128����Ϊ128Mb = 16MB������256��Block��4096��Sector

// W25QXXϵ��оƬ�ͺ�
#define W25Q80 0XEF13
#define W25Q16 0XEF14
#define W25Q32 0XEF15
#define W25Q64 0XEF16
#define W25Q128 0XEF17

#define W25QXX_CS PBout(12) // W25QXX��Ƭѡ�ź�

// W25QXXָ���
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactDeviceID 0x90
#define W25X_JedecDeviceID 0x9F

void W25QXX_Init(void);                           // ��ʼ��
u16 W25QXX_ReadType(void);                        // ��ȡW25QXXоƬ�ͺ�
u8 W25QXX_ReadSR(void);                           // ��ȡ״̬�Ĵ���
void W25QXX_WriteSR(u8 sr);                       // д״̬�Ĵ���
void W25QXX_WriteEnable(void);                    // дʹ��
void W25QXX_WriteDisable(void);                   // д��ֹ
void W25QXX_Read(u8 *buffer, u32 addr, u16 len);  // ��ȡ����
void W25QXX_Write(u8 *buffer, u32 addr, u16 len); // д������
void W25QXX_EraseChip(void);                      // ��Ƭ����
void W25QXX_EraseSector(u32 addr);                // ��������
void W25QXX_WaitBusy(void);                       // �ȴ�����
void W25QXX_PowerDown(void);                      // �������ģʽ
void W25QXX_WakeUp(void);                         // ����

#endif
