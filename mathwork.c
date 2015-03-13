#include"inc_com.h"

static unsigned int fc_tms=0;
static unsigned int adv_tms=0;
static unsigned int ba_tms=0;

#define _MIN 1e-16

void basic_calc(string expression,double *data_trans){
	double caltmp=0;
	int itmp=0;
	char  i=0,
		  ctmp=0,
		  ltmp=0,
		  hmn=0,
		  j=0;
	ba_tms++;
	hmn=sig_count(expression,'N');
	for(i=expression.len-1;i>=0;i--){
		switch(*(expression.head+i)){
			case '!':
				ltmp=expression.len;
				expression.len=i;
				ctmp=sig_count(expression,'N')-1;
				expression.len=ltmp;

				for(itmp=(int)(*(data_trans+ctmp)-1);itmp>1;itmp--){
					*(data_trans+ctmp)*=itmp;
				}
				*(expression.head+i)=1;
				expression.len-=clear_bub(expression,1);
				i=expression.len;
				break;
			case '^':
				ltmp=expression.len;
				expression.len=i;
				ctmp=sig_count(expression,'N')-1;
				expression.len=ltmp;
			     
				caltmp=pow(*(data_trans+ctmp),*(data_trans+ctmp+1));
				*(data_trans+ctmp)=caltmp;
				
				*(expression.head+i)   =1;
				*(expression.head+i+1) =1;
				expression.len-=clear_bub(expression,1);
				*(data_trans+ctmp+1)=0;
				for(j=ctmp+2;j<hmn;j++){
					*(data_trans+j-1)=*(data_trans+j);
					*(data_trans+j)=0;
				}
				hmn--;
				i=expression.len;
				break;
		}
	}

	for(i=0;i<expression.len;i++){
		switch(*(expression.head+i)){
			case '*':
				ltmp=expression.len;
				expression.len=i;
				ctmp=sig_count(expression,'N')-1;
				expression.len=ltmp;
			
				caltmp=(*(data_trans+ctmp));
				caltmp*=(*(data_trans+ctmp+1));
				*(data_trans+ctmp)=caltmp;
				*(expression.head+i)=1;
				*(expression.head+i+1) =1;
				expression.len-=clear_bub(expression,1);
				*(data_trans+ctmp+1)=0;
				for(j=ctmp+2;j<hmn;j++){
					*(data_trans+j-1)=*(data_trans+j);
					*(data_trans+j)=0;
				}
				hmn--;
				i=0;
				break;
			case '/':
				ltmp=expression.len;
				expression.len=i;
				ctmp=sig_count(expression,'N')-1;
				expression.len=ltmp;

				caltmp=*(data_trans+ctmp);
				if(*(data_trans+ctmp+1)!=0){
					caltmp=caltmp/(*(data_trans+ctmp+1));
				}else{
					caltmp=caltmp*100000000;
					err_code[0]++;
				}
				*(data_trans+ctmp)=caltmp;
				*(expression.head+i)=1;
				*(expression.head+i+1) =1;
				expression.len-=clear_bub(expression,1);
				*(data_trans+ctmp+1)=0;
				for(j=ctmp+2;j<hmn;j++){
					*(data_trans+j-1)=*(data_trans+j);
					*(data_trans+j)=0;
				}
				hmn--;
				i=0;
				break;
		}
	}

	caltmp=0;
	ctmp=0;
	for(i=0;i<expression.len;i++){
		if(*(expression.head+i)=='N'){
			if(i==0){
				caltmp+=*(data_trans+ctmp);
				ctmp++;
			}else if(*(expression.head+i-1)=='+'){
				caltmp+=*(data_trans+ctmp);
				ctmp++;
			}else if(*(expression.head+i-1)=='-'){
				caltmp-=*(data_trans+ctmp);
				ctmp++;
			}
		}
	}//此时此刻，caltmp=ans;
	*data_trans=caltmp;
	for(i=1;i<expression.len;i++){
		*(expression.head+i)=1;
	}
	expression.len-=clear_bub(expression,1);
}

