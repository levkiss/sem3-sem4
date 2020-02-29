PROG = shell
CFLAGS = -g -Wall

$(PROG): main.c list.o tree.o exec.o 
	$(CC) $(CFLAGS) main.c list.o tree.o exec.o -o $(PROG).exec
list.o: list.c list.h
	$(CC) $(CFLAGS) -c $< -o $@
tree.o: tree.c tree.h list.o 
	$(CC) $(CFLAGS) -c $< -o $@
exec.o: exec.c exec.h tree.o
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROG).exec
run:
	rlwrap ./$(PROG)