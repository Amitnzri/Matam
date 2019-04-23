
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -g
FLAGS_ND = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -g
VALGRIND_FLAGS = valgrind --leak-check=full --track-origins=yes
OUTPUT_FILE =  run

prog: map.o map.h debug.o
	gcc $(FLAGS_ND) map.o debug.o -o $(OUTPUT_FILE)

map.o: map.c map.h
	gcc map.c -c

debug.o: map_example_test.c map.h debug.c
	gcc debug.c -c

cnr: map.o map.h debug.o
	gcc $(FLAGS_ND) map.o debug.o -o $(OUTPUT_FILE)
	./run

cnr2: map.o map.h map_example_test.o test_utilities.h
	gcc $(FLAGS_ND) map.o map_example_test.o -o $(OUTPUT_FILE)
	./$(OUTPUT_FILE)

map_example_test.o: map.o map.h test_utilities.h map_example_test.c
	gcc map_example_test.c -c

clean:
	rm -f *.o
val:
	$(VALGRIND_FLAGS) ./$(OUTPUT_FILE)

run:
	./$(OUTPUT_FILE)