void adv_calc(string exp,double *data_trans){
	unsigned char i=0,
				  jud=0,
				  j=0;
	unsigned char uctmp=0,
				  hmn=0;
	string s_tmp;
	double ans_tmp;
	adv_tms++;
	hmn=sig_count(exp,'N');
reset:;
	  if(exp.len==1) goto END;
	for(i=0;i<exp.len;i++){
		jud=*(exp.head+i)=='('||*(exp.head+i)=='S'||*(exp.head+i)=='C'||*(exp.head+i)=='T'||*(exp.head+i)=='s'||*(exp.head+i)=='c'||*(exp.head+i)=='t'||*(exp.head+i)=='l'||*(exp.head+i)=='n';
		if(jud){
			for(j=i+1;j<exp.len;j++){
				jud=*(exp.head+j)=='('||*(exp.head+j)=='S'||*(exp.head+j)=='C'||*(exp.head+j)=='T'||*(exp.head+j)=='s'||*(exp.head+j)=='c'||*(exp.head+j)=='t'||*(exp.head+j)=='l'||*(exp.head+j)=='n';
				if(jud){
					goto next_i;
				}
				if(*(exp.head+j)==')'){
					goto break_j;
				}
			}//next j
break_j:;
		//exp.head+i 前括号 exp.head+j 后括号
		s_tmp.head=exp.head+i+1;
		s_tmp.len=j-i-1;
		jud=exp.len;
		exp.len=i;
		uctmp=sig_count(exp,'N');
		exp.len=jud;//jud使用完毕
		/*开始统计单元数据量*/
		jud=sig_count(s_tmp,'N')-1;
		hmn=hmn-jud;
		basic_calc(s_tmp,(data_trans+uctmp));
		ans_tmp=*(data_trans+uctmp);
		switch(*(exp.head+i)){
			case 'S':
				if(ans_tmp>=-1&&ans_tmp<=1){
					ans_tmp=asin(ans_tmp);
				}else{
					err_code[3]++;
					ans_tmp=0;
				}
				break;
			case 'T':
				ans_tmp=atan(ans_tmp);
				break;
			case 'C':
				if(ans_tmp>=-1&&ans_tmp<=1){
					ans_tmp=acos(ans_tmp);
				}else{
					err_code[3]++;
					ans_tmp=0;
				}
				break;
			case 's':
				ans_tmp=sin(ans_tmp);
				break;
			case 't':
				ans_tmp=tan(ans_tmp);
				break;
			case 'c':
				ans_tmp=cos(ans_tmp);
				break;
			case 'l':
				if(ans_tmp>0){
					ans_tmp=log10(ans_tmp);//log
				}else{
					ans_tmp=0;
					err_code[2]++;
				}
				break;
			
			case 'n':
				ans_tmp=fabs(ans_tmp);//abs
				break;
			
		}//处理不同的‘括号’函数
		*(exp.head+i)='N';
		*(data_trans+uctmp)=ans_tmp;
		//将括号改变为N,原数据流的第一个数据用作存储
		*(exp.head+i+1)=1;
		*(exp.head+i+2)=1;
		clear_bub(exp,1);
		//原来不管有几个字，现在只剩1个，+j已经移位
		exp.len-=(j-i);//一次性处理长度到位
		//损失的数据数量(N-1)
		for(j=uctmp+1;j<hmn;j++){
			*(data_trans+j)=*(data_trans+j+jud);//数据位移
		}//next j
goto  reset;
		}//End if
next_i:;
	}//next i
END:;
}//本程序稳健性已得到初步认证,不损坏数据和其他序列

double formula_calc(string exp,double *data_trans,double x,double y,double z){
	
	double *temp_data;
	double answer=0;
	unsigned char *temp_str;
	unsigned int i=0;
	unsigned char counter=0;
	unsigned char tflag=0,sflag=0;
	string temp;
	fc_tms++;
	for(i=0;i<exp.len;i++){
		if(*(exp.head+i)=='x'||*(exp.head+i)=='y'||*(exp.head+i)=='z'||*(exp.head+i)=='N'){
			counter++;
		}
	}//扫描需要多少个数据
	temp_data=(double *)malloc(sizeof(double)*(counter+3));
	temp_str=(unsigned char *)malloc(sizeof(unsigned char)*(exp.len+5));
	*temp_str='(';
	temp_str++;
	if(temp_data==NULL||temp_str==NULL){
		return(0);
	}
	for(i=0;i<=exp.len;i++){
		if(*(exp.head+i)=='x'||*(exp.head+i)=='y'||*(exp.head+i)=='z'){
			temp_str[i]='N';
		}else{
			temp_str[i]=*(exp.head+i);
		}
		switch(*(exp.head+i)){
		case 'N':
			temp_data[tflag]=data_trans[sflag];
			tflag++;
			sflag++;
			break;
		case 'x':
			temp_data[tflag]=x;
			tflag++;
			break;
		case 'y':
			temp_data[tflag]=y;
			tflag++;
			break;
		case 'z':
			temp_data[tflag]=z;
			tflag++;
			break;
		default:
			break;
		}
	}//数据准备完成
	temp_str--;
	temp_str[exp.len+1]=')';
	temp_str[exp.len+2]=0;
	temp.head=temp_str;
	temp.len=exp.len+2;
	adv_calc(temp,temp_data);
	answer=*temp_data;
	free(temp_str);temp_str=NULL;
	free(temp_data);temp_data=NULL;
	return(answer);
}

