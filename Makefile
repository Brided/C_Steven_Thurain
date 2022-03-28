create:
	gcc -Wall -g -pedantic test_unbounded.c -o test_unbounded.o

clean:
	rm *.o
