PROG = matrix


$(PROG): main.cpp matrix.o
	g++ -g -Wall main.cpp matrix.o -o $(PROG)
	g++ -g -Wall tests.cpp matrix.o -o tests

matrix.o: matrix.cpp matrix.h
	g++ -g -Wall -c $< -o $@

clean:
	rm -f *.o $(PROG) tests

run:
	./$(PROG)
	
test:
	 ./tests