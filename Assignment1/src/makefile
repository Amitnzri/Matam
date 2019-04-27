
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -g
FLAGS_ND = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -g
VALGRIND_FLAGS = valgrind --leak-check=full --track-origins=yes
OUTPUT_FILE =  run

prog: map.o map.h eurovision.o eurovision.h eurovisionTests.o ../testseurovisionTests.h eurovisionTestsMain.o list.h
	gcc $(FLAGS_ND) map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o libmtm.a -o $(OUTPUT_FILE)

mac: map.o map.h eurovision.o eurovision.h eurovisionTests.o ../tests/eurovisionTests.h eurovisionTestsMain.o list.h
	gcc $(FLAGS_ND) map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o MAC/libmtm.a -o $(OUTPUT_FILE)

eurovisionTests.o: ../tests/eurovisionTests.c ../tests/eurovisionTests.h
	gcc $(FLAGS_ND) ../tests/eurovisionTests.c -c

eurovisionTestsMain.o: ../tests/eurovisionTestsMain.c ../tests/eurovisionTests.h
	gcc $(FLAG_ND) ../tests/eurovisionTestsMain.c -c

eurovision.o: eurovision.c
	gcc $(FLAGS_ND) eurovision.c -c

map.o: map.c map.h
	gcc $(FLAGS_ND) map.c -c

debug.o: map_example_test.c map.h debug.c
	gcc debug.c -c

cnr: prog
	./$(OUTPUT_FILE)

cnr2: map.o map.h map_example_test.o test_utilities.h
	gcc $(FLAGS_ND) map.o map_example_test.o -o $(OUTPUT_FILE)
	./$(OUTPUT_FILE)

map_example_test.o: map.o map.h test_utilities.h map_example_test.c
	gcc $(FLAGS_ND) map_example_test.c -c

clean:
	rm -f *.o
val: prog

	$(VALGRIND_FLAGS) ./$(OUTPUT_FILE)

run: $(OUTPUT_FILE)
	./$(OUTPUT_FILE)
