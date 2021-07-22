#include "w25qxx.h"

u16 W25QXX_TYPE = W25Q128; // Ĭ���ͺ���W25Q128

// SPI��һҳ(0~65535)��д������256���ֽڵ�����
// ��ָ����ַ��ʼд�����256�ֽڵ�����
// buffer: ���ݴ洢��
// addr: ��ʼд��ĵ�ַ(24bit)
// len: Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���
static void W25QXX_Write_Page(u8 *buffer, u32 addr, u16 len)
{
    W25QXX_WriteEnable();
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_PageProgram);
    SPI2_ReadWriteByte((u8)((addr) >> 16));
    SPI2_ReadWriteByte((u8)((addr) >> 8));
    SPI2_ReadWriteByte((u8)addr);
    for (int i = 0; i < len; i++)
    {
        SPI2_ReadWriteByte(buffer[i]);
    }
    W25QXX_CS = 1;
    W25QXX_WaitBusy(); //�ȴ�д�����
}

// �޼���д����
// ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF
// �����Զ���ҳ����
// ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��
// buffer:���ݴ洢��
// addr:��ʼд��ĵ�ַ(24bit)
// len:Ҫд����ֽ���(���65535)
static void W25QXX_Write_NoCheck(u8 *buffer, u32 addr, u16 len)
{
    u16 remainder = 256 - addr % 256; // ��ҳʣ����ֽ���
    if (len <= remainder)
    {
        remainder = len; // ������256���ֽ�
    }
    while (1)
    {
        W25QXX_Write_Page(buffer, addr, remainder);
        if (len == remainder)
        {
            break; //д�������
        }
        else //len>remainder
        {
            buffer += remainder;
            addr += remainder;

            len -= remainder; //��ȥ�Ѿ�д���˵��ֽ���
            if (len > 256)
            {
                remainder = 256;
            } //һ�ο���д��256���ֽ�
            else
            {
                remainder = len;
            } //����256���ֽ���
        }
    }
}

void W25QXX_Init(void)
{
    SPI2_Init();

    W25QXX_TYPE = W25QXX_ReadType(); // ��ȡоƬ�ͺ�
}

// ��ȡW25QXX��״̬�Ĵ���
// BIT 7    6    5    4    3    2    1    0
//     SPR  RV   TB   BP2  BP1  BP0  WEL  BUSY
// SPR: Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
// TB,BP2,BP1,BP0: FLASH����д��������
// WEL: дʹ������
// BUSY: æ���λ(1:æ; 0:����)
// Ĭ��: 0x00
u8 W25QXX_ReadSR(void)
{
    u8 byte = 0;
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_ReadStatusReg);
    byte = SPI2_ReadWriteByte(0Xff);
    W25QXX_CS = 1;
    return byte;
}

// дW25QXX״̬�Ĵ���
// ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д
void W25QXX_WriteSR(u8 sr)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_WriteStatusReg);
    SPI2_ReadWriteByte(sr);
    W25QXX_CS = 1;
}

// W25QXXдʹ��
void W25QXX_WriteEnable(void)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_WriteEnable);
    W25QXX_CS = 1;
}

// W25QXXд��ֹ
void W25QXX_WriteDisable(void)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_WriteDisable);
    W25QXX_CS = 1;
}

// ��ȡоƬ�ͺ�
u16 W25QXX_ReadType(void)
{
    u16 type = 0;
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(0x90);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    SPI2_ReadWriteByte(0x00);
    type |= SPI2_ReadWriteByte(0xFF) << 8;
    type |= SPI2_ReadWriteByte(0xFF);
    W25QXX_CS = 1;
    return type;
}

// ��ȡ����
// ��ָ����ַ��ʼ��ȡָ�����ȵ�����
// buffer: ��������
// addr: ��ʼ��ȡ�ĵ�ַ(24bit)
// len: Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(u8 *buffer, u32 addr, u16 len)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_ReadData);
    SPI2_ReadWriteByte((u8)((addr) >> 16));
    SPI2_ReadWriteByte((u8)((addr) >> 8));
    SPI2_ReadWriteByte((u8)addr);
    for (int i = 0; i < len; i++)
    {
        buffer[i] = SPI2_ReadWriteByte(0XFF);
    }
    W25QXX_CS = 1;
}

// д������
// ��ָ����ַ��ʼд��ָ�����ȵ�����
// �ú�������������
// buffer:���ݴ洢��
// addr:��ʼд��ĵ�ַ(24bit)
// len:Ҫд����ֽ���(���65535)
void W25QXX_Write(u8 *buffer, u32 addr, u16 len)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    static u8 databuf[4096];
    secpos = addr / 4096;      // ������ַ
    secoff = addr % 4096;      // �������ڵ�ƫ��
    secremain = 4096 - secoff; // ����ʣ��ռ��С

    if (len <= secremain)
    {
        secremain = len; // ������4096���ֽ�
    }
    while (1)
    {
        W25QXX_Read(databuf, secpos * 4096, 4096); // ������������������
        for (i = 0; i < secremain; i++)            // У������
        {
            if (databuf[secoff + i] != 0XFF)
            {
                break;
            } // ��Ҫ����
        }
        if (i < secremain) // ��Ҫ����
        {
            W25QXX_EraseSector(secpos);     // �����������
            for (i = 0; i < secremain; i++) // ����
            {
                databuf[i + secoff] = buffer[i];
            }
            W25QXX_Write_NoCheck(databuf, secpos * 4096, 4096); // д����������
        }
        else
        {
            W25QXX_Write_NoCheck(buffer, addr, secremain); // д�Ѿ������˵�,ֱ��д������ʣ������.
        }
        if (len == secremain)
        {
            break; // д�������
        }
        else // д��δ����
        {
            secpos++;   // ������ַ��1
            secoff = 0; // ƫ��λ��Ϊ0

            buffer += secremain; // ָ��ƫ��
            addr += secremain;   // д��ַƫ��
            len -= secremain;    // �ֽ����ݼ�
            if (len > 4096)
            {
                secremain = 4096;
            } // ��һ����������д����
            else
            {
                secremain = len;
            } // ��һ����������д����
        }
    }
}

// ��������оƬ
// �ȴ�ʱ�䳬��...
void W25QXX_EraseChip(void)
{
    W25QXX_WriteEnable();
    W25QXX_WaitBusy();
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_ChipErase);
    W25QXX_CS = 1;
    W25QXX_WaitBusy();
}

// ����һ������
// addr:������ַ, ����ʵ����������
// ����һ������������ʱ��:150ms
void W25QXX_EraseSector(u32 addr)
{
    addr *= 4096;
    W25QXX_WriteEnable();
    W25QXX_WaitBusy();
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_SectorErase);
    SPI2_ReadWriteByte((u8)((addr) >> 16));
    SPI2_ReadWriteByte((u8)((addr) >> 8));
    SPI2_ReadWriteByte((u8)addr);
    W25QXX_CS = 1;
    W25QXX_WaitBusy();
}

// �ȴ�����
void W25QXX_WaitBusy(void)
{
    while ((W25QXX_ReadSR() & 0x01) == 0x01)
        ;
}

// �������ģʽ
void W25QXX_PowerDown(void)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_PowerDown);
    W25QXX_CS = 1;
    delay_us(3);
}

// ����
void W25QXX_WakeUp(void)
{
    W25QXX_CS = 0;
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);
    W25QXX_CS = 1;
    delay_us(3);
}
