#include "list.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>


#define SIZE 5


extern char ** environ;         // Нужна для $HOME и остальных
static int dollar = 0;        //возможно есть подстановка в слове
static int curbuf = 0;     // Индекс текущего символа в buf
static int current_word;        // Индекс текущего слова в списке lst
static int realloc_count = 1;   // Коэффициент, для расширения buf
static char str[SIZE];          // Массив из N символов
static char *buf;               // Указатель на буфферный массив
static int strcount;           // Индекс текущего символа в str
static int list_size;           // Размер lst
list lst = NULL;
int EOF_flag = 0;


static void add_word_lite(char *p);                 // Вспомогательная функция для add_word
static char * replace(char * a, char * o, char * r, int);  // Вспомогательная функция для подстановки $-перменных


void null_list() {
    list_size = 0;
    current_word = 0;
    lst = NULL;
}
/* Добавление NULL в конце lst и выравнивание памяти */
void termlist() {
    if (lst == NULL) {
        return;
    }
    if (current_word > list_size - 1) {
        lst = realloc(lst, (list_size + 1) * sizeof(*lst));
    }
    lst[current_word] = NULL;
    /*выравниваем используемую под список память точно по размеру списка*/
    lst = realloc(lst, (list_size = current_word + 1) * sizeof(*lst));
}
/* Печать списка */
void print_list(list li) {
    if (li == NULL) {
        return;
    }
    while (*li != NULL)
        printf("%s\n",*li++);
}
/* Удаление списка с высвобождением памяти */
void delete_list(list li) {
    if (li == NULL)
        return;
    int i;
    for (i = 0; li[i] != NULL; i++) {
        free(li[i]);
        li[i] = NULL;
    }    
    free(li);
    li = NULL;
}
/* Функция считывающая из stdin */
int getsym(char stri[SIZE],char ch) {
    if (ch != '\0') {
        return ch;
    }
    if ((strcount == SIZE) || ((curbuf > 0) && (buf[curbuf - 1] == '\n'))) {
        fgets(str, SIZE + 1, stdin);
        strcount = 0;
    }
    if (stri[strcount] != '\0') {
        return stri[strcount++];
    } else
        return EOF;
}

void addsym(char b) {
    /* Проверим сохранили ли мы предыдущее слово (если 0, то да) */
    if (curbuf != 0) {
        if (curbuf == (SIZE * realloc_count)) 
            buf = realloc(buf, (++realloc_count * SIZE));
        buf[curbuf++] = b;
    } else {
        buf = calloc(SIZE, sizeof(char));
        buf[curbuf++] = b;
    }
}

