/******************************************
文件名称: basic.c
作	  者: 
版    本:
说    明: 基础工具函数
修改记录: 
******************************************/
#include "all_use.h"




/* 4字节转化为Long */
DWORD four_char_to_long(BYTE* data)
{
	long_c long_temp;
	
	long_temp.cr[0] = data[3];
	long_temp.cr[1] = data[2];
	long_temp.cr[2] = data[1];
	long_temp.cr[3] = data[0];
	return long_temp.ln;
}
/* 2字节转化为short */
WORD two_char_to_short(BYTE* data)
{
	short_c short_temp;
	
	short_temp.cr[0] = data[1];
	short_temp.cr[1] = data[0];
	return short_temp.st;
}
BYTE* long_to_four_char(DWORD data, BYTE* str)
{
	long_c long_temp;
	
	long_temp.ln = data;
	str[0] = long_temp.cr[3];
	str[1] = long_temp.cr[2];
	str[2] = long_temp.cr[1];
	str[3] = long_temp.cr[0];
	return str;
}
BYTE* short_to_two_char(WORD data, BYTE* str)
{
	short_c short_temp;
	
	short_temp.st = data;
	str[0] = short_temp.cr[1];
	str[1] = short_temp.cr[0];
	return str;
}



BYTE hex2bcd(BYTE hex)
{
	hex %= 100;
	return(((hex / 10) << 4) + (hex % 10));
}





void time_hex2bcd(ST_TIMER time, BYTE* bcdtime)
{
	bcdtime[0] = hex2bcd(time.year % 100);
	bcdtime[1] = hex2bcd(time.month);
	bcdtime[2] = hex2bcd(time.day);
	bcdtime[3] = hex2bcd(time.hour);
	bcdtime[4] = hex2bcd(time.min);
	bcdtime[5] = hex2bcd(time.sec);
}




