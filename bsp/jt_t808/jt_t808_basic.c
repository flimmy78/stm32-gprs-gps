/******************************************
�ļ�����: jt_t808_basic.c
��	  ��: byy
��    ��:
˵    ��: JT/T808Э������,��֯
�޸ļ�¼: 2015-06-26,������
******************************************/
#include "all_use.h"



ALARM_STATUS Alarm_stu;
UNIT_STATUS  Status_stu;
const WORD ProtocolMakeCap = 1500;
BYTE ProtocolMakeBuf[ProtocolMakeCap + 1];





/*******************************************
��������: ���ݷ�ת��
��ڲ���: data, ת��ǰ������
          lenth,ת�Ժ������
���ڲ���: ת�����ֽ���
��    ע: ����ͨ��
*******************************************/
WORD t808_reverse_escape(BYTE* data, WORD lenth)
{
	WORD i, j;
	
	for(i = 0; i < lenth; i++)
	{
		if(data[i] == 0x7D && (data[i+1] == 0x01 || data[i+1] == 0x02))
		{
			data[i] += (data[i + 1] - 1);
			for(j = i + 1; j < lenth - 1; j++)
			{
				data[j] = data[j + 1];
			}
			lenth--;
		}
	}
	return(lenth);
}





/*******************************************
��������: T808Э����װ
��ڲ���: id, 	����ID
		  lenth,�����峤��
���ڲ���: ��װ������ݳ���
��    ע: ����ͨ��,û�зְ�����
*******************************************/
WORD t808_make_protocol(WORD id, WORD lenth)
{
	static WORD serial_num = 0;
	WORD ret_lenth = 0;
	BYTE check = 0;
	WORD i, j;
	
	/* ����ʶ */
	ProtocolMakeBuf[0]  = 0x7E;
	/* ��ϢID */
	ProtocolMakeBuf[1]  = (BYTE)(id >> 8);
	ProtocolMakeBuf[2]  = (BYTE)(id >> 0);
	/* ��Ϣ���� */
	ProtocolMakeBuf[3]  = (BYTE)(lenth >> 8);
	ProtocolMakeBuf[4]  = (BYTE)(lenth >> 0);
	/* �ֻ��� */
	ProtocolMakeBuf[5]  = ((Device.phone_num[0]  - '0') << 4) + (Device.phone_num[1]  - '0');
	ProtocolMakeBuf[6]  = ((Device.phone_num[2]  - '0') << 4) + (Device.phone_num[3]  - '0');
	ProtocolMakeBuf[7]  = ((Device.phone_num[4]  - '0') << 4) + (Device.phone_num[5]  - '0');
	ProtocolMakeBuf[8]  = ((Device.phone_num[6]  - '0') << 4) + (Device.phone_num[7]  - '0');
	ProtocolMakeBuf[9]  = ((Device.phone_num[8]  - '0') << 4) + (Device.phone_num[9]  - '0');
	ProtocolMakeBuf[10] = ((Device.phone_num[10] - '0') << 4) + (Device.phone_num[11] - '0');
	/* ��ˮ�� */
	ProtocolMakeBuf[11] = (BYTE)(serial_num >> 8);
	ProtocolMakeBuf[12] = (BYTE)(serial_num >> 0);
	serial_num++;
	/* У���� */
	check = 0;
	for(i = 0; i < lenth + 12; i++)
	{
		check ^= ProtocolMakeBuf[i + 1];
	}
	ProtocolMakeBuf[13 + lenth] = check;
	/* ����ʶ */
	ProtocolMakeBuf[14 + lenth] = 0x7E;
	ret_lenth = 15 + lenth;
	
	/* ת�� */
	for(i = 1; i < ret_lenth - 1; i++)
	{
		if(ProtocolMakeBuf[i] == 0x7E
		|| ProtocolMakeBuf[i] == 0x7D)
		{
			for(j = 0; j < ret_lenth - i; j++)
			{
				ProtocolMakeBuf[ret_lenth-j] = ProtocolMakeBuf[ret_lenth-j-1];
			}
			ProtocolMakeBuf[i + 1] = (ProtocolMakeBuf[i] == 0x7E)? 0x02 : 0x01;
			ProtocolMakeBuf[i] = 0x7D;
			i++;
			ret_lenth++;
		}
	}
	
	return ret_lenth;
}




/*******************************************
��������: ��֯λ����Ϣ
��ڲ���:
���ڲ���: 
��    ע: 
*******************************************/
WORD t808_make_location_info(BYTE* data)
{
	WORD s_lenth = 0;
	
	/* ������ʶ */
	long_to_four_char(Alarm_stu.value, &data[0]);
	/* ״̬ */
	long_to_four_char(Status_stu.value, &data[4]);
	/* ��ά�� */
	if(Gps_info.status == 'A')
	{
		long_to_four_char(Gps_info.la_data, &data[8]);
		long_to_four_char(Gps_info.lo_data, &data[12]);
	}
	else
	{
		long_to_four_char(at.loc_la, &data[8]);
		long_to_four_char(at.loc_lo, &data[12]);
	}
	/* �߶� */
	short_to_two_char(Gps_info.hight, &data[16]);
	/* �ٶ� */
	short_to_two_char(Gps_info.speed, &data[18]);
	/* ���� */
	short_to_two_char(Gps_info.dir, &data[20]);
	/* ʱ�� */
	time_hex2bcd(SysTime, &data[22]);
	s_lenth += 28;
	data += 28;
	
	/* ������Ϣ */
	/* ģ���� */
	data[0] = 0x2B;
	data[1] = 4;
	short_to_two_char(0, &data[2]);
	short_to_two_char(at.state_batte, &data[4]);
	data += 6;
	s_lenth += 6;
	/* GSM�ź����� */
	data[0] = 0x30;
	data[1] = 1;
	data[2] = at.state_csq;
	data += 3;
	s_lenth += 3;
	/* GNSS������ */
	data[0] = 0x31;
	data[1] = 1;
	data[2] = Gps_info.gps_sate;
	data += 3;
	s_lenth += 3;
	
	return(s_lenth);
}




/*******************************************
��������: �������͵�����ѹ�뷢�ͻ���
��ڲ���: 
���ڲ���: 
��    ע: 
*******************************************/
WORD t808_push_data_to_buf(BYTE* data, WORD lenth)
{
	WORD move_lenth = 0;
	
	/* ���ͻ����пռ䲻�� */
	if(at.socket[0].tdata_limit - at.socket[0].tlenth < lenth)
	{
		move_lenth = lenth - (at.socket[0].tdata_limit - at.socket[0].tlenth);
		memmove(at.socket[0].tdata, 
				at.socket[0].tdata + move_lenth, 
				at.socket[0].tlenth - move_lenth);
		at.socket[0].tlenth -= move_lenth;
	}
	
	memcpy(at.socket[0].tdata + at.socket[0].tlenth, data, lenth);
	at.socket[0].tlenth += lenth;
	
	return(lenth);
}