void intg(string exp, double *data_trans){
	//处理积分的
	//形如dm:step:n,exp)
	//即dN:N:N,exp)
	double *start=NULL,
		   *end=NULL,
		   *step=NULL;
	double ans=0,
		   counter=0,
		   tmpans=0,
		   lastdata=0;
	double stk=0;
	unsigned char i=0,
				  tag=1,
				  tmp_tag=0;
	long step_cnt=0;//debug

	start=(data_trans+0);
	end=(data_trans+2);
	step=(data_trans+1);
	if(*start>*end){
		start=(data_trans+2);
		end=(data_trans+0);
	}
	*step=(end[0]-start[0])/pow(2,step[0]);
	//常数处理完毕
	i=findlastbracket(exp,1);
	exp.len=i-5;
	exp.head+=6;
	*(exp.head)='(';
	data_trans+=3;
	for(counter = (*start); counter <= (*end); counter +=(*step)){
		tmpans=formula_calc(exp,data_trans,counter,0,0);
		if(step_cnt==0){
			stk+=tmpans*step[0];
		}else if(step_cnt%2==1){
			stk+=4*tmpans*step[0];
		}else{
			stk+=2*tmpans*step[0];
		}
		if(counter==end[0]||(counter+step[0])>end[0]){
			stk-=tmpans*step[0];
		}
		step_cnt++;
		if(step_cnt%10000==0||(counter+(*step)>(*end))){
			ans+=stk;
			stk=0;
		}
	}//ANS计算完成，录入数据并且消除气泡即可
	ans=ans/3;
	exp.head-=6;
	*(exp.head)='N';
	exp.len+=6;
	data_trans-=3;
	*data_trans=ans;
	for(i=1;i<exp.len;i++){
		*(exp.head+i)=1;
	}
	clear_bub(exp,1);
	//completed
}


