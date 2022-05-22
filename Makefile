create:
	gcc -g -Wall -c unbounded_int.c -o unbounded_int.o
	gcc -g -Wall -c test_unbounded.c -o test_unbounded.o
	gcc -g -Wall -c calc_unbounded_int.c -o calc_unbounded_int.o

	gcc -g -Wall test_unbounded.o unbounded_int.o -o test_unbounded
	gcc -g -Wall calc_unbounded_int.o unbounded_int.o -o calc_unbounded_int

tar:
	rm *.tar.gz
	tar -czvf C_STEVEN_THURAIN.tar.gz unbounded_int.c unbounded_int.h test_unbounded.c calc_unbounded_int.c README Makefile

clean:
	rm *.o
