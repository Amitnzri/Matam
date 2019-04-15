prog: map.o map.o debug.o 
	gcc map.o debug.o -o run

map.o: map.c map.h
	gcc map.c -c 
debug.o: map_example_test.c map.h debug.c
	gcc debug.c -c 
cnr: map.o map.h debug.o 
	gcc map.o debug.o -o run
	./run
clean:
	rm -f *.o
