#include <fcntl.h>  /* open() and O_XXX flags */
#include <sys/stat.h>   /* S_IXXX flags */
#include <sys/types.h>  /* mode_t */
#include <unistd.h> /* close() */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int same_file(int fd1, int fd2)
{
    struct stat stat1, stat2;
    if (fstat(fd1, &stat1) < 0)
        return -1;
    if (fstat(fd2, &stat2) < 0)
        return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}


int main (int argc, char **argv){

    if (argc == 1)      // 'cat' and 'cat -' are the same
    {
        argv[argc] = malloc(2 * sizeof(char));
        strcpy(argv[argc], "-");
    }
    if (argc==2)
        if (!strcmp(argv[1], "-n")){
            argv[argc] = malloc(2 * sizeof(char));
            strcpy(argv[argc], "-");
        }

    int number_lines = 0;
    int i, lines_cnt = 1;

    for (i = 1; i < argc; ++i)
        if (!strcmp(argv[i], "-n"))
            number_lines = 1;

    for (i = 1; i <= argc; ++i)
    {
        if (argv[i] == NULL)
            break;
        if ((argv[i][0] == '-') && strcmp(argv[i], "-"))
            continue;
        FILE *f;
        if (argv[i][0] == '-')
            f = stdin;
        else
        {
            int test = open(argv[i], O_RDONLY);
            if (test == -1)
            {
                fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
                continue;
            }
            int same = same_file(STDOUT_FILENO, test);
            if (same == -1)
                printf("cat: %s: error while checking file\n", argv[i]);
            if (same == 1)     // same_file can return -1 but who knows what we should do then
            {
                fprintf(stderr, "cat: %s: input file is output file\n", argv[i]);
                close(test);
                continue;
            }
            close(test);
            f = fopen(argv[i], "r");
        }
        float mult = 1.5;
        size_t sz = 256, it = 0;
        char *buf = malloc(sz);
        int c;
        do
        {
            while ((c = fgetc(f)) != EOF)
            {
                buf[it++] = (char)c;
                if (it == sz)
                {
                    sz *= mult;
                    buf = realloc(buf, sz);
                }
                if ((char)c == '\n')
                    break;
            }
            if (it == 0)
                break;
            if (number_lines)
                printf("%d\t", lines_cnt++);
            size_t j;
            for (j = 0; j < it; ++j)
                putc(buf[j], stdout);
            it = 0;
        } while (c != EOF);
        fclose(f);
        free(buf);
    }
    exit(0);
}