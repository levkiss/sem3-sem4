#ifndef tree_h
#define tree_h

#include "list.h"

enum type_of_next{
    NXT, AND, OR   // виды связей соседних команд в списке команд
};

struct cmd_inf {
    list argv;   // список из имени команды и аргументов
    char *infile;   // переназначенный файл стандартного ввода
    char *outfile;  // переназначенный файл стандартного вывода
    int append;     // =1, если >>
    int backgrnd;   // =1, если команда подлежит выполнению в фоновом режиме
    struct cmd_inf* psubcmd; // команды для запуска в дочернем shell
    struct cmd_inf* pipe; // следующая команда после "|"
    struct cmd_inf* next; // следующая после ";" (или после "&")
    enum type_of_next type;// связь со следующей командой через ; или && или ||
};

typedef struct cmd_inf *tree;
typedef struct cmd_inf node;

tree build_tree(list);          // Создает дерево
void print_tree(tree, int);     // Печатает дерево
void clear_tree(tree);          // Очищает дерево

#endif