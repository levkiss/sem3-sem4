#include <stdio.h>

//#include <string.h>

#define N 30
char str[N];
double rezult;

double subE(int i){
	double q=10.0;
	int ji,stepen;
	double rez = 1;

	ji=i;
	if (str[ji]=='+'||str[ji]=='-'){
		ji++;
	}
	stepen=0;
	while (str[ji]!='\0'){
		stepen=stepen*10+ (str[ji]-'0');
		ji++;
	}
	if (str[i]=='-'){
		q=0.1;
	}
	for (int j=stepen; j>0; j--){
		rez *=q;
	}
	return(rez);
}

double string2double(char str[N]){
	int i=0;
	int nol=0;
	int sum_pl=0;
	int sum_dro=0;
	double dob, vesh;

	while (str[i]!='.'&&str[i]!='e'&&str[i]!='E'){
		sum_pl= sum_pl*10+(str[i]-'0');
		i++;
	}				//Находим целую часть (числа до точки или е) и заносим в перем sum_pl

	if (str[i]=='e'||str[i]=='E'||(str[i]=='.' && str[i+1]=='e')||(str[i]=='.' && str[i+1]=='E')){   //вариант, когда после целой части идет либо .е, либо е 
		if (str[i]=='.'){		//случай .е увеличиваем индекс на 1, чтобы работать с е
			i++;
		}
		i++;
		//printf("%c %s\n", str[i], "str[i] before call ");
		dob=sum_pl;
		dob*=subE(i);      // умножаем целую часть на 10^x, значение Е вычисляется в функции 
		return dob;
	} 		

	else if (str[i]=='.' && str[i+1]=='\0'){      
		return dob+sum_pl;
		           //если число не содержит элементов после точки и не содержит Е, возвращаем его
	}

	else {	//если после целой части стоит точка, а после нее число	
		i++;
		while (str[i]!='\0'&&str[i]=='0'){		//подсчитываем колличество нулей, идущих сразу после точки
			nol++;
			i++;
		}
		while (str[i]!='\0' && str[i]!='e' && str[i]!='E'){
			sum_dro=sum_dro*10+ (str[i]-'0');
			i++;							//считываем числа после точки в ячейку sum_dro			
		}
		vesh=sum_dro;
		while (vesh>=1){
			vesh/=10;
		}
		for (int j=nol; j>0; j--){		//представляем число из ячейки sum_dro в виде  0.чччччччч
			vesh/=10;
		}
		dob=sum_pl;
		dob+=vesh;
		if (str[i]=='\0'){		//еслм нет е, то число имеет вид чччччч.чччччч , возвращаем
			return dob;
		}
		else if(str[i]=='e'||str[i]=='E'){		//число имеет вид ччччччч.чччччЕЕ(+|-|' ')чччч
			i++;
			dob*=subE(i);      // умножаем целую часть на 10^x, значение Е вычисляется в функции 
			return dob;
    
		}
	}	
}

int main(){
	printf("%s","Введите число ");
	while (scanf("%s",str)!=EOF){
	//scanf("%s",str);
		rezult = string2double(str);
		printf("%.10g\n",rezult);
	}
	
}
