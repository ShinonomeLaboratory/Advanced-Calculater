#include<stdio.h>
#include<string.h>

//#include<conio.h>
//use under win
//#include <curses.h>
//use under Linux


#include<malloc.h>
#include<math.h>
//#include<stdlib.h>
#include"cstr.h"
#include"mathwork.h"

#define uchar unsigned char
#define ushort unsigned char
#define uint unsigned int
#define string struct cstring
#define bit short//Keil里不需要 
//#define double float //模拟Keil 51环境才需要
//#define NULL ((void *)0)
#define TMP_STR_LEN 100
#define TMP_DBL_LEN 50


#ifndef _STRING_
#define _STRING_
	string{
		uchar *head;
		uchar len;
	};
#endif
extern uchar err_code[];

