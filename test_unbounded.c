#include<stdio.h>

#include"unbounded_int.h"

void afficher_unb_int(unbounded_int aff) {
  if(aff.signe=='*'){
    printf("affichage impossible\n");
    return;
  }
  printf("longueur: %ld  ", aff.len);
  // printf("signe: %c  ", aff.signe);
  printf("valeur: ");
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

// fonctions pour tester les fonctions somme, difference et produit
// indique si resultat erroné
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
  char *remaining;
  long long ll1 = strtol(e1, &remaining, 10);
  long long ll2 = strtol(e2, &remaining, 10);

  unbounded_int res=unbounded_int_somme(nb1,nb2);
  long long llres=ll1+ll2;
  int isCorrect = !unbounded_int_cmp_ll(res, llres);
  if(res.len==0){
    printf("Entrée int null ou vide détecté, arrêt du somme.\n");
    return;
  }
  printf("Resultat unbounded_int: \n");
  afficher_unb_int(res);
  if (!isCorrect) {
    fprintf(stderr, "Incorrect, attendu %lld\n", llres);
  }
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
  char *remaining;
  long long ll1 = strtol(e1, &remaining, 10);
  long long ll2 = strtol(e2, &remaining, 10);

  unbounded_int res=unbounded_int_difference(nb1,nb2);
  long long llres=ll1-ll2;
  int isCorrect = !unbounded_int_cmp_ll(res, llres);
  if(res.len==0){
    printf("Entrée int null ou vide détecté, arrêt du soustraction.\n");
    return;
  }
  printf("Resultat: \n");
  afficher_unb_int(res);
  if (!isCorrect) {
    printf("Incorrect, attendu %lld\n", llres);
  }
  printf("\n");
}

void testProd(char* e1, char* e2) {
  printf("Test produit %s*%s\n",e1,e2);
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
  char *remaining;
  long long ll1 = strtol(e1, &remaining, 10);
  long long ll2 = strtol(e2, &remaining, 10);

  unbounded_int res=unbounded_int_produit(nb1,nb2);
  long long llres=ll1*ll2;
  int isCorrect = !unbounded_int_cmp_ll(res, llres);
  if(res.len==0){
    printf("Entrée int null ou vide détecté, arrêt du produit.\n");
    return;
  }
  printf("Resultat: \n");
  afficher_unb_int(res);
  if (!isCorrect) {
    printf("Incorrect, attendu %lld\n", llres);
  }
  printf("\n");
}

