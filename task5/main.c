#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

#include "list.h"
#include "tree.h"
#include "exec.h"

extern list lst;
extern int EOF_flag;
extern int iferr;
sigjmp_buf START;
tree t = NULL;

//void bash_colors();

void error(int n, char * message){
    /* Если n = 1, то это лексическая ошибка, ничего в этом случае в этой функции делать не надо. Обо всем позаботится build_list() */
    if (n == 1) {
        printf("\nLexic error. %s\n", message);
        return;
    /* Если n = 2, то это синтаксическая ошибка start again */
    } else if (n == 2) {
        printf("Syntaxic error. %s\n", message);
    }
}

void sighndlr() {
    lst = NULL;
    t = NULL;
    printf("\n");
    siglongjmp(START, 1);
} //если получили cntrl - c считываем новую команду

int main(int argc, char * argv[]) {
    while (1) {
        sigsetjmp(START, 1);            // Метка прыжка в случае нажатия Ctrl-C
        iferr = 0;                   // Обнуление ошибки построения дерева
        delete_list(lst);               // Очистка списка
        clear_tree(t);                  // Очистка дерева
        //bash_colors();             
        printf("SHELL$ ");          // Приглашение к вводу
        signal(SIGINT, sighndlr); // Поменяем сигнал прерывания, для очистки ввода
        build_list();                   // Построение списка
        if (EOF_flag == 1)             // Если EOF закончим программу
            break;
        t = build_tree(lst);            // Построим дерево
        if (iferr == 0) {            // Если не возникло ошибки при построении дерева
            //print_tree(t, 1);         // Печать дерева
            execute_tree(t);            // Выполняет команды из дерева
        }
    }
    signal(SIGINT, SIG_DFL);            // Возращение стандартного поведения сигнала
    //clear_zombie(intL);
    printf("\n");
    return 0;
}

/*  void bash_colors()
{
    printf("%s", "\x1b[32m"); // green
    char s[100];
    gethostname(s, 100);
    printf("%s@%s", getenv("USER"), s);
    printf("%s", "\x1B[34m");   // gray 
    getcwd(s, 100);
    printf(":%s$ ", s);
    printf("%s", "\x1B[37m");
    putchar('\n');
} */