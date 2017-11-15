
#ifndef __BASIC_H__
#define __BASIC_H__


#include "type.h"



#define _abs(x)	((x)>0? (x):(-(x)))


extern DWORD four_char_to_long(BYTE* data);
extern WORD  two_char_to_short(BYTE* data);
extern BYTE* long_to_four_char(DWORD data, BYTE* str);
extern BYTE* short_to_two_char(WORD data, BYTE* str);
extern BOOL  stringIsSame(char* buf, const char* string);
extern BOOL  isIP(const char* ip);
extern DWORD IPstring2int(const char* ip);
extern char* IPint2string(DWORD intIp, char* IP);
extern BOOL  isCellPhoneNumber(char* num);

extern DWORD search_data_para(BYTE para_num,BYTE *the_text);
extern DWORD search_data_para2(BYTE para_num,BYTE *the_text,BYTE);
extern WORD  search_text_para(BYTE para_num,BYTE *the_text);
extern BYTE  check_para_num(BYTE* the_text);
extern BYTE  ip_to_text(BYTE *ip_data,BYTE *ip_text);
extern WORD  gb_to_unicode(BYTE  *gb_code,BYTE  *uniode_char);
extern BYTE  texthex_to_data(BYTE *the_hex);
extern void  data_to_hex(BYTE k,BYTE *out_data);
extern BYTE  hex2bcd(BYTE hex);
extern void  time_hex2bcd(ST_TIMER time, BYTE* bcdtime);
extern void  time_adjust_zone(ST_TIMER *sTime, BYTE zone);
extern WORD  copy_text_para(BYTE para_num,BYTE *the_text,BYTE *ds_addr,WORD lenth_limit);
extern WORD  search_text(BYTE *ftext,BYTE *the_text,WORD lenth);
extern WORD  data_to_texthex(BYTE *the_data,BYTE *dst_addr,WORD lenth_limit);

#endif

