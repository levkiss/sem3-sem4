#ifndef list_h
#define list_h

typedef char ** list;       //Структура список слов

void build_list(void);      // Создание списка
void print_list(list l);    // Печать списка
void delete_list(list l);   // Удаление списка с высвобождением памяти

#endif