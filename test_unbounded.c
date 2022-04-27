#include<stdio.h>

#include"unbounded_int.h"

void afficher_unb_int(unbounded_int aff) {
  printf("longueur: %ld\n", aff.len);
  printf("signe: %c\n", aff.signe);
  putchar(aff.signe);
  for (chiffre *e = aff.premier; e != NULL; e = e->suivant) {
    putchar(e->c);
  }
  printf("\n");
}


int main(void) {
  unbounded_int un = string2unbounded_int("+1234");
  afficher_unb_int(un);

  unbounded_int deux = string2unbounded_int("+123");
  afficher_unb_int(deux);

  unbounded_int trois = ll2unbounded_int(-11111222223LL);
  afficher_unb_int(trois);

  unbounded_int quatre = string2unbounded_int("-00011111222222");
  afficher_unb_int(quatre);

  printf("\n");

  char *cinq = unbounded_int2string(un);
  printf("%s\n", cinq);

  printf("%d\n\n", unbounded_int_cmp_unbounded_int(un, deux));
  printf("%d\n", unbounded_int_cmp_unbounded_int(trois, quatre));
  return 0;
}
