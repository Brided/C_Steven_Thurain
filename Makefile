create:
	gcc -g -Wall -c unbounded_int.c -o unbounded_int.o
	gcc -g -Wall -c test_unbounded.c -o test_unbounded.o
	gcc -g -Wall test_unbounded.o unbounded_int.o -o prog

clean:
	rm *.o
