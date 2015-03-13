
#include"inc_com.h"

uchar sig_find(string argc,uchar symb){
	/*在字符串argc中寻找symb并且返回首字序号*/
	uchar i=0;
	for(i=0;i<argc.len;i++)
	{
		if(*(argc.head+i)==symb)
		{
			return(i);
		}
	}
	return(255);
}

void stringcopy(string source,string target){
	uchar i=0;
	for(i=0;i<source.len;i++){
		*(target.head+i)=*(source.head+i);
	}
}
uchar length(uchar argc[],uchar symb){
	uchar i=0;
	while(1){
		if(argc[i]==symb||i==255)
		{
			return(i);
		}else{
			i++;
		}/*End if*/
	}/*End While*/
}
uchar sig_count(string argc,uchar symb){
	/*在字符串argc中统计symb的个数*/
	uchar i=0,cnt=0;
	for(i=0;i<argc.len;i++)
	{
		if(*(argc.head+i)==symb)
		{
			cnt++;
		}
	}
	return(cnt);
}

uchar clear_bub(string argc,uchar buble){
	uchar i=0,move=0;
	argc.len=length(argc.head,0);
	for(i=0;i<=argc.len;i++)
	{
buble_find: ;
		*(argc.head+i)=*(argc.head+i+move);
		if(*(argc.head+i)==buble){
			move++;
			goto buble_find;
		}
	}
	return(move);
}

unsigned char str_instead(string argc,string symb,string pif){
	unsigned char i=0,
				  j=0;
	unsigned char slen=0;
	slen=length(argc.head,0);
	for(i=0;i<slen;i++){
		if(*(argc.head+i)==*(symb.head)){
			for(j=i+1;j<symb.len+i;j++){
				if(*(argc.head+j)!=*(symb.head+j-i)){
					goto nexti;
				}
			}
			/*chech completed*/
			if(symb.len==pif.len){
				for(j=0;j<pif.len;j++){
					*(argc.head+i+j)=*(pif.head+j);
				}
			}else if(symb.len>pif.len){
				for(j=0;j<pif.len;j++){
					*(argc.head+i+j)=*(pif.head+j);
				}/*copy first*/
				for(j=i+pif.len;j<=slen;j++){
					*(argc.head+j)=*(argc.head+j+symb.len-pif.len);
				}/*move*/
				slen-=(symb.len-pif.len);
			}else if(symb.len<pif.len){
				for(j=slen;j>=i+symb.len;j--){
					*(argc.head+j+pif.len-symb.len)=*(argc.head+j);
				}/*move first*/
				for(j=0;j<pif.len;j++){
					*(argc.head+i+j)=*(pif.head+j);
				}/*copy second*/
				slen-=(symb.len-pif.len);	
			}
		}
nexti:;
	}/*next i*/
	return(slen);
}

double tra_number(string DAT){
	unsigned char i;
	double rtn=0,hel=0;
	signed short dot=-1;
	for(i=0;i<DAT.len;i++){
		if(*(DAT.head+i)=='.'){
			dot=i;
			break;
		}
	}
	if(dot<=-1){//无小数点
		for(i=0;i<DAT.len;i++){
			rtn*=10;
			rtn+=*(DAT.head+i)-0x30;
		}
		return(rtn);
	}else{

		for(i=0;i<dot;i++){
			rtn*=10;
			rtn += *(DAT.head+i)-0x30;
		}
		for(i=DAT.len-1;i>dot;i--){
			hel/=10;
			hel+= *(DAT.head+i)-0x30;
		}
		hel/=10;
		rtn+=hel;
		return(rtn);
	}
}

bit str_compare(string argc,string symb){
	uchar i=0;
	if(argc.len!=symb.len){
		return(0);
	}
	for(i=0;i<argc.len;i++){
		if(*(symb.head+i)!=*(argc.head+i)){
			return(0);
		}
	}
	return(1);
}

unsigned char findlastbracket(string expression,unsigned char left_brackets){
	unsigned short i=0;
	unsigned short exitflag=0;
	unsigned unpair=0;
	unpair=left_brackets;
	for(i=0;exitflag==0;i++){
		if(expression.head[i]==')'){
			unpair--;
		}else if(*(expression.head+i)=='('||
				 *(expression.head+i)=='S'||
				 *(expression.head+i)=='C'||
				 *(expression.head+i)=='T'||
				 *(expression.head+i)=='s'||
				 *(expression.head+i)=='c'||
				 *(expression.head+i)=='t'||
				 *(expression.head+i)=='l'||
				 *(expression.head+i)=='n'
				 ){
			unpair++;
		}
		if(unpair==0){
			exitflag=1;
		}
	}
	return((unsigned char)(i-1));
}

void sig_instead(string argc,unsigned char symb,unsigned char pif){
	unsigned char i;
	for(i=0;i<argc.len;i++){
		if(argc.head[i]==symb){
			argc.head[i]=pif;
		}
	}
}
