CC = g++
FLAGS = -std=c++11 -Wall -pedantic-errors -Werror -DNDEBUG

run: eurovision.o test2.o 
	$(CC) $^ -o run 

our: eurovision.o iterator_test.o
	$(CC) $^ -o run
eurovision.o: src/eurovision.cpp includes/eurovision.h
	$(CC) $(FLAGS) src/eurovision.cpp -c

iterator_test.o: tests/Our_tests/iterator_test.cpp
	$(CC) $(FLAGS) tests/Our_tests/iterator_test.cpp -c 

test2.o: tests/part_B/test2.cpp
	$(CC) $(FLAGS) tests/part_B/test2.cpp -c

clean:
	rm *.o




