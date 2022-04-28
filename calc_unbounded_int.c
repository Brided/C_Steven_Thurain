#include<stdio.h>
#include<assert.h>
#include"unbounded_int.h"

/*
  Cherche la chaine de charactère égale à searched. Retourne sa position si une telle chaine existe, -1 sinon.
  Si plusieurs chaines correspondent à searched retourne la première occurence.

                                          "-i"    "-o"
  Exemples: ./calc_unbounded_int -i    ->   1      -1
            ./calc_unbounded_int -i -o ->   1       2
            ./calc_unbounded_int -o    ->  -1       1
*/
int search_args_options(int argc, char *argv[], char *searched) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], searched) == 0) {
      return i;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  /* code */
  int inputArgPos = search_args_options(argc, argv, "-i");
  int outputArgPos = search_args_options(argc, argv, "-o");
  printf("%d\n", inputArgPos);
  printf("%d\n", outputArgPos);
  return 0;
}