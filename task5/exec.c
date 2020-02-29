#include "exec.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SIZE 5

static int exec_conv(tree);            // Выполняет конвейер команд из указанного дерева
static int exec_cd(char * argv[]);     // Выполняет внутреннюю команду cd
static int exec_pwd(tree);             // Выполняет внутреннюю команду pwd
static int exec_cmd (tree, int);       // Создает процесс для выполнения входной команды
static void kill_process(void);        // Завершает процесс в случае прерывания

static int start_pid = 0;                  // ID исходного процесса
extern int EOF_flag;
intlist * intL = NULL;          // Список процессов зомби

/* Выполняет команды из указанного дерева */
int execute_tree (tree T) {
    int res = 0;
    tree P = T;
    start_pid = getpid();
    /* Пока дерево Р не закончится будем выполнять конвейеры команд */
    while (P != NULL) {
        res = exec_conv(P);
        /*  Проверим условия && и || */
        if ((P->type == AND) && (res != 0)) {
            while ((P->next != NULL) && (P->type == AND)) {
                    P = P->next;
            }
        }
        else if ((P->type == OR) && (res == 0)) {
            while ((P->next != NULL) && (P->type == OR)) {
                P = P->next;
            }
        }
        P = P->next;
    }
    return res;
}

void kill_process(void) {
    int pid;
    pid = getpid();
    fprintf(stderr, "\n");
    if (start_pid != pid) {
        exit(9);
    }
}
/* Выполняет конвейер команд из указанного дерева */
int exec_conv(tree T) {
    tree P = T;
    tree Q = T;
    int pid = 0, pgid=0;      
    int fd[2], old = -1;    // Переменные для преобразования файловых дескрипторов
    int status = 0;         
    signal(SIGINT, kill_process);
    /* Обработаем все  команды конвейера */
    while (P != NULL) {
        if ((P->argv != NULL) && (strcmp(P->argv[0], "cd") == 0)) {
            if (old != -1) {
                close(old);
            }
            if (P->pipe != NULL) {
                if (pipe(fd) < 0) {
                    exit(1);
                }
                old = fd[0];
                close(fd[1]);
            }            status = exec_cd(T->argv);
            return status;
        }
        else if ((P->argv != NULL) && (strcmp(P->argv[0], "exit") == 0)) {
            /* Выйдем из процесса */
            if (getpid() != start_pid) {
                if (T->argv[1] != NULL) {
                    fprintf(stderr, "\n exit cannot have arguments!!");
                    exit(1);
                }
                exit(0);
            } else {
                EOF_flag = 1;
                if (T->argv[1] != NULL) {
                    fprintf(stderr, "\nexit cannot have arguments!!");
                    return 1;
                }
                return 0;
            }
        }
        /* Первая команда конвеера */
        if (P == T) {
            if (P->pipe != NULL) {
                if (pipe(fd) < 0) {
                    exit(1);
                }
                /* Создание копии процесса, перенаправление ввода-вывода и замена тела процесса */
                if ((pid = fork()) == 0) {
                    dup2(fd[1], 1);
                    close(fd[0]);
                    close(fd[1]);
                    exec_cmd(P, pgid);
                }
                old = fd[0];
                close(fd[1]);
            }
            else if ((pid = fork()) == 0) {
                exec_cmd(P, pgid);
            }
        }
        else if (P->pipe == NULL) {
             if ((pid = fork()) == 0) {
                dup2(old, 0);
                close(old);
                exec_cmd(P, pgid);
            }
            close(old);
        }
        else {
            if (pipe(fd) < 0) {
                exit(1);
            }
            /* Создание копии процесса, перенаправление ввода-вывода и замена тела процесса */
            if ((pid = fork()) == 0) {
                dup2(old, 0);
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);
                close(old);
                exec_cmd(P, pgid);
            }
            close(fd[1]);
            close(old);
            old = fd[0];
        }
        /* Переход к следущей команды, с сохранением предыдущей */
        Q = P;
        P = P->pipe;
    }
    while (wait(&status) != -1);
    return status;
}

 /* Выполнение одной программы */