void testCmpIntInt(char* e1, char* e2){
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

void testCmpIntLlong(char* e1, long long ll2) {
  printf("Test comparaison %s==%lld ?\n",e1,ll2);
  unbounded_int nb1 = string2unbounded_int(e1);
  if(nb1.signe=='*'){
    printf("Problème avec \"%s\", arrêt du comparaison.\n",e1);
    return;
  }

  int res=unbounded_int_cmp_ll(nb1,ll2);
  printf("Resultat: %d\n",res);
  printf("\n");
}

long long testNb(long long inf, long long sup, int somme, int diff, int cmpunb, int cmpl, int mult){
  if(somme==0 && diff==0 && cmpunb==0 && cmpl==0 && mult==0){
    printf("Veuillez choisir ce que vous vouliez tester en mettant un valeur != 0 :\nsomme pour sommes,\ndiff pour difference,\ncmpunb pour comparaison  entre unbounded_int et unbounded_int,\ncmpl pour comparaison entre unbounded_int et long long,\nmult pour multiplication.\n");
    return 0;
  }

  long long err=0;
  int res=0;
  long long resl=0;

  unbounded_int ui;
  unbounded_int uj;
  unbounded_int s;
  unbounded_int d;
  unbounded_int m;
  
  for(long long i=inf;i<=sup;i++){
    for(long long j=i;j<=sup;j++){
      ui=ll2unbounded_int(i);
      uj=ll2unbounded_int(j);
      
      if(ui.signe=='*'){
	printf("Problème avec %lld, arrêt du comparaison.\n",i);
	err++;
	continue;
      }
      if(uj.signe=='*'){
	printf("Problème avec %lld, arrêt du comparaison.\n",j);
	err++;
	continue;
      }

      /*
      printf("//////");
      printf("ui,uj:\n");
      afficher_unb_int(ui);
      afficher_unb_int(uj);
      printf("\n");
      */
      
      if(cmpunb!=0 || cmpl!=0){
	res=0;
	if(i<j) res=-1;
	else if(i>j) res=1;
	
	if(cmpunb!=0 && unbounded_int_cmp_unbounded_int(ui,uj)!=res){
	  printf("Comparaison unb/unb faux : %lld,%lld",i,j);
	  err++;
	}

	if(cmpl!=0 && unbounded_int_cmp_ll(ui,j)!=res){
	  printf("Comparaison unb/ll faux : %lld,%lld",i,j);
	  err++;
	}
      }
      
      //printf("SOMME\n");
      if(somme!=0){
	resl=i+j;
	s=unbounded_int_somme(ui,uj);
	
	if(s.signe=='*'){
	  printf("Problème avec somme(%lld,%lld) arrêt du comparaison.\n",i,j);
	  err++;
	  continue;
	}
	if(unbounded_int_cmp_ll(s,resl)!=0){
	  /*
	  printf("ui,uj:\n");	  
	  afficher_unb_int(ui);
	  afficher_unb_int(uj);
	  */
	  printf("Somme faux : %lld,%lld attendu:%lld,res:\n",i,j,resl);	  
	  afficher_unb_int(s);
	  err++;
	}
      }

      //printf("DIFF\n");
      if(diff!=0){
	resl=i-j;
	d=unbounded_int_difference(ui,uj);
	if(d.signe=='*'){
	  printf("Problème avec diff(%lld,%lld) arrêt du comparaison.\n",i,j);
	  err++;
	  continue;
	}
	if(unbounded_int_cmp_ll(d,resl)!=0){
	  printf("Difference faux %d: %lld,%lld attendu:%lld,res:\n",unbounded_int_cmp_ll(d,resl)!=0,i,j,resl);
	  afficher_unb_int(d);
	  err++;
	}
      }
      
      //printf("MULT\n");
      if(mult!=0){
	resl=i*j;

	/*
	printf("ui,uj:\n");	  
	afficher_unb_int(ui);
	afficher_unb_int(uj);
	*/
	  
	m=unbounded_int_produit(ui,uj);
	if(m.signe=='*'){
	  printf("Problème avec mult(%lld,%lld) arrêt du comparaison.\n",i,j);
	  err++;
	  continue;
	}
	if(unbounded_int_cmp_ll(m,resl)!=0){
	  printf("Produit faux : %lld,%lld attendu:%lld,res:\n",i,j,resl);
	  afficher_unb_int(m);
	  err++;
	}
      }

      /*
      printf("\\\\\\");
      printf("\nerr:%lld\n\n",err);
      */
    }
  }
  return err;
}

int main(void) {
  long long inf=-100;
  long long sup=100;
  printf("Tests de %lld a %lld:\n",inf,sup);
  printf("Nb d'erreurs:%lld\n",testNb(inf,sup,1,1,1,1,1));

  /*
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
  testSomme("+100","-10");
  testSomme("+10","+100");
  testSomme("+1","+1");
  testSomme("+1","+9999");
  testSomme("+9999","+1");

  testDiff("+1","+10");
  testDiff("+100","+10");
  testDiff("+100","-10");
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

  testCmpIntInt("-100","-100");
  testCmpIntInt("+100","-100");
  testCmpIntInt("-1000","-100");
  testCmpIntInt("-10001","-10002");
  testCmpIntInt("-10001","0");
  testCmpIntInt("0","-10001");

  testCmpIntLlong("4444", 4444ll);
  testCmpIntLlong("-1461114", -1461114ll);
  testCmpIntLlong("4444", 4424ll);
  testCmpIntLlong("4443444", 44244ll);
  testCmpIntLlong("-4444", -44424ll);
  testCmpIntLlong("123", 124ll);
  testCmpIntLlong("-323", -183ll);
  testCmpIntLlong("0", 0);
  testCmpIntLlong("123", 0);
  testCmpIntLlong("-123", 0);

  testProd("20", "201");
  testProd("333", "2");
  testProd("456", "51566");
  testProd("4004", "2002");
  testProd("1", "0");
  testProd("-1", "0");

  */
  return 0;
}