void add_word() {
    /* Проверим сохранили ли мы предыдущее слово (если 0, то да) */
    /* Возможно это первое слово, для данной программы - это одно и тоже*/
    if (curbuf != 0) {
        /* Сохраним слово в lst */
        if (curbuf == (SIZE * realloc_count)) {
            buf = realloc(buf, (realloc_count) * SIZE + 1);
        }
        buf[curbuf++] = '\0';
        if ((dollar > 0 ) && (dollar < 100)) {
            int check1 = 0, check2 = 0;
            /* Проверим не является ли данное слово директорией */
            if (strchr(buf, '$') != NULL) {
                while ((strstr(buf, "$HOME") != NULL) || (strstr(buf, "$SHELL") != NULL) || (strstr(buf, "$USER") != NULL) || (strstr(buf, "$EUID") != NULL)) {
                    char *tmp = NULL;
                    if (strstr(buf, "$SHELL") != NULL) {
                        tmp = replace(buf, "$SHELL", getenv("SHELL"), 1);
                        check1 = 1;
                    }
                    else
                        tmp = buf;
                    if (strstr(buf, "$USER") != NULL) {
                        tmp = replace(tmp, "$USER", getlogin(), 2);
                        check2 = 1;
                    }
                    else
                        if (check1 == 0)
                            tmp = buf;
                    if (strstr(buf, "$HOME") != NULL) {
                        tmp = replace(tmp, "$HOME", getenv("HOME"), 3);
                    }
                    else
                        if ((check1 == 0) && (check2 == 0))
                            tmp = buf;
                    if (strstr(buf, "$EUID") != NULL) {
                        char timed[4];
                        sprintf(timed, "%llu", geteuid());
                        tmp = replace(tmp, "$EUID", timed, 4);
                    }
                    free(buf);
                    buf = NULL;
                    curbuf = 0;
                    realloc_count = 1;
                    for (int i = 0; tmp[i] != '\0'; i++) {
                        addsym(tmp[i]);
                    }
                    if (curbuf == (SIZE * realloc_count)) {
                        buf = realloc(buf, (realloc_count) * SIZE + 1);
                    }
                    buf[curbuf++] = '\0';
                    buf = realloc(buf, curbuf);
                }
            }
            
        }
        dollar = 0;
        buf = realloc(buf, curbuf);
        add_word_lite(buf);
        /* Восстановление индексных переменных */
        curbuf = 0;
        realloc_count = 1;
    }
}
/* Добавление buf в lst */
char * replace(char *str, char *orig, char *rep, int t) {
    static char A1[2048], A2[2048], A3[2048], A4[2048];
    char *p;
    char *clean =NULL;
    p = clean;
    if ((p = strstr(str, orig)) == 0)
        return str;
    if (t == 1) {
        strncpy(A1, str, p - str + 1);
        A1[p - str] = '\0';
        sprintf(A1 + (p - str), "%s%s", rep, p + strlen(orig));
        return A1;
    } else if ( t == 2) {
        strncpy(A2, str, p - str + 1);
        A2[p - str] = '\0';
        sprintf(A2 + (p - str), "%s%s", rep, p + strlen(orig));
        return A2;
    }
    else if (t == 3) {
        strncpy(A3, str, p - str + 1);
        A3[p - str] = '\0';
        sprintf(A3 + (p - str), "%s%s", rep, p + strlen(orig));
        return A3;
    }
    else {
        strncpy(A4, str, p - str + 1);
        A4[p-str] = '\0';
        sprintf(A4 + (p - str), "%s%s", rep, p + strlen(orig));
        return A4;
    }
}

void add_word_lite(char *b) {
    /* Проверим не нужно ли увеличить объем памяти lst */
    if (current_word > list_size - 1) {
        lst = realloc(lst, (list_size += SIZE) * sizeof(*lst));
    }
    lst[current_word++] = b;
    /* Очистка буферной переменной */
    buf = NULL;
}
/* "Объявление" lst */

