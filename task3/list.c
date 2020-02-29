#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "list.h"



void termlist(){
	if (lst==NULL) return; 
	if (curlist>sizelist-1)
		lst=realloc(lst,(sizelist+1)*sizeof(*lst)); 
	lst[curlist]=NULL; //выравниваем используемую под список память точно по размеру списка
	lst=realloc(lst,(sizelist=curlist+1)*sizeof(*lst)); 
} // заканчиваем ввод заполнение массива и выравниваем используемую память

void null_list() {
    sizelist = 0;
    curlist = 0;
    lst = NULL;
}

int true_sym(int c){
	int truee=1;
	char trues[76]= {"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890_ \t\n|&;><()$/."};
	for (int i=0; i<76; i++){
		if (c == trues[i])
			truee=0;
			//проверяем есть ли j-ый элемент строки в массиве допустимых символов
		}
	erro= truee;
	return truee;	 
} //проверяем, все ли символы в считанном блоке допустимы

void nullbuf(){ 
	buf=NULL;
	sizebuf=0;
	curbuf=0; 
} //обнуляем буфер

void addsym(){
	if (curbuf>sizebuf-1)
		buf=realloc(buf, sizebuf+=SIZE); /* увеличиваем буфер при необходимости */
    buf[curbuf++]=c;
	//printf("%s %c\n", "SYmbol is added -- ", line[inde-1]);
} //добавляем символ

void print_list(list lst){ 
	int i;
	if (lst==NULL) return;
	for (i=0; i<sizelist-1; i++)
		printf("%s\n",lst[i]);
	return;
} //печатаем массив

void addword(){
	if (curbuf>sizebuf-1)
		buf=realloc(buf, sizebuf+=1); /* для записи ’\0’ увеличиваем буфер при необходимости */
	buf[curbuf++]='\0'; /*выравниваем используемую память точно по размеру слова*/ 
	buf=realloc(buf,sizebuf=curbuf);
	if (curlist>sizelist-1)
		lst=realloc(lst, (sizelist+=SIZE)*sizeof(*lst));
	lst[curlist++]=buf; 
	//free(buf);
	printf("%s %s\n",buf ,"Word added ");
	nullbuf();
	/*printf("\nNOw in list\n");
	print_list(lst);
	printf("\nInside\n"); */

	/*if (curbuf>sizebuf-1)
		buf=realloc(buf, sizebuf+=1); //при необходимости увеличиваем размер буфера
	buf[curbuf++]='\0';
	buf=realloc(buf,sizebuf=curbuf); //выравниваем используемую память точно по размеру слова
	if (curlist>sizelist-1)
		lst=realloc(lst, (sizelist+=SIZE)*sizeof(*lst)); //при необходимости увеличиваем массив под список 
	lst[curlist++]=buf; //заносим слова из буфера в массив
	printf("%s %s\n",buf ,"Word added ");
	free(buf);
	nullbuf();  */
} // добавляем новое слово в массив 


void clear_list() {
    int i;
    sizelist = 0;
    curlist = 0;
    if (lst == NULL)
        return;
    for (i=0; lst[i] != NULL; i++)
        free(lst[i]);
    free(lst);
    lst= NULL;
}

void changeEl(list l1, list l2){
	void *lem = *l1;
	//*lem = *l1;
	*l1=*l2;
	*l2=lem;
} //меняем местами два слова в массиве 

void sort_list(list lst) {
    if (lst == 0)
        return;
    int len = sizelist-1;
    for (int i = 1; i < len; i++)
        for (int j = 0; j < len - i; j++)
            if (strcmp(lst[j], lst[j + 1]) > 0)
                changeEl(&lst[j], &lst[j + 1]);
} //сортируем массив методом пузырька	


int symset(int c){ 
	int sym[] = {'|', '&', '(', '>', '<', ';', ')', '\n', '\t', ' ', EOF};
	for (int i=0; i<10; i++) {
		if (c==sym[i]){
			return sym[i];
		}
	}
	return 0;
} // сравниваем символ со специальными

int getsym(){
	if (inde < SIZE){
		inde++;
		//printf("%s %c\n", "SYmbol is taken -- ", line[inde-1]);
		return line[inde-1];
	}
	else{
		fread(line, sizeof(char), SIZE, stdin);
		inde=0;
		return getsym();
	}
} //считываем очередной символ, если строка закончилась, считываем новую


list build_list() {
	typedef enum {Start, Word, Greater, Greater2, Newline, Stop, Error} vertex; 
	vertex V=Start;
	char cp;
	c=getsym();
	//printf("%c\n",c);
	null_list();
	while(1==1) switch(V){
		case Start:
			//printf("%s\n","IN START ");
			if(c==' '||c=='\t') c=getsym();
			else if (c== EOF) { 
				printf("%s\n", "EOF is found !! ");

				termlist();
        	    //clear_list();
        	    print_list(lst);
				V=Stop; 
			}
			else if (c=='\n') { 
				printf("%s\n", "new line ?n is found !! ");
				free(buf);
				termlist();
				//printf("%s %d\n","Length of list is ", sizelist-1);
				V=Newline;
			} 
			else {
				//printf("IN START ___else\n");
				free(buf);
				nullbuf();
				addsym();
				if (true_sym(c)==0){ 
					V = (symset(c)==0)? Word: Greater;
					cp=c;
					c=getsym();
				}
				else {
                    V = Error;
				}
			}
		break;
		case Word:
			//printf("IN WORD\n");
			if (c==EOF){
				printf("EOF found");
				addsym();
				V=Start;
			}
			else if (true_sym(c)!=0){
				V=Error;
			}
			else if(symset(c)==0){ 
				addsym();
				//printf("%s %c\n","Sym is added to word ", c);
   		 		c=getsym();
   		 		//printf("%s\n", "symset ====0");
			}
			else { 
				//printf("%s\n","Word is going to be added ");
				addword();
				V=Start;
			}
		break;
		case Greater: 
			//printf("IN GREATER\n");
			if ((c==cp)&&((c=='|')||(c=='&')||(c=='>'))){
				addsym();
            	c=getsym();
            	V=Greater2;
			}
			else {
            	V=Start;
				addword(); 
			}
		break;
		case Greater2:
			//printf("IN GREATER2\n"); 
			V=Start;
			addword();
		break;
		case Newline:

			return lst;
		break; 
		case Stop:
			//printf("IS GOING TO STOP\n");
			exit(0);
			return lst;
		break;
		case Error:
			printf("%c %s\n",c," -- Wrong symbol given !! Deleting current list, continue");
            free(buf);
            nullbuf();
            V=Start;
            c=getsym();
            while ((c!='\n')&&(c!=EOF))
            	c=getsym();
            //clear_list();
			//return NULL;
		break;
	}
} 