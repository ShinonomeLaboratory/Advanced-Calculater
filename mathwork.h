#ifndef _MATHWORK_H_
#define _MATHWORK_H_
	#include"inc_com.h"
/*
	errcode errtype			abt code
		0 	出现除数为0		(math error)
		1 	括号不匹配		(syntax error)
		2   log数据小于0	(math error)
		3	反三角函数越界	(math error)
		4	出现未知输入	(syntax error)
*/
	extern void adv_calc(string exp,double *data_trans);
	extern double formula_calc(string exp,double *data_trans,double x,double y,double z);
	extern void input_trans(char *usr_ipt,double *data_trans);
	extern void fx_killer(string exp,double *data_trans);
	extern void check_data(uchar *exp);
	extern void debug_opt();

#endif
