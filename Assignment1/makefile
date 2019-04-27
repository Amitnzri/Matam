
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -g
FLAGS_ND = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -g
VALGRIND_FLAGS = valgrind --leak-check=full --track-origins=yes
OUTPUT_FILE =  run

prog: map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o
	gcc $(FLAGS_ND) map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o libmtm.a -o $(OUTPUT_FILE)

mac: map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o
	gcc $(FLAGS_ND) map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o src/MAC/libmtm.a -o $(OUTPUT_FILE)

eurovisionTests.o: tests/eurovisionTests.c headers/eurovisionTests.h
	gcc $(FLAGS_ND) tests/eurovisionTests.c -c

eurovisionTestsMain.o: tests/eurovisionTestsMain.c headers/eurovisionTests.h
	gcc $(FLAG_ND) tests/eurovisionTestsMain.c -c

eurovision.o: src/eurovision.c
	gcc $(FLAGS_ND) src/eurovision.c -c

map.o: src/map.c headers/map.h
	gcc $(FLAGS_ND) src/map.c -c

debug.o: map_example_test.c headers/map.h debug.c
	gcc debug.c -c

map_example_test.o: headers/test_utilities.h map_example_test.c
	gcc $(FLAGS_ND) map_example_test.c -c

clean:
	rm -f *.o
val: prog

	$(VALGRIND_FLAGS) ./$(OUTPUT_FILE)

run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE)
