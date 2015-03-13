#include"inc_com.h"
/*主函数用来测试，结构*/
uchar err_code[5]={0};
/*
	errcode errtype			abt code
		0 	出现除数为0		(math error)
		1 	括号不匹配		(syntax error)
		2   log数据小于0	(math error)
		3	反三角函数越界	(math error)
		4	出现未知输入	(syntax error)
*/
unsigned short main(){
	unsigned char exp[255]={0};
	double data_trans[100]={0};
	string expres;
	uchar len=0;
	unsigned char command;
	//printf("  Super MathWork@C By 袁逸凡 \n  加载结束。\n");
	printf("  Super MathWork@C By TsingJyujing \n  Load over. \n");
MAIN_START:;
	//debug for C51
	//signed char i=0;
	//uchar smg[17]={0};
	//input start
		//printf("请输入表达式:\n>");
		printf("Please Input The Expression:\n>");
		scanf("%s",exp+1);
		if(exp[1]=='q'&&exp[2]==0){
			return(0);
		}
		exp[0]='(';
		len=length(exp,0);
		exp[len]=')';
		exp[len+1]=0;
	//input end
	//process start
		input_trans(exp,data_trans);
		check_data(exp);
		expres.head=exp;
		expres.len=length(exp,0);
		fx_killer(expres,data_trans);
		adv_calc(expres,data_trans);
	//process end
	len=length(exp,0)-1;
	if(len!=0){
		len=1;
	}
	/*到这里计算出了一个结果*/
	//printf("  错误产生: %d %d %d %d %d \n",err_code[0],err_code[1],err_code[2],err_code[3],err_code[4]);
	//printf("  表达式警告:%d\n  最终输出:%s\n",len,exp);
	printf("  Error(s) : %d+%d+%d+%d+%d \n",err_code[0],err_code[1],err_code[2],err_code[3],err_code[4]);
	printf("  Warning(s) :%d\n  Rest Expression :%s\n",len,exp);
	debug_opt();
	//printf("表达式数值: %19.15lf\n计算结束，输入q退出，输入其他字符继续:\n",*data_trans);
	printf("Expression Value: %19.15lf\nCalculate ended, input 'q' to exit and other character to continue:\n",*data_trans);
	command=getchar();
	if(command!='q'){
		goto MAIN_START;
	}
	return(0);
}
