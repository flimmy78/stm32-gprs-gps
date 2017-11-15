/******************************************
�ļ�����: flash.c
��	  ��: byy
��    ��:
˵    ��: STM32�ڲ�FlashӦ��
�޸ļ�¼: 2011-10-09,��������
******************************************/
#include "all_use.h"




/*******************************************
�������ƣ�Flash_Write_Str(DWORD Flash_Adr,BYTE*String,WORD NUM)
�������ܣ���Flash��д���ݣ�дǰ�����
��ڲ�����DWORD Flash_Adr��	�ڲ�Flash��ַ
		  BYTE* string��	д�������׵�ַ
		  WORD  NUM��		д�������ֽ���
		  BYTE  Erase��		�Ƿ������־��1������0������
���ڲ����� 
��    ע: ����д��1024�ֽڣ���ò�Ҫ����1000�ֽ�
*******************************************/ 
void Flash_Write_String(DWORD Flash_Adr,BYTE*String,WORD NUM, BYTE Erase)
{
	BYTE Num1,Num2,temp1;
	DWORD Flash_Data;

	if(NUM > 1024) NUM = 1024;
	Num1 = NUM / 4;
	Num2 = NUM % 4;
	FLASH_Unlock();
	if(Erase == 1)
	{
		FLASH_ErasePage(Flash_Adr - Flash_Adr % 1024);
	}
	IWDG_Feed();
	for(temp1 = 0;temp1 < Num1;temp1++)
	{	
		Flash_Data = *String 
				   + ((DWORD)(*(String + 1)) << 8)
				   + ((DWORD)(*(String + 2)) << 16)
				   + ((DWORD)(*(String + 3)) << 24);
		FLASH_ProgramWord(Flash_Adr,Flash_Data);
		Flash_Adr += 4;
		String += 4;
		IWDG_Feed();
	}
	if(Num2 != 0)
	{
		if(Num2 == 1)
		{
			Flash_Data = *String; 
		}
		else if(Num2 == 2)
		{
			Flash_Data = *String 
					   + ((DWORD)(*(String + 1)) << 8);
		}
		else
		{
			Flash_Data = *String 
					   + ((DWORD)(*(String + 1)) << 8)
					   + ((DWORD)(*(String + 2)) << 16);
		}
		FLASH_ProgramWord(Flash_Adr,Flash_Data);
	}
	FLASH_Lock();
}




/*******************************************
�������ƣ�Flash_Read_String()
�������ܣ���Flash������������
��ڲ�����DWORD Flash_Adr��	�ڲ�Flash��ַ
		  BYTE* string��	���������׵�ַ
		  WORD  NUM��		���������ֽ���
���ڲ����� 
��    ע: 
*******************************************/
void Flash_Read_String(DWORD Flash_Adr, BYTE*String, WORD NUM)
{	
	BYTE Num1,Num2,temp1,temp2;
	DWORD Flash_Data;

	Num1 = NUM / 4;
	Num2 = NUM % 4;
	if(Flash_Adr % 4 != 0)
	{
		Flash_Adr -= Flash_Adr % 4;
	}
	FLASH_Unlock();
	for(temp1 = 0;temp1 < Num1;temp1++)
	{
		Flash_Data = *(VDWORD*)(Flash_Adr);
		for(temp2 = 0;temp2 < 4;temp2++)
		{
			*String = Flash_Data % 0x100;
			Flash_Data >>= 8;
			String++;
		}
		Flash_Adr += 4;
	}
	Flash_Data = *(VDWORD*)(Flash_Adr);
	for(temp1 = 0;temp1 < Num2;temp1++)
	{
		*String = Flash_Data % 0x100;
		Flash_Data >>= 8;
		String++;
	}
	FLASH_Lock();
}





/*******************************************
�������ƣ�void write_device_info_to_flash(void)
�������ܣ�����д��Flash
��ڲ�����
���ڲ����� 
��    ע:
*******************************************/
void write_device_info_to_flash(void)
{
	Flash_Write_String(Flash_Page_Device_Info, (BYTE*)&Device, sizeof(DEVICE_INFO), 1);
}




/*******************************************
�������ƣ�void read_device_info_from_flash(void)
�������ܣ�����д��Flash
��ڲ�����
���ڲ����� 
��    ע:
*******************************************/
void read_device_info_from_flash(void)
{
	Flash_Read_String(Flash_Page_Device_Info, (BYTE*)&Device, sizeof(DEVICE_INFO));
}


