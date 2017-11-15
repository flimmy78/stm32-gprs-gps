#ifndef _GB2312_UNICODE_H_
#define _GB2312_UNICODE_H_
#include "type.h"




extern char* gbk2utf(char* gbk);
extern char* utf2gbk(char* utf);
extern void UniToGB(unsigned char *pbuf);

extern int GBKToUnicode(char* gbk);
extern WORD GBToUni(BYTE *uni, BYTE* gbk, WORD limit);



#endif
