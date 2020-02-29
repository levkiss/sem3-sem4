#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "tree.h"

#define SIZE 5

static char * curlex;    //указатель текущей лексемы
static int words = 0;                 // Индекс слова в lst
static int brackets = 0;             // Сколько открывающих скобок
static int * cmd_in_br = NULL;  // Количество команд за скобками
int iferr = 0;             

static tree create(void);             // Создает дерево из одного элемента, обнуляет все поля
static void add_arg(tree t);            // Добавляет очередной элемент в массив argv 
static void bracket_manipulation(void); // Добавляет память для массива cmd_in_br
static void clear_brackets(void);       // Очищает память массива для скобок в случае ошибки
static void flush_error(void); 
static void make_shift(int i);       // Обнуление индексных переменных, очистка промежуточной памяти, iferr = 1

tree build_tree(list l) {
    tree begin_cmd; 
    tree prev_cmd;  
    tree cur_cmd;   
    if ((l == NULL) || ((curlex = l[words++]) == NULL)) {
        words = 0;
        return NULL;
    }
    begin_cmd = cur_cmd = create();
    prev_cmd = cur_cmd;
    // check for error
    if ((strcmp(curlex, ")") == 0) || 
        (strcmp(curlex, "<") == 0) || 
        (strcmp(curlex, ">") == 0) || 
        (strcmp(curlex, ">") == 0) || 
        (strcmp(curlex, ";") == 0) || 
        (strcmp(curlex, "&") == 0) || 
        (strcmp(curlex, "||") == 0) || 
        (strcmp(curlex, "&&") == 0) || 
        (strcmp(curlex, "|") == 0)) {
        flush_error();
        error(2, "Wrong symbol found !!");
        return cur_cmd;
    }
        /* Если попался символ '(', то необходимо создать промежуточную ветку в дереве */
    if (strcmp(curlex, "(") == 0) {
        if (l[words] == NULL) {
            flush_error();
            error(2, "No ) found !!");
            return cur_cmd;
        }
        bracket_manipulation();
        cur_cmd = build_tree(l);
        prev_cmd->psubcmd = cur_cmd;
        cur_cmd = prev_cmd;
    /* Иначе просто добавим аргумент; Если есть скобки то необходимо увеличить массив считывающий количество комманд под скобками*/
    } else {
       if ((brackets > 0) && (cur_cmd->argv == NULL)) {
           cmd_in_br[brackets - 1]++;
       }
        add_arg(cur_cmd);
    }
    curlex = l[words++];
    while ((curlex != NULL) && (iferr == 0)) {
        /* Если после ||, && или | ничего нет, то уведомим об ошибке */
        if (((strcmp(curlex, "||") == 0) || (strcmp(curlex, "&&") == 0) || (strcmp(curlex, "|") == 0)) && (l[words] == NULL)) {
            flush_error();
            error(2, "Missing command after ||, && or |.");
            return cur_cmd;
        }
        /* Проверим возможные ошибки */
        if (strcmp(curlex, "(") == 0) {
            flush_error();
            error(2, "No command separator before (.");
            return cur_cmd;
        }
        /* Если попался символ ')', то необходимо вернуться в промежуточную ветку */
        else  if (strcmp(curlex, ")") == 0) {
            if ((brackets != 0) && (cmd_in_br[brackets - 1] == 0)) {
                flush_error();
                error(2, "No commands in brackets.");
                return cur_cmd;
            } else if (brackets == 0)  {
                flush_error();
                error(2, "No ( found !!");
                return cur_cmd;
            }
            /* Возвращаясь, не забудем очищать память*/
            if ((brackets > 0) && (cmd_in_br[brackets - 1] > 1)) {
                cmd_in_br[brackets - 1]--;
                return begin_cmd;
            } else if ((brackets > 0) && (cmd_in_br[brackets - 1] == 1)) {
                cmd_in_br[brackets - 1] = 0;
                cmd_in_br = realloc(cmd_in_br, ((brackets - 1) * sizeof(int)));
                if (brackets == 1) {
                    free(cmd_in_br);
                }
                brackets--;
                return begin_cmd;
            }
        }
        /* Если попался символ '<', то необходимо добавить входной файл */
        else if (strcmp(curlex, "<") == 0) {
            /* Ошибка в случае отсутствия входного файла */
            if (l[words] == NULL)  {
                flush_error();
                error(2, "Nothing after < found !!");
                return cur_cmd;
            }
            curlex = l[words++];
            /* Ошибка в случае двух входных файлов */
            if (cur_cmd->infile != NULL) {
                flush_error();
                error(2, "Only one input file allowed.");
                return cur_cmd;
            }
            /* Добавление файла в дерево */
            long tmp = strlen(curlex);
            cur_cmd->infile= calloc(tmp + 1, sizeof(char));
            strncpy(cur_cmd->infile, curlex, tmp + 1);
            //cur_cmd->infile = curlex;
            curlex = l[words++];
        }
        /* Если попался символ '>', то необходимо добавить входной файл */
        else if ((strcmp(curlex, ">") == 0) || (strcmp(curlex, ">>") == 0)) {
            /* Ошибка в случае двух выходных файлов */
            if (cur_cmd->outfile != NULL) {
                flush_error();
                error(2, "Only one output file allowed.");
                return cur_cmd;
            }
            /* Ошибка в случае отсутствия выходного файла */
            if (l[words] == NULL)  {
                flush_error();
                error(2, "Nothing after > (>>).");
                return cur_cmd;
            }
            if (strcmp(curlex, ">>") == 0) {
                cur_cmd->append = 1;
            }
            /* Добавление файла в дерево */
            curlex = l[words++];
            long tmp = strlen(curlex);
            cur_cmd->outfile= calloc(tmp + 1, sizeof(char));
            strncpy(cur_cmd->outfile, curlex, tmp + 1);
            curlex = l[words++];
        }
        /* Если попался символ '|', то необходимо создать новую команду в конвейере */
        else if (strcmp(curlex, "|") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->pipe = cur_cmd;
            if (cur_cmd->backgrnd == 1)
                prev_cmd->backgrnd = 1;
            /* Перенаправим следущие команды конвейера */
            if (cur_cmd->next != NULL) {
                prev_cmd->next = cur_cmd->next;
                cur_cmd->next = NULL;
                prev_cmd->type = cur_cmd->type;
                cur_cmd->type = NXT;
            }
            prev_cmd = cur_cmd;
        }
        /* Если попался символ '||', то необходимо создать новую команду в дереве */
        else if (strcmp(curlex, "||") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = OR;
            prev_cmd = cur_cmd;
        }
        /* Если попался символ '&&', то необходимо создать новую команду в дереве */
        else if (strcmp(curlex, "&&") == 0) {
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = AND;
            prev_cmd = cur_cmd;
        }
        /* Если попался символ ';', то необходимо создать новую команду в дереве */
        else if (strcmp(curlex, ";") == 0) {
            /* Если ';)', то "игнорируем" ';' */
            if ((brackets > 0) && (l[words] != NULL) && (l[words][0] == ')')) {
                curlex = l[words++];
                continue;
            }
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = NXT;
            prev_cmd = cur_cmd;
        }
        /* Если попался символ '&', то необходимо добавить выполнить функцию make_backgrnd(begin_cmd) */
        else if (strcmp(curlex, "&") == 0) {
            cur_cmd->backgrnd = 1;
            /* Если '&)', то "игнорируем" '&' */
            if ((brackets > 0) && (l[words] != NULL) && (l[words][0] == ')')) {
                curlex = l[words++];
                continue;
            }
            cur_cmd = build_tree(l);
            prev_cmd->next = cur_cmd;
            prev_cmd->type = NXT;
            prev_cmd = cur_cmd;
        }
        /* Если попался аргумент, то добавим его к команде */
        else {
            /* Тоже ошибка */
            if ((cur_cmd->infile != NULL) || (cur_cmd->outfile != NULL) || (cur_cmd->psubcmd != NULL)) {
                flush_error();
                error(2, "Unexpected lexem.");
                return cur_cmd;
            }
            add_arg(cur_cmd);
            curlex = l[words++];
        }
    }
    /* Проверим возможную ошибку */
    if ((brackets != 0) && (curlex == NULL)) {
        flush_error();
        error(2, "Missing )");
        return cur_cmd;
    }
    words = 0;
    return begin_cmd;
}
/* Создает массив для считывания количества комманд под скобками*/
void clear_tree(tree T) {
    if (T == NULL)
        return;
    clear_tree(T->next);
    clear_tree(T->pipe);
    clear_tree(T->psubcmd);
    free(T->infile);
    free(T->outfile);
    delete_list(T->argv);
    free(T);
    T = NULL;
}

