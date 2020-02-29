#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 16 // max folder name length

int main() {
    char *dir;
    int wordnum = 1;
    dir = calloc(N, sizeof(char));
    getcwd(dir, wordnum++ * N);
    while (dir[0] == '\0') {
        dir = realloc(dir, N * wordnum * sizeof(char));
        getcwd(dir, wordnum++ * N);
    }

    printf("\nNow you are in :: %s\n", dir);
    free(dir);
    return 0;
}
