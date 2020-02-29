#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	if (argc != 3){
		fprintf(stderr, "%s\n", "Not 2 arguments are given!! " );
		exit(1);
	}
	FILE *f1,*f2;
	char c1,c2;
	int strcount=1; 
	int linecount=1;

    f1 = fopen(argv[1], "r");
    f2 = fopen(argv[2], "r");
    if (f1 == NULL) {
        /* Ошибка при чтении первого файла. Завершаем работу */
        fprintf(stderr, "%s  No such file or directory\n", argv[1]);
        exit(1);
    }
    if (f2 == NULL) {
        /* Ошибка при чтении второго файла. Завершаем работу */
        fprintf(stderr, "%s  No such file or directory\n", argv[2]);

        exit(1);
    }

    c1=getc(f1);
	c2=getc(f2);

	while (!feof(f1) && !feof(f2)){
		if (c1 != c2){
			printf("%s differs from %s: line %d, char %d\n", argv[1], argv[2], linecount, strcount);
            exit(0);
		}
		else if (c1 =='\n'){
			linecount++;
			strcount=0;
		}
		strcount++;
		c1=getc(f1);
		c2=getc(f2);

	}

	if (feof(f1)&&feof(f2)){
		printf("%s have no differs with %s", argv[1], argv[2]);
        exit(0);
	}

	printf("%s differs from %s: line %d, char %d\n", argv[1], argv[2], linecount, ++strcount);
    exit(0);
    return 0;
}