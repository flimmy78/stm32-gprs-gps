#ifndef __MSG_RSV_H_
#define __MSG_RSV_H_
#include "type.h"



extern BYTE recv_msg_rsv(BYTE* number, BYTE* msg);
extern WORD send_msg_to_num(char* num, const char *format,...);

extern WORD make_pdu_sms(char* pdu_buf, char* oa, BYTE* msg);


#endif



