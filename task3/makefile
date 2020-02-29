main: main.o list.o
	gcc -g main.o list.o
 
main.o: main.c list.h
	gcc -g -c -Wall main.c
 
list.o: list.c list.h
	gcc -g -c -Wall list.c
 
clean:
	rm -f *.out *.o