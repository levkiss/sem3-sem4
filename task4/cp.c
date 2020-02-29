#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>

//#define BUFSIZ 5

int main (int argc, char **argv){
	
	if (argc != 3)
    {
        fprintf(stderr, "Not 2 arguments are given!! Error\n");
        exit(1);
    }

    struct stat stat_1, stat_2;
    stat(argv[1], &stat_1);
    stat(argv[2], &stat_2);

    char *dirc, *bname;

    if (stat_1.st_ino == stat_2.st_ino) {
    	printf("Same arguments are given!! Error\n");
        exit(1);
    }

    int from, to;

    if ((from = open(argv[1], O_RDONLY, 0)) == -1) 
    {
        fprintf(stderr, "%s : No such file or directory or read protected\n", argv[1]);
        close(from);
        exit(1);
    }

    if S_ISDIR(stat_2.st_mode) {
        bname=basename(argv[2]);

    	if (chdir(bname) == -1){
    		fprintf(stderr, "%s : Can't open directory or read protected\n", argv[2]);
        	exit(1);
    	}
    	else if ((to = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            printf("Wrong second argument !! Error\n");
            close(to);
            close(from);
            exit(1);
        }
    }
    else {
    	if ((to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
            printf("Wrong second argument !! Error\n");
            close(to);
            close(from);
            exit(1);
        }
    }
    char buf[BUFSIZ];  // Буферный массив
    int bait;            // Количество элементов в буфере
    while ((bait = read(from, buf, BUFSIZ)) > 0)
        write(to, buf, bait);

    close(from);
    close(to);
    
    printf("The end!\n");
    return 0;
}