/*******************************************
函数名称: stringIsSame
函数功能: 判断字符串是否相同
入口参数: 
出口参数: 
备    注: 
*******************************************/
BOOL stringIsSame(char* buf, const char* string)
{
	int i;
	for(i = 0; i < strlen(string); i++)
	{
	 	if(buf[i] != string[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}





/*******************************************
函数名称: isIP
函数功能: 判断是不是合法的IP地址
入口参数: 
出口参数: 
备    注: 
*******************************************/
BOOL isIP(const char* ip)
{
	const char* p_str;
	BYTE i;
	BYTE zd;
	DWORD val;
	
	for(i = 0; i < strlen(ip); i++)
    {
        if(!isdigit(ip[i]) && ip[i] != '.')
        {
            return FALSE;
        }
    }
	
    if (strlen(ip) < 7 || strlen(ip) > 15)
    {
		return FALSE;
	}
	
	/* 第1个字段 */
	p_str = ip;
	for(zd = 0; zd < 4; zd++)
	{
		if(!isdigit(*p_str))
		{
			return FALSE;
		}
		val = 0;
		for(i = 0; i < 3; i++)
		{
			if(!isdigit(*p_str))
			{
				break;
			}
			val *= 10;
			val += *p_str - '0';
			++p_str;
		}
		if((*p_str != '.' && *p_str != '\0') || val > 255)
		{
			return FALSE;
		}
		++p_str;
	}
    return TRUE;
}




/*******************************************
函数名称: IPstring2int
函数功能: 
入口参数: 
出口参数: 
备    注: 
*******************************************/
DWORD IPstring2int(const char* ip)
{
	const char* p_str;
	DWORD val;
    DWORD intip;
	BYTE i;
	BYTE zd;
	
	if(isIP(ip))
	{
		intip = 0;
		p_str = ip;
		for(zd = 0; zd < 4; zd++)
		{
			intip <<= 8;
			val = 0;
			for(i = 0; i < 3; i++)
			{
				if(!isdigit(*p_str))
				{
					break;
				}
				val *= 10;
				val += *p_str - '0';
				++p_str;
			}
			intip += val;
			++p_str;
		}
    	return intip;
	}
	
	return 0;
}  




/*******************************************
函数名称: IPint2string
函数功能: 
入口参数: 
出口参数: 
备    注: 
*******************************************/
char* IPint2string(DWORD intIp, char* IP)
{
	BYTE a = intIp >> 24;
	BYTE b = intIp >> 16;
	BYTE c = intIp >> 8;
	BYTE d = intIp;
	sprintf(IP, "%d.%d.%d.%d", a, b, c, d);
	return IP;
}



BOOL isCellPhoneNumber(char* num)
{
	BYTE i = 0;
	
	if(strlen(num) != 11)
	{
		return FALSE;
	}
	
	for(i = 0; i < 11; i++)
	{
		if(!isdigit(num[i]))
		{
			return FALSE;
		}
	}
	return TRUE;
}





//查找字符串中的第N个参数并回传其值
//适用于at指令gps数据以及核心板通信中查找参数
DWORD search_data_para(BYTE para_num, BYTE *the_text)
{
	WORD  j;
	BYTE  i = 0;
	DWORD the_data = 0;
	
	for(j = 0; (the_text[j] != '\0') && (the_text[j] != '\r'); j++)
	{	
		if(the_text[j] == ',' || ((the_text[j] == ':') && (i == 0)))
		{   
			if(para_num)
			{
				para_num--;
			}
			i++;
			if(para_num == 0)
			{	
				j++;
				if(the_text[j] == ' ')
				{
					j++;
				}
				
				while(isdigit(the_text[j]))
				{
					the_data *= 10;
					the_data += the_text[j] - '0';
					j++;
				}
				return(the_data);
			}
		}
		if(j > 1600)
		{
			return(0xffffffff);	
		}
	}
	return(0xffffffff);
}





//查找字符串中的第N个参数并回传其值
//fn为小数点后几位
//适用于at指令gps数据以及核心板通信中查找参数
DWORD search_data_para2(BYTE para_num, BYTE *the_text, BYTE fn)
{
	WORD  j;
	BYTE  i = 0;
	DWORD the_data = 0;
	
	for(j = 0; (the_text[j] != '\0') && (the_text[j] != '\r'); j++)
	{	
		if(the_text[j] == ',' || (the_text[j] == ':' && i == 0))
		{
			if(para_num)
			{
				para_num--;
			}
			i++;
			if(para_num == 0)
			{
				j++;
				if(the_text[j] == ' ') 
				{
					j++;
				}
				
				while(isdigit(the_text[j]))
				{
					the_data *= 10;
					the_data += the_text[j] - '0';
					j++;	
				}
				if(fn)
				{
					if((the_text[j] == '.'))
					{
						j++;
						while(isdigit(the_text[j]))
						{
							the_data *= 10;
							the_data += the_text[j] - '0';
							j++;
							fn--;
							if(fn == 0)
							{								
								break;
							}
						}
					}
					while(fn)
					{
						the_data *= 10;
						fn--;
					}
				} 
				return(the_data);
			}
		}
		if(j > 1600) 
		{
			return(0xffffffff);	
		}
	}
	return(0xffffffff);
}




//查找字符串中的第N个参数，并回传该参数对应的字符串的首地址偏移
//适用于at指令gps数据以及核心板通信中查找参数
WORD search_text_para(BYTE para_num, BYTE *the_text)
{	
	WORD j;
	BYTE i = 0;
	
	for(j = 0; (the_text[j] != '\0') && (the_text[j] != '\r'); j++)
	{
		if(the_text[j] == ',' || ((the_text[j] == ':') && (i == 0)))
		{
			if(para_num)
			{
				para_num--;
			}
			
			i++;
			if(para_num == 0)
			{
				j++;
				if(the_text[j] == ' ') 
				{
					j++;
				}
				
				return(j);
			}
		}
		if(j > 1600)
		{
			return(0);	
		}
	}
	return(0);
}




//计算参数个数
BYTE check_para_num(BYTE* the_text)
{	
	BYTE j = 0;
	
	for(; ((*the_text) != '\0') && ((*the_text) != '\r') && ((*the_text) != '\n'); the_text++)
	{
		if((*the_text) == ':' || (*the_text) == ',')
		{
			j++;
		}
	}
	return(j);
}




//字符串数字转换为bcd码
BYTE text_to_bcd(BYTE *text, BYTE *bcd, BYTE lenth)
{
	BYTE i = 0;
	
	for(i = 0; i < (lenth / 2); i++)
	{
		bcd[i] = 0x00;
	}
	i = 0;
	while(i < lenth)
	{	
		if((*text == ',') || (*text == 0)) 
		{
			return(i);
		}
		else if(isdigit(*text))
		{	
			*bcd &= 0xf0;
			*bcd |= *text - '0';
			++text;
			++i;
			if((i % 2) == 0)
			{
				++bcd;
			}
			else
			{
				*bcd <<= 4;
			}
		}
		else
		{
			++text;
		}
	}
	return(i);
}





BYTE bcd_to_byte(BYTE bcd,BYTE lenth)
{
	DWORD the_data = 0;
	BYTE temp;

	temp = bcd;
	the_data += temp >> 4;
	the_data *= 10;
	the_data += temp & 0x0f;
	
	return(the_data);
}




BYTE byte_to_bcd(BYTE byte, BYTE lenth)
{
	return ((byte / 10) << 4) + (byte % 10);
}




DWORD bcd_to_data(BYTE *bcd, BYTE lenth)
{
	DWORD the_data = 0;
	
	if(lenth > 4)
	{
		lenth = 4;
	}
	
	while(lenth)
	{
		the_data = the_data * 100;
		the_data += bcd_to_byte(bcd[0], 1);
		bcd++;
		lenth--;
	}
	return(the_data);
}





//数字转bcd
BYTE data_to_bcd(DWORD the_data, BYTE *bcd, BYTE lenth)
{	
	BYTE i;

	for(i = (lenth / 2); i > 0; i--)
	{
		bcd[i - 1] = the_data % 10;
		the_data /= 10;
		bcd[i - 1] |= (the_data % 10) << 4;
		the_data /= 10;
	}
	return(lenth);
}




//文本转bcd码
WORD text_to_hex(BYTE *the_text, BYTE *dst_addr, WORD lenth_limit)
{
	WORD i;
	BYTE temp;
	
	lenth_limit -= 2;
	for(i = 0; i < (lenth_limit / 2); i++)
	{
		if(the_text[i] == 0)
		{
			break;
		}
	}
	if(i == 0)
	{
		return(0);
	}
	i -= 1;
	lenth_limit = i * 2 + 2;
	
	while(1)
	{
		temp = the_text[i];
		if((temp >> 4) < 10)
		{
			dst_addr[i * 2] = '0' + (temp >> 4);
		}
		else
		{
			dst_addr[i * 2] = 'A' + (temp >> 4) - 10;
		}
		if(temp & 0x0F)
		{
			dst_addr[i * 2 + 1] = '0' + (temp & 0x0f);
		}
		else
		{
			dst_addr[i * 2 + 1] = 'A' + (temp & 0x0f) - 10;
		}
		if(i == 0)
		{
			break;	
		}
		i--;
	}
	dst_addr[lenth_limit] = 0;
	return(lenth_limit);
}




WORD data_to_texthex(BYTE *the_data, BYTE *dst_addr, WORD lenth_limit)
{
	WORD i;
	BYTE temp;

	i = lenth_limit - 1;
	
	while(1)
	{
		temp = the_data[i];		
		if((temp >> 4) <= 9)
		{
			dst_addr[i * 2] = '0' + (temp >> 4);
		}
		else
		{
			dst_addr[i * 2] = 'A' + (temp >> 4) - 10;
		}
		
		if(temp & 0x0f)
		{
			dst_addr[i * 2 + 1] = '0' + (temp & 0x0f);
		}
		else
		{
			dst_addr[i * 2 + 1] = 'A' + (temp & 0x0f) -10;
		}

		if(i == 0) 
		{
			break;
		}
		i--;
	}
	dst_addr[lenth_limit * 2] = 0;
	return(lenth_limit);
}



BYTE texthex_to_data(BYTE *the_hex)
{
	BYTE temp;
	
	if(*the_hex == 0)
		return(0);
	
	if((*the_hex) >= '0' && (*the_hex) <= '9')	
		temp = *the_hex - '0';
	else 
		temp = *the_hex - 'A' + 10;
	
	the_hex++;
	temp <<= 4;
	
	if((*the_hex) >= '0' && (*the_hex) <= '9')	
		temp += *the_hex - '0';
	else 
		temp += *the_hex - 'A' + 10;
	
	return(temp);
}




WORD copy_text(BYTE *from, BYTE *to, WORD lenth_limit)
{
	WORD i = 0;
	
	while(*from != 0)
	{
		*to = *from;
		i++;
		to++;
		from++;
		if(i >= lenth_limit)
		{
			break;
		}
	}
	if(i < lenth_limit)
	{
		*to = 0;
	}
	return(i);
}







/*******************************************
函数名称: adjust_zone
函数功能: 调整时区 
入口参数:
出口参数: 
备    注:$GPRMC
*******************************************/
void time_adjust_zone(ST_TIMER *sTime, BYTE zone)
{
	if(zone > 0)
	{
		sTime->hour += zone;
		if(sTime->hour >= 24)
		{	
			sTime->hour -= 24;
			sTime->day++;
			switch(sTime->month)
			{	
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:			
					if(sTime->day > 31)
					{
						sTime->day = 1;
						sTime->month++;
					}
					break;
				case 2:
					if((sTime->year % 4 == 0 && sTime->day > 29)
					|| (sTime->year % 4 != 0 && sTime->day > 28))
					{
						sTime->day = 1;
						sTime->month++;
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					if(sTime->day > 30)
					{
						sTime->day = 1;
						sTime->month++;
					}
					break;
				default:
					sTime->month = 1;
					break;
			}
			if(sTime->month > 12)
			{
				sTime->year += 1;
				sTime->month = 1;
			}
		}
	}
}



