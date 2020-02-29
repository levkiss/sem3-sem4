PROG = main
CC = c++
C_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.cpp=.o)))
CC_FLAGS := -Wall -Wextra -g -std=c++11

$(PROG): $(OBJ_FILES)
	$(CC) -o $@ $^
	echo "type: make help"

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -rf shell ./obj/*.o *.dSYM $(PROG) testing

test:
	./testing	

run:
	./$(PROG)

valgrind:
	valgrind --track-origins=yes --leak-check=full ./$(PROG)

help:
	echo "You can use make with clean/run/valgrind/help"