
OS = csl3
FLAGS = -std=c99 -Wall -pedantic-errors -Werror -g
FLAGS_ND = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG -g
VALGRIND_FLAGS = valgrind --leak-check=full --track-origins=yes
BUILDDIR = build
OUTPUT_FILE =  run

prog: state.o judge.o map.o eurovision.o eurovisionTests.o eurovisionTestsMain.o
	gcc $(FLAGS_ND) $? lib/$(OS)/libmtm.a -o $(OUTPUT_FILE)


external: main.o map.o eurovision.o state.o judge.o
	gcc $(FLAGS_ND) *.o lib/$(OS)/libmtm.a -o $(OUTPUT_FILE)

state.o: src/state.c include/state.h
	gcc $(FLAGS_ND) src/state.c -c


judge.o: src/judge.c include/judge.h
	gcc $(FLAGS_ND) src/judge.c -c

main.o:
	gcc $(FLAGS_ND) ex_tests/main.c -c

eurovisionTests.o: tests/eurovisionTests.c include/eurovisionTests.h
	gcc $(FLAGS_ND) tests/eurovisionTests.c -c

eurovisionTestsMain.o: tests/eurovisionTestsMain.c include/eurovisionTests.h
	gcc $(FLAGS_ND) tests/eurovisionTestsMain.c -c

eurovision.o: src/eurovision.c
	gcc $(FLAGS_ND) src/eurovision.c -c

map.o: src/map.c include/map.h
	gcc $(FLAGS_ND) src/map.c -c

map_example_test.o: include/test_utilities.h map_example_test.c
	gcc $(FLAGS_ND) map_example_test.c -c

clean:
	rm -f *.o

val:$(OUTPUT_FILE)
	$(VALGRIND_FLAGS) ./$(OUTPUT_FILE)

run: prog $(OUTPUT_FILE)
	rm -f *.o
	./$(OUTPUT_FILE)