void input_trans(char *usr_ipt,double *data_trans){
/*	用户输入的字符串在这里处理
	进行词组替代，数据整理和控制字与数据分流
	主要功能：替代，解决所有的函数与积分
	核心模块：函数的替代(log,ln较重要，还有sqrt(n,exp)尝试转换)，数据的顺序录入(比较重要)；
*/
	//首先替换关键字sin(,cos(,tan(,asin(,acos(,atan(

	string symb;
	string pif;
	string argc;
	unsigned char sym[20]={"asin("};
	unsigned char pi[2]={"S"};
	unsigned char i=0,j=0,pdata_trans=0,tag=1;
	double tmp;

	argc.head=usr_ipt;
	symb.head=sym;
	pif.head=pi;
	argc.len=length(usr_ipt,0);
	sig_instead(argc,',',':');
	argc.len=length(usr_ipt,0);
	//e=2.7182818
	//p=3.1415926
	symb.len=19;
	pif.len=1;
	pi[0]='e';
	strcpy(sym,"2.71828182845904523");
	str_instead(argc,pif,symb);
	pi[0]='p';
	strcpy(sym,"3.14159265358979323");
	str_instead(argc,pif,symb);
	argc.len=length(usr_ipt,0);
	argc.head=usr_ipt;
	argc.len=length(usr_ipt,0);
	symb.len=5;
	symb.head=sym;
	pif.len=1;
	pif.head=pi;

	strcpy(sym,"asin(");
	pi[0]='S';
	str_instead(argc,symb,pif);//ins asin
	strcpy(sym,"acos(");
	pi[0]='C';
	str_instead(argc,symb,pif);//ins acos
	strcpy(sym,"atan(");
	pi[0]='T';
	str_instead(argc,symb,pif);//ins atan
	strcpy(sym,"intg(");
	pi[0]='d';
	str_instead(argc,symb,pif);//ins intg
	strcpy(sym,"difx(");
	pi[0]='v';
	str_instead(argc,symb,pif);//ins difx
	strcpy(sym,"cqux(");
	pi[0]='q';
	str_instead(argc,symb,pif);//ins cqux
	strcpy(sym,"dqux(");
	pi[0]='u';
	str_instead(argc,symb,pif);//ins dqux

	symb.len=4;

	strcpy(sym,"sin(");
	pi[0]='s';
	str_instead(argc,symb,pif);//ins sin
	strcpy(sym,"abs(");
	pi[0]='n';
	str_instead(argc,symb,pif);//ins sin
	strcpy(sym,"cos(");
	pi[0]='c';
	str_instead(argc,symb,pif);//ins cos
	strcpy(sym,"tan(");
	pi[0]='t';
	str_instead(argc,symb,pif);//ins tan
	strcpy(sym,"log(");
	pi[0]='l';
	str_instead(argc,symb,pif);//ins log
	
	symb.len=2;
	strcpy(sym,"y'");
	pi[0]='z';
	str_instead(argc,symb,pif);//ins y'

	for(i=0;i<argc.len;i++){
		if(*(argc.head+i)=='-'){
			if((i!=0)&&(*(argc.head+i-1)=='d'||*(argc.head+i-1)=='u'||*(argc.head+i-1)=='q'||*(argc.head+i-1)=='v'||*(argc.head+i-1)=='^'||*(argc.head+i-1)=='*'||*(argc.head+i-1)=='/'||*(argc.head+i-1)=='('||*(argc.head+i-1)=='S'||*(argc.head+i-1)=='C'||*(argc.head+i-1)=='T'||*(argc.head+i-1)=='s'||*(argc.head+i-1)=='c'||*(argc.head+i-1)=='t'||*(argc.head+i-1)=='l'||*(argc.head+i-1)=='n')){
				if(*(argc.head+i+1)=='.'||(*(argc.head+i+1)>=0x30 && *(argc.head+i+1)<=0x39)){
					for(j=i+1;j<argc.len;j++){
						if(*(argc.head+j)=='.'||(*(argc.head+j)>=0x30&&*(argc.head+j)<=0x39)){
							//NOP
						}else{
							//找到字尾
							break;
						}//END IF
					}//NEXT J
					//BREAK J:
					symb.head=argc.head+i+1;//symb作为寄存器
					symb.len=j-i-1;
					*(data_trans+pdata_trans)=0-tra_number(symb);
					pdata_trans++;
					//然后消除气泡,数据改站，重新计算长度与指针
					*(argc.head+i)='N';
					for(j=j-1;j>i;j--){
						*(argc.head+j)=1;
					}//NEXT J
					argc.len-=clear_bub(argc,1);
				}else{
					*(argc.head+i)=1;
					pi[0]=1;
					strcpy(sym,"0-");
					symb.len=2;
					pif.len=1;
					str_instead(argc,pif,symb);
					i++;
				}
			}
		}else if(*(argc.head+i)=='.'||(*(argc.head+i)>=0x30&&*(argc.head+i)<=0x39)){
			//找到字首
			for(j=i;j<argc.len;j++){
				if(*(argc.head+j)=='.'||(*(argc.head+j)>=0x30&&*(argc.head+j)<=0x39)){
					//NOP
				}else{
					//找到字尾
					break;
				}//END IF
			}//NEXT J
			//BREAK J:
			symb.head=argc.head+i;//symb作为寄存器
			symb.len=j-i;
			*(data_trans+pdata_trans)=tra_number(symb);
			pdata_trans++;
			//然后消除气泡,数据改站，重新计算长度与指针
			*(argc.head+i)='N';
			for(j=j-1;j>i;j--){
				*(argc.head+j)=1;
			}//NEXT J
			argc.len-=clear_bub(argc,1);
		}//END IF
	}//NEXT I
	
	for(i=0;i<argc.len;i++){//处理lN,EXP)->(lEXP)/N)
		if(*(argc.head+i)=='l'){//找到l标志
			symb.head=argc.head;
			symb.len =i;
			pdata_trans=sig_count(symb,'N');
			tmp=*(data_trans+pdata_trans);
			tmp=log10(tmp);
			//先处理N,->(的转变
			*(argc.head+i)='(';
			*(argc.head+i+1)='l';
			*(argc.head+i+2)=1;
			clear_bub(argc,1);
			argc.len--;
			//数据位移=1：
			//再处理)->)/N)
			strcpy(sym,")/N)");
			symb.head=sym;
			symb.len=4;
			pi[0]=1;
			pi[1]=0;
			tag=1;

			for(j=i+1;;j++){
				if(*(argc.head+j)=='d'||*(argc.head+j)=='('||*(argc.head+j)=='S'||*(argc.head+j)=='C'||*(argc.head+j)=='T'||*(argc.head+j)=='s'||*(argc.head+j)=='c'||*(argc.head+j)=='t'||*(argc.head+j)=='l'||*(argc.head+j)=='n'){
					tag++;
				}else if(*(argc.head+j)==')'){
					tag--;
				}else if(*(argc.head+j)=='N'){
					pi[1]+=1;
				}
				if(tag==1){
					break;
				}
			}

			*(argc.head+j)=1;

			argc.len=str_instead(argc,pif,symb);

			for(j=0;j<pi[1];j++){
				*(data_trans+pdata_trans+j)=*(data_trans+pdata_trans+j+1);
			}
			*(data_trans+pdata_trans+pi[1])=tmp;
			//数据的移动完成
			i+=2;
		}
	}
}

