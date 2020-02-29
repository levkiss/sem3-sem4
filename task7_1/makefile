client: client.c
	gcc client.c
	./a.out 8086

server: server.c
	gcc server.c
	./a.out 8086

clean:
	rm -f *.out *.o

valgrind:
	valgrind --tool=memcheck ./server