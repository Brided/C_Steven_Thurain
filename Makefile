create:
	gcc -g -Wall -c unbounded_int.c -o unbounded_int.o
	gcc -g -Wall -c test_unbounded.c -o test_unbounded.o
	gcc -g -Wall -c calc_unbounded_int.c -o calc_unbounded_int.o

	gcc -g -Wall test_unbounded.o unbounded_int.o -o test_unbounded
	gcc -g -Wall calc_unbounded_int.o unbounded_int.o -o calc_unbounded_int

clean:
	rm *.o