void intg_killer(string exp,double *data_trans){
	uchar tmp=0,
		  i=0,
		  tag=0,//消除的数据量+1
		  back=0,//一共的数据量
		  drift=0;//记录数据位移
	string exp_dif;
	exp_dif.head=exp.head;
	back=sig_count(exp,'N');
	while(1){
		tmp=sig_find(exp,'d');//寻找第一个d
		if(tmp==255){
			break;
		}//ELSE:
		drift=0;
		for(i=0;i<=tmp;i++){
			if(*(exp.head+i)=='N'){
				drift++;
			}
		}
		//统计前面的数据
		tag=1;
		for(i=tmp;;i++){
			if(*(exp.head+i)=='('||*(exp.head+i)=='S'||*(exp.head+i)=='C'||*(exp.head+i)=='T'||*(exp.head+i)=='s'||*(exp.head+i)=='c'||*(exp.head+i)=='t'||*(exp.head+i)=='l'||*(exp.head+i)=='n'){
				tag++;
			}else if(*(exp.head+i)==')'){
				tag--;
			}
			if(tag==0){
				break;
			}
		}//用以找寻最后一个括号

		for(;i>tmp;i--){//从最后一个括号往前数
			if(*(exp.head+i)=='N'){
				tag++;
			}
		}
		tag--;
		//统计自己的数据
		exp_dif.head=exp.head+tmp;
		intg(exp_dif,(data_trans+drift));
		//数据扫尾工作
		for(i=drift+1;i<back-tag;i++){
			*(data_trans+i)=*(data_trans+i+tag);
		}
		//最后，back变化
		back-=tag;
	}
}

void parameter_caculate(string exp,double *data_trans){
	//unsigned char str_instead(string argc,string symb,string pif);
	//return slen_new!
	string symb;
	unsigned char symb_dat[2]={0};
	string pif;
	unsigned char pif_dat[5]={0};
	unsigned char 
		find_s=0,
		find_l=0,
		data_before=0,
		data_after=0,
		data_inside=0,
		i=0;
	string calc;

	symb.head=symb_dat;
	symb.len=1;
	pif.head=pif_dat;
	pif.len=2;
	while(1){
		find_s=sig_find(exp,':');
		if(find_s==255){
			break;
		}
		*(exp.head+find_s)=9;
		*symb.head=9;
		*pif.head=')';
		*(pif.head+1)=',';
		str_instead(exp,symb,pif);
		for(find_l=find_s-1;!(*(exp.head+find_l)==':'||*(exp.head+find_l)==','||*(exp.head+find_l)=='d'||*(exp.head+find_l)=='u'||*(exp.head+find_l)=='q'||*(exp.head+find_l)=='v'||*(exp.head+find_l)=='l');find_l--);
		*symb.head=8;
		*pif.head=*(exp.head+find_l);
		*(pif.head+1)='(';
		*(exp.head+find_l)=8;
		str_instead(exp,symb,pif);
		find_s++;
		find_l++;
		calc.head=exp.head+find_s;
		calc.len=length(calc.head,0);
		data_after=sig_count(calc,'N');
		
		calc.head=exp.head;
		calc.len=find_l;
		data_before=sig_count(calc,'N');

		calc.head=exp.head+find_l;
		calc.len=find_s-find_l+1;
		data_inside=sig_count(calc,'N');
		adv_calc(calc,(data_trans+data_before));
		for(i=data_before+1;i<=data_after+data_before;i++){
			*(data_trans+i)=*(data_trans+i+data_inside-1);
		}
	}
}

