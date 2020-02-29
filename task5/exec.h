#ifndef EXEC_H
#define EXEC_H

#include <unistd.h>
#include "tree.h"

typedef struct backgrndList {   // Структура списка процессов-зомби
    int pid;
    struct backgrndList *next;
} intlist;

int execute_tree(tree);         // Выполняет команды из указанного дерева
void clear_zombie(intlist *);   // Очищает список зомби процессов
#endif