void  build_list() {
    char b, c;                  // Вспомагательные символьные переменные
    char peeped_char = '\0';    // Переменная, в которой хранится уже просмотренный символ, который надо будет использовать еще раз
    int bckslsh = 0;   
    buf = NULL;            
    curbuf = 0;       
    realloc_count = 1; 
    int again = 1;      // Повторяет ввод при ошибке и EOF
    int kav2x = 0;       // Индикатор двойных кавычек
    int kav1x = 0;// Индикатор одинарных кавычек    
    for (int i = 0; i < SIZE; i++)
        str[i] = '\0';
    /* В случае EOF необходимо выйти из функции */
    if (EOF_flag == 1) {
        lst = NULL;
        return;
    }
    rewind(stdin);
    /* Внешний цикл программы, он необходим, в случае неправильного ввода данных */
    while (again != 0) {
        /* Обнулим переменную необходимую для повтора ввода */
        again = 0;
        /* Обнулим индекс стр */
        strcount = 0;
        /* Присвоим lst значение null */
        null_list();
        /* Cчитаем первые SIZE символов в массив str */
        fgets(str, SIZE + 1, stdin);
        /* Будем вызывать функцию getsym пока не достигнем конца файла */
        while ((b = getsym(str, peeped_char)) != EOF) {
            /* Обнулим подсмотренный символ */
            peeped_char = '\0';
        /* Активный бекслеш */
            if (bckslsh == 1) {
                bckslsh = 0;
                /* Добавим символ b в buf */
                    addsym(b);
            }
            /* Если нам попался спецсимвол, то нужно сохранить предыдущее слово (если оно было или уже не завершенно) и создать новое слово */
            else if ((b == '&') || (b == '|') || (b == ';') || (b == '>') || (b == '<') || (b == ')') || (b == '(') || (b == '#') || (b == '"') || (b == '\'') || (b == '$')) {
        /* Кавычки */
                /* Если попались кавычки, то изменим флаг kav2x */
                if ((b == '"') && (kav1x == 0)) {
                    if  (kav2x == 0) {
                        kav2x = 1;
                    } else {
                        kav2x = 0;
                        add_word();
                    }
                } else if ((b == '"') && (kav1x == 1)) {
                    addsym(b);
                /* Если попались одинарные кавычки, то изменим флаг kav1x */
                } else if ((b == '\'') && (kav2x == 0)) {
                    if  (kav1x == 0) {
                        kav1x = 1;
                    } else {
                        kav1x = 0;
                        add_word();
                    }
                } else if ((b == '\'') && (kav2x == 1)) {
                    addsym(b);
            /* Случай с # */
                } else if (b == '#') {
                    if ((kav2x == 1) || (kav1x == 1)) {
                        addsym(b);
                    }
                    else if ((str[SIZE - 1] == '\n') || (str[SIZE - 1] == '\0')) {
                        break;
                    } else {
                        while (b != '\n')
                            b = getchar();
                        break;
                    }
            /* Случай с $ */
                } else if (b == '$') {
                    if (kav1x == 1) {
                        addsym(b);
                    }
                    else {
                        dollar++;
                        addsym(b);
                    }
        /* Другие спецсимволы и кавычки */
                }
                else if (((b != '"') && (kav2x == 1)) || ((b != '\'') && (kav1x == 1))) {
                    addsym(b);
        /* Обычный случай спецсимвола */
                } else {
                    /* Проверим сохранили ли мы предыдущее слово (если 0, то да) */
                    /* Возможно это первое слово, для данной программы - это одно и тоже*/
                    add_word();
                    /* Зарезервируем память для 2 символов буфера */
                    buf = calloc(2, sizeof(char));
                    /* Добавим первый символ слова в буферную переменную buf */
                    buf[0] = b;
                    buf[1] = '\0';
                    /* Возможно спецсимвол состоит из двух символов. Проверим это */
                    if  ((b == '&') || (b == '|') || (b == '>')) {
                        /* Спецсимвол может состоять из двух символов */
                        /* Считаем следущий символ из входной строки */
                        c  = getsym(str, peeped_char);
                        if (b == c) {
                            /* Спецсимвол состоит из двух символов.  */
                            buf[1] = c;
                            buf = realloc(buf, 3);
                            buf[2] = '\0';
                        } else {
                            /* Получилось так, что спецсимвол состоит из одного символа, сохраним следущий символ в переменную peeped_char */
                            peeped_char = c;
                        }
                    }
                    /* Сохраним слово в lst */
                    add_word_lite(buf);
                }
            }
        /* Случаи с бекслешем */
            else if (b == '\\') {
                if (kav1x == 1) {
                    addsym(b);
                }
                else {
                    /* Возможно это просто символ '\' проверим следующий символ */
                    c  = getsym(str, peeped_char);
                    if (b == c) {
                        /* Это символ \ */
                        addsym(b);
                    }
                    else {
                        peeped_char = c;
                        if ((kav2x == 0) && (kav2x == 0)) {
                            bckslsh = 1;
                        }
                        else if (kav2x == 1) {
                            bckslsh = 1;
                            if ((c != '$') && (c != '"')) {
                                addsym(b);
                            }
                        }
                    }
                }
            }
        /* Пробел и кавычки */
            /* В случае если kav2x = 1 (или kav1x = 1) и b = ' ', то нужно вставить пробел */
            else if ((b == ' ') && ((kav2x == 1) || (kav1x == 1))) {
                addsym(b);
            }
        /* Пробел, табуляция */
            /* Если нам попался символ пробела, табуляции или символ перехода на новую строку, то нужно сохранить предыдущее слово (если оно было или уже не завершенно) */
            else if ((b == ' ') || (b == '\n') || (b == '\t')) {
                    add_word();
                    /* В случае если попался символ перехода на новую строку то выйдем из циклв*/
                    if (b == '\n')
                        break;
            }
        /* Обычный символ */
            /* Если нам попался обычный символ, то продолжим заполнять слово, если его не было, то создадим новое */
            else if (((b >= 'a') && (b <= 'z')) || ((b >= 'A') && (b <= 'Z')) || ((b >= '0') && (b <= '9')) || (b == '_') || (b == '/') || (b == '.') || (b == '-') || (b == ':') || (b == '!') || (b == ',')) {
                addsym(b);
            } else {
                /* Неверные входные данные. Очищаем буфер, список и предлагаем ввести данные заново */
                /* Если EOF то необходимо повторить весь ввод, те again = 1 */
                again = 1;
                kav2x = 0;
                kav1x = 0;
                bckslsh = 0;
                dollar = 0;
                /* Подчищаем все и востанавливаем переменные */
                free(buf);
                termlist();
                delete_list(lst);
                null_list();
                buf = NULL;
                curbuf = 0;
                realloc_count = 1;  // Необходимо проверить нужно ли нам дополнительно счесть символы, чтобы ничего лишнего не осталось
                int clear_stdin = 1;
                /* Если в str есть пустой символ или символ перехода на новую строку, то ничего подчищать не надо */
                for (int i = 0; i < SIZE; i++) {
                    if ((str[i] == '\n') || (str[i] == '\0')) {
                        clear_stdin = 0;
                        break;
                    }
                }
                for (int i = 0; i < SIZE; i++) {
                    str[i] = '\0';
                }
                //strcount = 0;
                /* Считаем все символы до EOF или конца строки, чтобы не "намусорить" */
                if (clear_stdin) {
                    while (((b = getchar()) != EOF) && (b != '\n'));
                }
                /* Сбросим флаг конца файла */
                rewind(stdin);
                /* Сообщим об ошибке */
                error(1, "Wrong symbol.");
                printf("SHELL$ ");
            }
        }
        /* ОШИБКА */
        if ((kav2x == 1) || (kav1x == 1)) {
            again = 1;
            kav2x = 0;
            kav1x = 0;
            bckslsh = 0;
            dollar = 0;
            /* Подчищаем все и востанавливаем переменные */
            free(buf);
            termlist();
            delete_list(lst);
            null_list();
            buf = NULL;
            curbuf = 0;
            realloc_count = 1;  // Необходимо проверить нужно ли нам дополнительно счесть символы, чтобы ничего лишнего не осталось
            int clear_stdin = 1;
            /* Если в str есть пустой символ или символ перехода на новую строку, то ничего подчищать не надо */
            for (int i = 0; i < SIZE; i++) {
                if ((str[i] == '\n') || (str[i] == '\0')) {
                    clear_stdin = 0;
                    break;
                }
            }
            for (int i = 0; i < SIZE; i++) {
                str[i] = '\0';
            }
            if (clear_stdin) {
                while (((b = getchar()) != EOF) && (b != '\n'));
            }
            rewind(stdin);
            error(1, "Check it.");
            printf("SHELL$ ");
        }
    }
    /* Сохраним последнее слово, если его не сохранили */
    if (buf != NULL) {
        add_word();
    }
    /* Выравнивание размера lst и добавление NULL в конце */
    termlist();
    list_size = 0;
    current_word = 0;
    if (b == EOF)
        EOF_flag
       = 1;
}
