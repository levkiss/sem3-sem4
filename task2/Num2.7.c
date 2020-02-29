#include <stdio.h>
#include <setjmp.h>

jmp_buf begin;

char curlex;

void getlex(void){
	while ((curlex=getchar())==' ');
}
void error(){
	printf("\nОШИБКА\n");
	while(getchar()!='\n');
	longjmp(begin,1);
}
int expr(){
	int e=add();
	while (curlex =='+'|| curlex=='-'){
		if(curlex=='+'){
			getlex();
			e+=add();
		}
		else{
			getlex();
			e-=add();
		}
	}
	return e;
}	
int add(){
	int a=powe();
	int f,os;
	while (curlex=='*'||curlex=='/'){
		if (curlex=='*'){
			getlex();
			a*=powe();
		}
		else {
			getlex();
			f=powe();
			os=a%f;
			a=(a-os)/f;
		}
	}
	return a;
}
int powe(void){
	int po=mult();
	int p=po;
	while (curlex=='^'){
		getlex();
		for (int i=powe();i>1; i--){
			p*=po;
		}
	}
	return p;
}
int mult(){
	int m;
	switch(curlex){
		case '0':case'1':case'2':case '3':case'4':case'5':case '6':case'7':case'8':case'9':m=curlex-'0';
			break;
		case '(':getlex();
			m=expr();
			if (curlex ==')') break;
		default:error();
	}
	getlex();
	return m;
}


int main(){
	int result;
	setjmp(begin);
	printf("==>");
	getlex();
	result=expr();
	if (curlex != '\n') error();
	printf("\n%d\n",result);
	return 0;
}