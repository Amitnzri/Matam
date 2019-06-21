CC = g++
FLAGS = -std=c++11 -Wall -pedantic-errors -Werror -DNDEBUG

run: eurovision.o test1.o 
	$(CC) $^ -o run 

eurovision.o: src/eurovision.cpp includes/eurovision.h
	$(CC) $(FLAGS) src/eurovision.cpp -c

test1.o: includes/test1.cpp
	$(CC) $(FLAGS) includes/test1.cpp -c 

clean:
	rm *.o




