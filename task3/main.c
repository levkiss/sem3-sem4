#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "list.h"


//char line[SIZE];


int main(){
	null_list();
	nullbuf();
	while (feof(stdin) == 0){
		//for (int i = 0; i < SIZE; i++)
        //    line[i] = '\n';
		printf("Enter words \n");
		fread(line, sizeof(char), SIZE, stdin);
		inde = 0;
		printf("%s %s\n", "Readed line:", line);
		lst = build_list();
		//printf("%s %d\n","ERROR ", erro);
		if (erro==0){ 
			printf("%s\n", "Building list is completed");
			printf("%s %d\n", "Size of your list is ", sizelist);
			print_list(lst);
			sort_list(lst);
			printf("%s\n", "This is your sorted list");
			print_list(lst);
			clear_list();
			//free(lst);
		}
		erro = 0;
	}
	free(buf);
	clear_list();
	return 0; 
}