/******************************************
文件名称: flash.c
作	  者: byy
版    本:
说    明: STM32内部Flash应用
修改记录: 2011-10-09,程序建立。
******************************************/
#include "all_use.h"




/*******************************************
函数名称：Flash_Write_Str(DWORD Flash_Adr,BYTE*String,WORD NUM)
函数功能：向Flash内写数据，写前需擦除
入口参数：DWORD Flash_Adr，	内部Flash地址
		  BYTE* string，	写入数据首地址
		  WORD  NUM，		写入数据字节数
		  BYTE  Erase，		是否擦除标志，1擦除，0不擦除
出口参数： 
备    注: 最大可写入1024字节，最好不要超过1000字节
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
函数名称：Flash_Read_String()
函数功能：从Flash中连续读数据
入口参数：DWORD Flash_Adr，	内部Flash地址
		  BYTE* string，	读出数据首地址
		  WORD  NUM，		读出数据字节数
出口参数： 
备    注: 
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
函数名称：void write_device_info_to_flash(void)
函数功能：参数写入Flash
入口参数：
出口参数： 
备    注:
*******************************************/
void write_device_info_to_flash(void)
{
	Flash_Write_String(Flash_Page_Device_Info, (BYTE*)&Device, sizeof(DEVICE_INFO), 1);
}




/*******************************************
函数名称：void read_device_info_from_flash(void)
函数功能：参数写入Flash
入口参数：
出口参数： 
备    注:
*******************************************/
void read_device_info_from_flash(void)
{
	Flash_Read_String(Flash_Page_Device_Info, (BYTE*)&Device, sizeof(DEVICE_INFO));
}


