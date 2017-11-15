
#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif



#define SWAP(x) (x & 0xFF),((x >> 8) & 0xFF)


typedef enum 
{
	FALSE = 0,
	TRUE = !FALSE
}BOOL;
typedef unsigned char  			BYTE;
typedef unsigned short			WORD;
typedef unsigned long  			DWORD;
typedef volatile unsigned long	VDWORD;
typedef volatile unsigned short	VWORD;
typedef volatile unsigned char	VBYTE;





typedef union
{
	WORD st;
	BYTE  cr[2];	
}short_c;



typedef union
{
	DWORD  ln;
	WORD st[2];
	BYTE  cr[4];	
}long_c;



//时间结构体 
typedef struct  
{ 
	 WORD year; 
	 BYTE month; 
	 BYTE day; 
	 BYTE hour; 
	 BYTE min; 
	 BYTE sec;    
	 BYTE week;    
}ST_TIMER;


#endif  /* __TYPE_H__ */
