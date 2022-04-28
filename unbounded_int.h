#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct chiffre{
  struct chiffre *suivant;
  char c;
  struct chiffre *precedent;
} chiffre;

typedef struct{
  char signe;
  size_t len;
  chiffre *premier;
  chiffre *dernier;
} unbounded_int;

unbounded_int string2unbounded_int(const char *e);

unbounded_int ll2unbounded_int(long long i);

char *unbounded_int2string(unbounded_int i);

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);


