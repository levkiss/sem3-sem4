#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char **argv) {
  DIR *dir;
 // char name[256];
  struct dirent *di;
  if ((dir = opendir(".")) == NULL){
    fprintf(stderr, "I can't open\n", dir);
    return 0; 
  }
  while ((di = readdir(dir)) != 0) {
    //name = di->d_name;
    if (!strcmp(di->d_name, ".") || !strcmp(di->d_name, ".."))
      continue; // не печатаем саму себя же 
    //if (!strcmp(di->d_name[0], ".")) //не печатаем служебные файлы
    printf("%s\t", di->d_name);
  }
  closedir(dir);
  printf("\n");
  return 0;
}