void difx_killer(string exp,double *data_trans){
	uchar temp=0;
	uchar rubb=0;
	uchar aslen=0;
	uchar dep=0;
	uchar datacount=0;
	double 
		ans=0,
		fxs[4]={0},
		dx=0;
	string reco;
	while(1){
		reco.head=exp.head;
		reco.len=exp.len;
		temp=sig_find(reco,'v');
		if(temp==255){break;}
		aslen=temp;
		rubb=reco.len;
		reco.len=temp;
		temp=sig_count(reco,'N');//之前的数据量
		reco.len=rubb;
		reco.head+=(aslen+2);
		reco.len=findlastbracket(reco,1)+1;
		datacount=sig_count(reco,'N');
		reco.head[0]='(';
		//ans=formula_calc(reco,(data_trans+temp+1),*(data_trans+temp),0,0);
		dep=(unsigned char)fabs(log(ans)/log(2))+5;
		dx=pow(2,0-dep);//FLOAT CHANGE IT
		fxs[0]=formula_calc(reco,(data_trans+temp+1),*(data_trans+temp)-2*dx,0,0);
		fxs[1]=formula_calc(reco,(data_trans+temp+1),*(data_trans+temp) - dx,0,0);
		fxs[2]=formula_calc(reco,(data_trans+temp+1),*(data_trans+temp) + dx,0,0);
		fxs[3]=formula_calc(reco,(data_trans+temp+1),*(data_trans+temp)+2*dx,0,0);
		ans=(fxs[2]-fxs[1]);
		ans*=pow(2,dep);ans/=2;
		//计算结束
		*(data_trans+temp)=ans;
		reco.head-=2;
		for(aslen=1;aslen<=reco.len+1;aslen++){
			reco.head[aslen]=3;
		}
		reco.len+=2;
		clear_bub(reco,3);
		exp.len-=(reco.len-1);
		reco.head[0]='N';
		dep=1;
		for(aslen=1;*(reco.head+aslen)!=0;aslen++){
			if(reco.head[aslen]=='N'){
				*(data_trans+temp+dep)=*(data_trans+temp+dep+datacount);
				dep++;
			}
		}
	}
}

