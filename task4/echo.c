#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
    int nflag;      // Если 0 то надо перейти на новую строчку в конце
    

    if (*++argv && !strcmp(*argv,"-n")) { //проверяем на наличие флага
        --argc;
        nflag = 1;
    } 
    else {
        *--argv;
        nflag = 0;
    }
    // выводим оставшиеся аргументы 
    while (--argc > 0)
        printf("%s %s", *++argv, " ");
    // Если nflag = 0, то переходим на новую строку 
    if (!nflag)
        putchar('\n');
    return 0;
}