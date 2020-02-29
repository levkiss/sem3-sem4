#ifndef __LIST__
#define __LIST__

#include <stdio.h>

#define SIZE 10


typedef char ** list;
int c; /*текущий символ */
list lst; /* список слов (в виде массива)*/
char * buf; /* буфер для накопления текущего слова*/ 
int sizebuf; /* размер буфера текущего слова*/
int sizelist; /* размер списка слов*/
int curbuf; /* индекс текущего символа в буфере*/ 
int curlist; /* индекс текущего слова в списке*/
char line[SIZE];
int erro; 
int inde;

list build_list();
void termlist();
int true_sym(int c);
void nullbuf();
void sort_list(list lst);
void addword();
void null_list();
void addsym();
void print_list(list l); 
void clear_list(); 

#endif