void print_argv(char **p, int shift) {
    char **q = p;
    if (p != NULL) {
        while (*p != NULL) {
            make_shift(shift);
            fprintf(stderr, "argv[%d]=%s\n",(int) (p - q), *p);
            p++;
        }
    }
}
/* Печать дерева*/
void print_tree(tree t, int shift) {
    char **p;
    if (t == NULL)
        return;
    p = t->argv;
    if (p != NULL)
        print_argv(p, shift);
    else {
        make_shift(shift);
        fprintf(stderr, "psubshell\n");
    }
    make_shift(shift);
    if (t->infile == NULL)
        fprintf(stderr, "infile=NULL\n");
    else
        fprintf(stderr, "infile=%s\n", t->infile);
    make_shift(shift);
    if (t->outfile == NULL)
        fprintf(stderr, "outfile=NULL\n");
    else
        fprintf(stderr, "outfile=%s\n", t->outfile);
    make_shift(shift);
    fprintf(stderr, "append=%d\n", t->append);
    make_shift(shift);
    fprintf(stderr, "background=%d\n", t->backgrnd);
    make_shift(shift);
    fprintf(stderr, "type=%s\n", t->type == NXT ? "NXT": t->type ==OR ? "OR": "AND");
    make_shift(shift);
    if(t->psubcmd == NULL)
        fprintf(stderr, "psubcmd=NULL \n");
    else{
        fprintf(stderr, "psubcmd---> \n");
        print_tree(t->psubcmd, shift + 5);
    }
    make_shift(shift);
    if(t->pipe == NULL)
        fprintf(stderr, "pipe=NULL \n");
    else{
        fprintf(stderr, "pipe---> \n");
        print_tree(t->pipe, shift + 5);
    }
    make_shift(shift);
    if(t->next == NULL)
        fprintf(stderr, "next=NULL \n");
    else {
        fprintf(stderr, "next---> \n");
        print_tree(t->next, shift + 5);
    }
}
/* Удаление дерева с высвобождением памяти */
void bracket_manipulation(void) {
    brackets++;
    if (brackets == 1)
        cmd_in_br = calloc(1, sizeof(int));
    else
        cmd_in_br = realloc(cmd_in_br, (brackets * sizeof(int)));
    cmd_in_br[brackets - 1] = 0;
    if (brackets > 1) {
        cmd_in_br[brackets - 2]++;
    }
}
/* Очищает память массива для скобок в случае ошибки */
void clear_brackets(void) {
    if (brackets != 0) {
        brackets = 0;
        cmd_in_br = realloc(cmd_in_br, 0);
        free(cmd_in_br);
    }
}
/* Обнуление индексных переменных, очистка промежуточной памяти, iferr = 1 */
void flush_error(void) {
    words = 0;
    clear_brackets();
    iferr = 1;
}
/* Создает дерево из одного элемента, обнуляет все поля */
tree create() {
    tree t = NULL;
    t = calloc(1, sizeof(*t));
    t->argv = NULL;
    t->infile = NULL;
    t->outfile = NULL;
    t->append = 0;
    t->backgrnd = 0;
    t->psubcmd = NULL;
    t->pipe = NULL;
    t->next = NULL;
    return t;
}
/* Добавляет очередной элемент в массив argv текущей команды */
void add_arg(tree t) {
    int i = 0;
    if (t->argv == NULL)
        t->argv = calloc(1, sizeof(*t->argv));
    while (t->argv[i] != NULL)
        i++;
    long tmp = strlen(curlex);
    t->argv[i] = calloc(tmp + 1, sizeof(char));
    strncpy(t->argv[i++], curlex, tmp + 1);
    t->argv = realloc(t->argv, ((i + 1) * sizeof(*t->argv)));
    t->argv[i] = NULL;
}
/* Вспомогательная функция для печати дерева */
void make_shift(int n) {
    while (n--)
        putc(' ', stderr);
}