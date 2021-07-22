#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "malloc.h"

#include "led.h"
#include "w25qxx.h"
#include "sdio_sdcard.h"

#include "mass_mal.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "memory.h"
#include "usb_bot.h"

extern u8 Max_Lun; //֧�ֵĴ��̸���,0��ʾ1��,1��ʾ2��.
int main(void)
{
	u8 offline_cnt = 0;
	u8 tct = 0;
	u8 USB_STA;
	u8 Divece_STA;

	delay_init();		 //��ʱ������ʼ��
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	my_mem_init(SRAMIN); //��ʼ���ڲ��ڴ��

	LED_Init();	   //��ʼ����LED���ӵ�Ӳ���ӿ�
	W25QXX_Init(); //��ʼ��W25Q128

	if (SD_Init())
	{
		Max_Lun = 0; //SD������,���ֻ��һ������.
	}
	else //SD ������
	{
		Mass_Memory_Size[1] = SDCardInfo.CardCapacity; //�õ�SD���������ֽڣ�����SD����������4G��ʱ��,��Ҫ�õ�����u32����ʾ
		Mass_Block_Size[1] = 512;					   //��Ϊ������Init����������SD���Ĳ����ֽ�Ϊ512��,��������һ����512���ֽ�.
		Mass_Block_Count[1] = Mass_Memory_Size[1] / Mass_Block_Size[1];
	}
	if (W25QXX_TYPE != W25Q128)
	{
	}
	else //SPI FLASH ����
	{
		Mass_Memory_Size[0] = 1024 * 1024 * 12; //ǰ12M�ֽ�
		Mass_Block_Size[0] = 512;				//����SPI FLASH�Ĳ���������СΪ512
		Mass_Block_Count[0] = Mass_Memory_Size[0] / Mass_Block_Size[0];
	}
	delay_ms(1800);
	USB_Port_Set(0); //USB�ȶϿ�
	delay_ms(700);
	USB_Port_Set(1);											  //USB�ٴ�����
	Data_Buffer = mymalloc(SRAMIN, BULK_MAX_PACKET_SIZE * 2 * 4); //ΪUSB���ݻ����������ڴ�
	Bulk_Data_Buff = mymalloc(SRAMIN, BULK_MAX_PACKET_SIZE);	  //�����ڴ�
	//USB����
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
	delay_ms(1800);
	while (1)
	{
		delay_ms(1);
		if (USB_STA != USB_STATUS_REG) //״̬�ı���
		{
			USB_STA = USB_STATUS_REG; //��¼����״̬
		}
		if (Divece_STA != bDeviceState)
		{
			Divece_STA = bDeviceState;
		}
		tct++;
		if (tct == 200)
		{
			tct = 0;
			LED0 = !LED0; //��ʾϵͳ������
			if (USB_STATUS_REG & 0x10)
			{
				offline_cnt = 0; //USB������,�����offline������
				bDeviceState = CONFIGURED;
			}
			else //û�еõ���ѯ
			{
				offline_cnt++;
				if (offline_cnt > 10)
					bDeviceState = UNCONNECTED; //2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG = 0;
		}
	}
}