int exec_cmd(tree T, int pgid) {
    int in, out = 0, res = 0; 
    if (T->backgrnd == 1) {
        signal(SIGINT, SIG_IGN);
    }
    /* Иначе вернем стандартную функцию прерывания */
    else {
       signal(SIGINT, SIG_DFL);
    }
    /* Добавим источник ввода */
    if  (T->infile != NULL) {
        if ((in = open(T->infile, O_RDONLY)) < 0) {
            /* Уведомим об ошибке если не удалось прочесть такой файл */
            fprintf(stderr, "%s: Input file does not exist!\n", T->infile);
            exit(1);
        }  
        dup2(in, 0);
        close(in);
    }
    /* Добавим источник вывода */
    if  (T->outfile != NULL) {
        if (T->append == 1) {
            if ((out = open(T->outfile, O_WRONLY | O_CREAT |O_APPEND, 0777)) < 0) {
                /* Уведомим об ошибке если не удалось создать или открыть такой файл */
                fprintf(stderr, "%s: You cant write in this file!\n", T->outfile);
                exit(1);
            }
        }
        else {
            if ((out = open(T->outfile, O_WRONLY | O_CREAT |O_TRUNC, 0777)) < 0) {
                /* Уведомим об ошибке если не удалось создать или открыть такой файл */
                fprintf(stderr, "%s: You cant write in this file!\n", T->outfile);
                exit(1);
            }
        }
        dup2(out, 1);
        close(out);
    }
    /* В случае если команда открыта в фоновом режиме подадим ей на вход EOF */
    if (T->backgrnd == 1) {
        int f = open("/dev/null", O_RDONLY);
        dup2(f, 0);
        if (fork() == 0) {
            execvp(T->argv[0], T->argv);
            exit(0);
        }
        else {
            kill(getpid(), SIGKILL);
        }
    }
    /* Если это сабшелл запустим его */
    if (T->psubcmd != NULL) {
        res = execute_tree(T->psubcmd);
        if (res != 0) {
            exit(1);
        }
        else {
            exit(0);
        }
    }
    /* Проверим не является ли argv[0] встроенной командой pwd */
    else if ((T->argv != NULL) && (strcmp(T->argv[0], "pwd") == 0)) {
            res = exec_pwd(T);
        exit(res);
    }
    /* Просто выполним команду  */
    else {
        execvp(T->argv[0], T->argv);
        fprintf(stderr, "%s: unexpected command!\n", T->argv[0]);
        exit(1);
    }
}
/* Меняет текущий каталог */
int exec_cd(char *argv[]) {
    char *s;
    /* Если аргументов нет, то перейдем в домашнюю папку */
    if (argv[1] == NULL) {
        s = getenv("HOME");
        if (s == NULL) {
            fprintf(stderr, "Error. Can not find folder!\n");
            return 1;
        }
        else
            chdir(s);
    }
    /* Если аргументов больше одного уведомим об ошибке */
    else if (argv[2] != NULL)  {
        fprintf(stderr, "cd has more then one argument! Error\n");
        return 1;
    }
    else if (chdir(argv[1])) {
        perror(argv[1]);
        return 1;
    }
    return 0;
}
/* Указывает полный путь к текущему каталогу */
int exec_pwd(tree T) {
    char * dir;
    int attempt = 1;
    /* Если есть аргументы, уведомим об ошибке */
    if (T->argv[1] != NULL) {
        fprintf(stderr, "pwd has arguments! Error\n");
        return 1;
    }
    /* Запишем наш каталог в dir */
    dir = calloc(SIZE, sizeof(char));
    getcwd(dir, attempt++ * SIZE);
    while (dir[0] == '\0') {
        dir = realloc(dir, SIZE * attempt * sizeof(char));
        getcwd(dir, attempt++ * SIZE);
    }
    /* Печать и освобождение памяти */
    fprintf(stdout, "%s\n", dir);
    free(dir);
    exit(0);
}

/* Создает список всех порожденных процессов-зомби */
void add_zombie(int pid) {
    intlist * tmp;
    tmp = malloc(sizeof(intlist));
    tmp->pid = pid;
    tmp->next = intL;
    intL = tmp;
}