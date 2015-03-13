#ifndef _STRING_H_
#define _STRING_H_
	#include"inc_com.h"

	extern uchar sig_find(string argc,uchar symb);
	extern double tra_number(string DAT);
	extern uchar str_instead(string argc,string symb,string pif);
	extern uchar clear_bub(string argc,uchar buble);
	extern uchar sig_count(string argc,uchar symb);
	extern uchar length(uchar argc[],uchar symb);
	extern void stringcopy(string source,string target);
	extern uchar sig_find(string argc,uchar symb);
	extern bit str_compare(string argc,string symb);
	extern unsigned char findlastbracket(string expression,unsigned char left_brackets);
	extern void sig_instead(string argc,unsigned char symb,unsigned char pif);
#endif
