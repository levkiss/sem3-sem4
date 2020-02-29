#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define N 20
typedef char elemtype[N];

typedef struct Node *link; 
typedef struct Node { 
	elemtype elem;
	struct Node *next; 
} node;
typedef link list; 


list addEl(elemtype el, list l){
	if (l == NULL) {
        l = malloc(sizeof(node));
		for (int i = 0; i < N; i++)
            l->elem[i] = el[i];
        l->next = NULL;
    } else
        l->next = addEl(el, l->next);
   return l;
} //добавляем новый элемент в список

int wrdcmp(elemtype el1, elemtype el2) {
	int i = 0;
	while ((el1[i]==el2[i])&&(el1[i] != '\0')&&(el2[i] != '\0'))
		i++;
	if ((el1[i] == '\0')&&(el2[i]=='\0'))
		return 1;
	else return 0;
} // сравниваем две строки(слова), выдает 0 если различны, 1 если одинаковы

list delEl(list l, elemtype s) {
	if (l->next != NULL) {
		if (wrdcmp(l->elem, s) == 1) {
			list li;
			li = l;
			l = l->next;
			free(li);
			l = delEl(l, s);
		} 
		else 
			l->next = delEl(l->next, s);
	}
	return l;
} // функция удаления элементов списка, совпадающих с последним

void printL(list l) {
    if (l != NULL) {
        int i = 0;
        while (l->elem[i] != '\0'){
            printf("%c", l->elem[i]);
            i++;
        }
        printf(" ");
        printL(l->next);
    }
} // печать списка

void delet(list l) {
    list li;
    while (l != NULL) {
        li = l;
        l = l->next;
        //free(li->elem);
        free(li);
    }
} // удаление списка

int main(){
	list li = NULL;
	int i = 0;
	elemtype el, Lel;
	while (scanf("%s", el) != EOF) {
        li = addEl(el, li); // создаем список
    }
    printf("%s\n", "List is done");
    if (li == NULL){
    	printf("%s\n", "No elements in list");
    }
    list l = li; // ищем последний элемент
	while (l->next != NULL){
		l = l->next;
	}
    while (l->elem[i] != '\0') {
        Lel[i] = l->elem[i];
        i++;
    }
    Lel[i]='\0'; //дописываем в последнее слово \0 в конец
    li = delEl(li, Lel); //удаляем слова
    printL(li);
    delet(li); //удаляем список
    printf("\n%s\n", "List deleted ");
    getchar();
    return 0;
}