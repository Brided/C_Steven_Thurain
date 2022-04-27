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

void testSTR2Int(char* e){
  printf("\nString: %s\n",e);
  unbounded_int res = string2unbounded_int(e);
  afficher_unb_int(res);
}

void testInt2STR(char* e){
  printf("Test int2str(str2int()) de \"%s\"\n",e);
  printf("Resultat obtenu: \"%s\"\n",unbounded_int2string(string2unbounded_int(e)));
}

int main(void) {
  testSTR2Int("+1234");

  testSTR2Int("-123");

  unbounded_int trois = ll2unbounded_int(-11111222223LL);
  afficher_unb_int(trois);

  testSTR2Int("-");

  testSTR2Int("00000");

  testSTR2Int("-0001845d");

  testSTR2Int("");

  testSTR2Int(NULL);

  testSTR2Int("*054");

  testSTR2Int("0001");

  testSTR2Int("1000");

  testInt2STR("-4543676543298");
  testInt2STR(NULL);
  testInt2STR("");
  testInt2STR("+103");
  testInt2STR("-000");
  return 0;
}
