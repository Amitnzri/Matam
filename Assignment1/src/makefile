prog: map.o map.o debug.o
	gcc map.o debug.o -o run

map.o: map.c map.h
	gcc map.c -c

debug.o: map_example_test.c map.h debug.c
	gcc debug.c -c

cnr: map.o map.h debug.o
	gcc map.o debug.o -o run
	./run

cnr2: map.o map.h map_example_test.o test_utilities.h
		gcc map.o map_example_test.o -o run
		./run

map_example_test.o: map.o map.h test_utilities.h map_example_test.c
	gcc map_example_test.c -c

clean:
	rm -f *.o
