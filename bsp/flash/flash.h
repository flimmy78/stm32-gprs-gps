#ifndef __FLASH_BYY_H 
#define __FLASH_BYY_H
	
#include "type.h"	
	
#define Flash_Page_Device_Info		0x0801FC00




extern void Flash_Write_String(DWORD Flash_Adr,BYTE*String,WORD NUM,BYTE Erase); 
extern void Flash_Read_String(DWORD Flash_Adr,BYTE*String,WORD NUM);

void write_device_info_to_flash(void);
void read_device_info_from_flash(void);

#endif
