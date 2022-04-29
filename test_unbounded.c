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
  printf("\n");
}

void testInt2STR(char* e){
  printf("Test int2str(str2int()) de \"%s\"\n",e);
  printf("Resultat obtenu: \"%s\"\n\n",unbounded_int2string(string2unbounded_int(e)));
}

void testSomme(char* e1, char* e2){
  printf("Test somme entre %s et %s\n",e1,e2);
  unbounded_int nb1 = string2unbounded_int(e1);
  unbounded_int nb2 = string2unbounded_int(e2);
  if(nb1.signe=='*'){
    printf("Problème avec \"%s\", arrêt du somme.\n",e1);
    return;
  }
  if(nb2.signe=='*'){
    printf("Problème avec \"%s\", arrêt du somme.\n",e2);
    return;
  }

  unbounded_int res=unbounded_int_somme(nb1,nb2);
  if(res.len==0){
    printf("Entrée int null ou vide détecté, arrêt du somme.\n");
    return;
  }
  printf("Resultat: \n");
  afficher_unb_int(res);
  printf("\n");
}

void testDiff(char* e1, char* e2){
  printf("Test différence %s-(%s)\n",e1,e2);
  unbounded_int nb1 = string2unbounded_int(e1);
  unbounded_int nb2 = string2unbounded_int(e2);
  if(nb1.signe=='*'){
    printf("Problème avec \"%s\", arrêt du soustrction.\n",e1);
    return;
  }
  if(nb2.signe=='*'){
    printf("Problème avec \"%s\", arrêt du soustraction.\n",e2);
    return;
  }

  unbounded_int res=unbounded_int_difference(nb1,nb2);
  if(res.len==0){
    printf("Entrée int null ou vide détecté, arrêt du soustraction.\n");
    return;
  }
  printf("Resultat: \n");
  afficher_unb_int(res);
  printf("\n");
}

void testCMP(char* e1, char* e2){
  printf("Test comparaison %s==%s ?\n",e1,e2);
  unbounded_int nb1 = string2unbounded_int(e1);
  unbounded_int nb2 = string2unbounded_int(e2);
  if(nb1.signe=='*'){
    printf("Problème avec \"%s\", arrêt du comparaison.\n",e1);
    return;
  }
  if(nb2.signe=='*'){
    printf("Problème avec \"%s\", arrêt du comparaison.\n",e2);
    return;
  }

  int res=unbounded_int_cmp_unbounded_int(nb1,nb2);
  printf("Resultat: %d\n",res);
  printf("\n");
}

int main(void) {
  unbounded_int trois = ll2unbounded_int(-11111222223LL);
  afficher_unb_int(trois);

  testSTR2Int("+1234");
  testSTR2Int("-123");
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

  testSomme("+1","+10");
  testSomme("+100","+10");
  testSomme("+10","+100");
  testSomme("+1","+1");
  testSomme("+1","+9999");
  testSomme("+9999","+1");

  testDiff("+1","+10");
  testDiff("+100","+10");
  testDiff("+10","+100");
  testDiff("-1","-10");
  testDiff("-100","-10");
  testDiff("-10","-100");
  testDiff("+1","+1");
  testDiff("+1","+9999");
  testDiff("+9999","+1");

  testDiff("+1","-10");
  testDiff("+100","-10");
  testDiff("-1","+10");
  testDiff("-100","+10");

  testCMP("-100","-100");
  testCMP("+100","-100");
  testCMP("-1000","-100");
  testCMP("-10001","-10002");

  return 0;
}
