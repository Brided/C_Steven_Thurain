#include<stdio.h>

#include"unbounded_int.h"

void afficher_unb_int(unbounded_int aff) {
  printf("longueur: %ld\n", aff.len);
  putchar(aff.signe);
  for (chiffre *e = aff.premier; e != NULL; e = e->suivant) {
    putchar(e->c);
  }
  printf("\n");
}

int main(void) {
  unbounded_int un = string2unbounded_int("+1234");
  afficher_unb_int(un);

  unbounded_int deux = string2unbounded_int("-123");
  afficher_unb_int(deux);

  unbounded_int trois = ll2unbounded_int(-11111222223LL);
  afficher_unb_int(trois);
  return 0;
}