void equx_killer(string exp,double *data_trans){
	unsigned char 
		find_q=0,
		last_bracket=0,
		data_before=0,
		data_after=0
		;
	string strt,temp;
	double
		start_x=0,
		start_x1=0,
		x2=0,
		step=1,
		ans0=0,
		ans[2]={1,-1},
		accg=0,//精度等级
		*data_link; 
	while(1){
		
		strt.head=exp.head;
		strt.len=exp.len;
		temp.head=exp.head;
		temp.len=exp.len;
		find_q=sig_find(strt,'q');
		if(find_q==255){break;}
		temp.len=find_q;
		data_before=sig_count(temp,'N');
		strt.head+=find_q+6;//指向逗号
		*strt.head='(';//补全表达式
		last_bracket=findlastbracket(strt,0);
		strt.len=last_bracket+1;
		temp.head=strt.head+last_bracket;
		temp.len=length(temp.head,0);
		temp.head=exp.head+find_q;
		temp.len=last_bracket+7;
		data_after=sig_count(temp,'N');
		data_link=data_trans+data_before+3;
		start_x=0;
		step=*(data_link-2);
		accg=*(data_link-1)+1;
		accg=pow(0.5,(int)accg);
		ans0=formula_calc(strt,data_link,*(data_link-3),0,0);
		if(fabs(ans0)<=accg){
			ans0=*(data_link-3);
			goto EQU_SOLVED;}
		while(1){
			start_x+=step;
			ans[0]=formula_calc(strt,data_link,*(data_link-3)+start_x,0,0);
			ans[1]=formula_calc(strt,data_link,*(data_link-3)-start_x,0,0);
			if(fabs(ans[0])<=accg){
				ans0=*(data_link-3)+start_x;
				goto EQU_SOLVED;

			}
			if(fabs(ans[1])<=accg){
				ans0=*(data_link-3)-start_x;
				goto EQU_SOLVED;
			}
			if((ans0*ans[0])<0){
				start_x=*(data_link-3)+start_x;
				break;
			}
			if((ans0*ans[1])<0){
				start_x=*(data_link-3)-start_x;
				break;
			}
		}
		//到这儿解出了初始条件:*data_link-3与start_x
		start_x1=*(data_link-3);
		while(1){
			ans[0]=formula_calc(strt,data_link,start_x,0,0);
			ans[1]=formula_calc(strt,data_link,start_x1,0,0);
			x2=start_x1-(start_x1-start_x)*ans[1]/(ans[1]-ans[0]);
			if(fabs(x2-start_x1)<=accg){
				ans0=x2;
				goto EQU_SOLVED;
			}else{
				start_x=start_x1;
				start_x1=x2;
			}
		}
		//到这儿ans已经解决
		EQU_SOLVED:;
		data_before=sig_count(strt,'N');
		data_before+=2;
		data_link-=3;
		for(find_q=1;find_q<=data_after;find_q++){
			*(data_link+find_q)=*(data_link+find_q+data_before);
		}
		for(find_q=1;find_q<temp.len;find_q++){
			*(temp.head+find_q)=3;
		}
		clear_bub(temp,3);
		*temp.head='N';
		*data_link=ans0;
	}
}

void dqux_killer(string exp,double *data_trans){
	unsigned char 
		find_u=255,
		lastbracket=0,
		data_before=0,
		data_after=0;
	signed char sign_of_solve=0;
	unsigned char i=0;
	string 
		strt,
		temp;
	double
		x0=0,
		y0=0,
		xq=0,
		yp=0,
		yc=0,
		step=0;
	double 
		*data_link=NULL;
	double
		x_kernel=0,
		x_plus=0,
		x_minus=0,
		delta_x=0
		;
	double
		ax0=0,ax1=0,ax2=0,
		ay0=0,ay1=0;
	//一阶常微分方程基本解的步骤
	//Carelessly uttered,die as soon as born.
	while(1){//ux,y,X,c,exp)
		i=0;
		strt.head=exp.head;
		strt.len=exp.len;
		temp.head=exp.head;
		temp.len=exp.len;

		data_link=data_trans;

		find_u=sig_find(exp,'u');
		
		if(find_u==255){
			break;
		}

		temp.len=find_u;
		data_before=sig_count(temp,'N');

		strt.head+=find_u;
		strt.len=findlastbracket(strt,1)+1;
		temp.head=strt.head+strt.len;
		temp.len=length(temp.head,0);
		strt.head+=8;
		strt.len-=8;
		*strt.head='(';//补全表达式
		data_after=sig_count(temp,'N');
		data_link+=data_before;
		x0=data_link[0];
		y0=data_link[1];
		xq=data_link[2];
		step=(xq-x0);
		step*=pow(0.5,data_link[3]);
		//数据终于初始化完了
		while(1){
			//搜索y'的范围确定解的初始条件
			delta_x=0.1;
			x_kernel=formula_calc(strt,data_link,x0,y0,x_kernel);
			if(fabs(x_kernel)<=_MIN){
				goto GET_YP_ED;
				//yp没变化
			}
			if(x_kernel>=_MIN){
				sign_of_solve=1;
			}else if(x_kernel<=0-1e-6){
				sign_of_solve=-1;
			}
			while(1){
				x_plus=yp+delta_x;
				x_minus=yp-delta_x;
				x_plus=formula_calc(strt,data_link,x0,y0,x_plus);
				x_minus=formula_calc(strt,data_link,x0,y0,x_minus);
				if(x_minus*sign_of_solve<=0){
					x_kernel=yp-delta_x;
					break;
				}
				if(x_plus*sign_of_solve<=0){
					x_kernel=yp+delta_x;
					break;
				}
				delta_x+=0.1;
			}
			//弦截法解y'真值
			ax0=x_kernel;
			ax1=yp;
			while(1){
				ay0=formula_calc(strt,data_link,x0,y0,ax0);
				ay1=formula_calc(strt,data_link,x0,y0,ax1);
				ax2=ax1-(ax1-ax0)*ay1/(ay1-ay0);
				if(fabs(ax2-ax1)<=_MIN){
					yp=ax2;
					goto GET_YP_ED;
				}else{
					ax0=ax1;
					ax1=ax2;
				}
			}
		//到这儿ans已经解决
		//确定下一个y0+=yp*step;x0+=step;
GET_YP_ED:;
			x0+=step;
			//用新的x0和yp解出yc
			ax0=yp-1;
			ax1=yp+1;
			yp=y0+yp*step;
			while(1){
				ay0=formula_calc(strt,data_link,x0,yp,ax0);
				ay1=formula_calc(strt,data_link,x0,yp,ax1);
				ax2=ax1-(ax1-ax0)*ay1/(ay1-ay0);
				if(fabs(ax2-ax1)<=_MIN){
					break;
				}else{
					ax0=ax1;
					ax1=ax2;
				}
			}
			yc=ax2*step+y0;
			y0=(yp+yc)/2;
			x_kernel=yp;
			if(x0>=xq){
				//printf("ans==%lf\n",y0);
				*strt.head=',';
				break;
			}
		}
		//break outed
		temp.head=exp.head+find_u+2;
		temp.len=findlastbracket(temp,1);
		temp.head-=2;
		temp.len+=3;
		x_kernel=(double)length(temp.head,0);

		//清理数据
		data_before=sig_count(temp,'N')-1;
		for(i=1;i<=data_after;i++){
			data_link[i]=data_link[i+data_before];
		}
		*data_link=y0;
		for(i=1;i<temp.len;i++){
			*(temp.head+i)=3;
		}
		*temp.head='N';
		clear_bub(temp,3);
	}//end while large
}// end function

