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


// (1)
unbounded_int string2unbounded_int(const char *e);

// (2)
unbounded_int ll2unbounded_int(long long i);

// (3)
char *unbounded_int2string(unbounded_int i);

// (4)
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

// (5)
int unbounded_int_cmp_ll();

// (6)
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

// (7)
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

// (8)
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);