void fx_killer(string exp,double *data_trans){
	parameter_caculate(exp,data_trans);

	difx_killer(exp,data_trans);
	equx_killer(exp,data_trans);
	dqux_killer(exp,data_trans);
	intg_killer(exp,data_trans);
}

void check_data(uchar *exp){
	//用于输入转换完成以后的检测。
	string argc;
	string check,
		   symb;
	uchar sym[10]={0};
	uchar i=0;
	uchar tag=0;
	argc.head=exp;
	argc.len=length(exp,0);
	symb.head=sym;
	for(i=0;i<argc.len;i++){
		if(*(exp+i)=='d'){
			strcpy(sym,"dN,N,N,");
			check.head=exp+i;
			check.len=7;
			symb.len=7;
			if(str_compare(check,symb)==0){
				err_code[4]++;
			}
		}
		if(*(exp+i)=='l'){
			strcpy(sym,"lN,");
			check.head=exp+i;
			check.len=3;
			symb.len=3;
			if(str_compare(argc,symb)==0){
				err_code[4]++;
			}
		}
		if(*(exp+i)==')'){tag--;}
		if(*(exp+i)=='d'||*(exp+i)=='('||*(exp+i)=='S'||*(exp+i)=='C'||*(exp+i)=='T'||*(exp+i)=='s'||*(exp+i)=='c'||*(exp+i)=='t'||*(exp+i)=='l'||*(exp+i)=='n'||*(exp+i)=='v'||*(exp+i)=='q'||*(exp+i)=='u'){
			tag++;
		}	
	}
	if(tag!=0){err_code[1]++;}
}

void debug_opt(){
	//printf("运算量: \n  三元代数式运算:%d次 \n  高级表达式运算:%d次 \n  基础  四则运算:%d次\n",fc_tms,adv_tms,ba_tms);
	printf("Sub Function Exceute Log: \n Complex Algebra Calculate:%d tms \n  Advanced Algebra Calculate:%d tms \n  Basic Algebra Calculate:%d tms\n",fc_tms,adv_tms,ba_tms);
	fc_tms=0;
	adv_tms=0;
	ba_tms=0;
}
