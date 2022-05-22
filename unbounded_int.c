#include <ctype.h>

#include"unbounded_int.h"

static unbounded_int init_unb_int() {
  unbounded_int res;
  res.signe = '+';
  res.len = 0;
  res.premier = NULL;
  res.dernier = NULL;
  return res;
}

static chiffre *init_chiffre(char c) {
  chiffre *res;
  res = malloc(sizeof(chiffre));
  if(res==NULL){
    return NULL;
  }
  res->c = c;
  res->suivant = NULL;
  res->precedent = NULL;
  return res;
}

static int nullouvide(unbounded_int* ui){
  if(ui==NULL) return 1;
  if(ui->len==0) return 2;
  if(ui->premier==NULL || ui->dernier==NULL) return 3;
  if(ui->signe=='*') return 4;
  else return 0;
}

static int iszero(unbounded_int* ui){
  if (ui->len > 1) {
    return 0;
  }
  if (ui->premier->c != '0') {
    return 0;
  }
  return 1;
}

static void retirer_chiffre_debut(unbounded_int *nbr) {
  if (nbr->premier == NULL && nbr->dernier == NULL) {
    return;
  } else {
    chiffre *toFree = nbr->premier;
    nbr->premier = nbr->premier->suivant;
    nbr->premier->precedent = NULL;
    free(toFree);
    nbr->len--;
  }
}

static void retirer_zeros(unbounded_int *nbr) {
  while (nbr->premier->c == '0' && nbr->len > 1) {
    retirer_chiffre_debut(nbr);
  }
}

static void ajouter_chiffre_debut(unbounded_int *nbr, char c) {
  chiffre *ajout = init_chiffre(c);
  if(ajout==NULL){
    nbr->signe='*';
    return;
  }
  if (nbr->premier == NULL && nbr->dernier == NULL) {
    nbr->premier = ajout;
    nbr->dernier = ajout;
  } else {
    ajout->suivant = nbr->premier;
    nbr->premier->precedent = ajout;
    nbr->premier = ajout;
  }
  nbr->len++;
}

static void ajouter_chiffre_fin(unbounded_int *nbr, char c) {
  chiffre *ajout = init_chiffre(c);
  if(ajout==NULL){
    nbr->signe='*';
    return;
  }
  if (nbr->premier == NULL && nbr->dernier == NULL) {
    nbr->premier = ajout;
    nbr->dernier = ajout;
  } else {
    ajout->precedent = nbr->dernier;
    nbr->dernier->suivant = ajout;
    nbr->dernier = ajout;
  }
  nbr->len++;
}

/*
  Retourne NULL si le char* entré e contient des caractères autre que des chiffres (à l'exception de '+' et '-' pour indiquer une signe), ou sinon, retourne char* *(e+start) où start est le premier index d'un chiffre différent de zero.
  Si e contient que des zeros, retourne le dernier 0 de e.

  Exemple: "-0123"    -> -123
           "00014803" -> 14803
           "*786"     -> NULL
           "+A123"    -> NULL
           ""         -> NULL
           "0000000"  -> 0
*/
char* getInt(const char* e) {
  int start=-1;
  int len=strlen(e);
  if(len==0) return NULL;
  int i=0;
  if (*e != '+' && *e != '-') {
    if(isdigit(*e)==0) return NULL;
  }
  else{
    if(len>1) i++;
    else return NULL;
  }

  while(i<len){
    if(isdigit(*(e+i))==0){
      return NULL;
    }
    else{
      if(start==-1 && *(e+i)!='0'){
	start=i;
      }
    }
    i++;
  }

  char* res;
  if(start==-1) res=(char*)(e+(len-1));
  else res=(char*)(e+start);
  return res;
}

unbounded_int string2unbounded_int(const char *e) {
  unbounded_int res = init_unb_int();
  if(e==NULL || strlen(e)==0){
    printf("String entré NULL ou vide.\n");
    res.signe='*';
    return res;
  }
  char* intE=getInt(e);
  if(intE==NULL){
    printf("Entier pas trouvée: %s\n",e);
    res.signe='*';
    return res;
  }
  // printf("Entier détecté: %s\n",intE);

  int length=strlen(intE);
  int count = 0;

  if (*e == '+' || *e == '-') {
    res.signe = *e;
  }

  while (count < length) {
    ajouter_chiffre_fin(&res, *(intE + count));
    count++;
  }

  return res;
}

char* unbounded_int2string(unbounded_int i){
  if(nullouvide(&i)!=0) return NULL;
  int len=i.len;

  char* res=malloc(sizeof(char)*(len+1));
  if(res==NULL){
    perror("error in unbounded_int2string/malloc\n");
    return NULL;
  }

  res[0]=i.signe;
  int count=1;
  chiffre* iter=i.premier;
  while(iter!=NULL){
    res[count]=iter->c;
    iter=iter->suivant;
    count++;
  }

  if(len!=(count-1)) printf("Warning: unbounded_int2string length et nb de chiffres comptés sont différents: length:%d,count:%d\n",len,(count-1));

  return res;
}

unbounded_int ll2unbounded_int(long long i) {
  unbounded_int res = init_unb_int();
  int nb;

  if (i < 0) {
    res.signe = '-';
    i *= -1;
  }

  while (i > 0) {
    nb = i % 10;
    i /= 10;

    ajouter_chiffre_debut(&res, '0' + nb);
  }

  return res;
}

unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
  unbounded_int res=init_unb_int();
  if(nullouvide(&a)!=0 || nullouvide(&b)!=0) return res;
  char asigne=a.signe;
  char bsigne=b.signe;
  if(asigne!=bsigne) return unbounded_int_difference(a,b);
  res.signe=asigne;

  chiffre* aiter=a.dernier;
  chiffre* biter=b.dernier;
  int somme=0;
  int retenu=0;
  int achiffre=0;
  int bchiffre=0;

  while(aiter!=NULL && biter!=NULL){
    achiffre=aiter->c-'0';
    bchiffre=biter->c-'0';

    somme=achiffre+bchiffre+retenu;
    retenu=somme/10;
    ajouter_chiffre_debut(&res,(somme%10)+'0');
    aiter=aiter->precedent;
    biter=biter->precedent;
  }

  while(aiter!=NULL){
    achiffre=aiter->c-'0';
    somme=achiffre+retenu;
    retenu=somme/10;
    ajouter_chiffre_debut(&res,(somme%10)+'0');
    aiter=aiter->precedent;
  }

  while(biter!=NULL){
    bchiffre=biter->c-'0';
    somme=bchiffre+retenu;
    retenu=somme/10;
    ajouter_chiffre_debut(&res,(somme%10)+'0');
    biter=biter->precedent;
  }

  if(retenu!=0) ajouter_chiffre_debut(&res,retenu+'0');
  return res;
}


unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b){
  unbounded_int res=init_unb_int();
  if(nullouvide(&a)!=0 || nullouvide(&b)!=0) return res;
  char asigne=a.signe;
  char bsigne=b.signe;
  if(asigne!=bsigne){
    b.signe=a.signe;
    unbounded_int res=unbounded_int_somme(a,b);
    if(a.signe=='+') b.signe='-';
    else b.signe='+';
    return res;
  }
  if(a.len<b.len){
    unbounded_int res=unbounded_int_difference(b,a);
    if(res.signe=='+') res.signe='-';
    else res.signe='+';
    return res;
  }

  chiffre* aiter=a.dernier;
  chiffre* biter=b.dernier;
  int somme=0;
  int retenu=0;
  int achiffre=0;
  int bchiffre=0;
  chiffre* firstNonZero=NULL;

  while(aiter!=NULL && biter!=NULL){
    achiffre=aiter->c-'0';
    bchiffre=biter->c-'0';
    //printf("%d,%d : ",achiffre,bchiffre);
    somme=achiffre-bchiffre-retenu;
    //printf("somme=%d,",somme);
    if(somme<0){
      somme=10+achiffre-(bchiffre+retenu);
      retenu+=1;
    }
    else retenu=0;
    //printf("ret=%d\n",retenu);
    ajouter_chiffre_debut(&res,(somme%10)+'0');
    if(somme%10!=0) firstNonZero=res.premier;
    aiter=aiter->precedent;
    biter=biter->precedent;
  }

  while(aiter!=NULL){
    achiffre=aiter->c-'0';
    //printf("a %d: ",achiffre);
    somme=achiffre-retenu;
    //printf("somme=%d,",somme);
    if(somme<0){
      somme=10+achiffre-retenu;
      retenu+=1;
    }
    else retenu=0;
    //printf("ret=%d\n",retenu);
    ajouter_chiffre_debut(&res,(somme%10)+'0');
    if(somme%10!=0) firstNonZero=res.premier;
    aiter=aiter->precedent;
  }

  if(retenu>0){
    ajouter_chiffre_debut(&res,retenu+'0');
    if(bsigne=='+') res.signe='-';
    else res.signe='+';
  }
  else res.signe=asigne;

  if(firstNonZero==NULL){
    res.premier=res.dernier;
    (res.dernier)->precedent=NULL;
  }
  else{
    res.premier=firstNonZero;
    (res.premier)->precedent=NULL;
  }
  return res;
}

unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
  if (iszero(&a) || iszero(&b)) {
    return string2unbounded_int("0");
  }

  unbounded_int c = init_unb_int();
  int r;
  chiffre *chA;
  chiffre *chB;
  chiffre *chC;
  chiffre *chCAB;

  ajouter_chiffre_debut(&c, '0');

  for (chB = b.dernier, chC = c.premier; chB != NULL; chB = chB->precedent, chC = chC->precedent) {
    r = 0;
    if (chB->c == '0') {
      continue;
    }
    chCAB = chC;
    for (chA = a.dernier; chA != NULL; chA = chA->precedent) {
      int v = (chCAB->c - '0') + (chA->c - '0') * (chB->c - '0') + r;
      chCAB->c = (v % 10) + '0';
      r = v / 10;
      if (chCAB->precedent == NULL) {
        ajouter_chiffre_debut(&c, '0');
      }
      chCAB = chCAB->precedent;
    }
    chCAB->c = r + '0';
  }
  retirer_zeros(&c);
  if (a.signe != b.signe) {
    c.signe = '-';
  }
  return c;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b) {
  // On peut comparer un char avec un char.

  if (a.signe != b.signe) {
    if(a.signe == '-') {
      return -1;
    } else {
      return 1;
    }
  }

  // Si le signe est négatif les retours sont opposés aux retours dans le cas du signe positifs.
  int signe = (a.signe == '-')?-1:1;

  if (a.len != b.len) {
    if (a.len < b.len) {
      return -1 * signe;
    } else {
      return 1 * signe;
    }
  }

  chiffre *chA = a.premier;
  chiffre *chB = b.premier;

  while (chA->c == chB->c && chA->suivant != NULL && chB->suivant != NULL) {
    chA = chA->suivant;
    chB = chB->suivant;
  }

  if (chA->c < chB->c) {
    return -1 * signe;
  } else if (chA->c > chB->c) {
    return 1 * signe;
  } else {
    return 0;
  }
}

int unbounded_int_cmp_ll(unbounded_int a, long long b) {
  // ...

  char bSigne = (b < 0)?'-':'+';
  bSigne = (b == 0)?'0':bSigne;

  if (a.signe != bSigne) {
    if (a.signe == '-') {
      return -1;
    } else if(bSigne == '0' && iszero(&a)) {
      return 0;
    } else {
      return 1;
    }
  }

  // Si le signe est négatif les retours sont opposés aux retours dans le cas du signe positifs.
  int signe = (bSigne == '-')?-1:1;
  if (signe == -1) {
    b *= -1;
  }

  char aChDiff = '*';
  char bChDiff = '*';
  chiffre *chA = a.dernier;

  int nb;

  while (b > 0 && chA != NULL) {
    nb = b % 10;
    b = b / 10;

    if ('0' + nb != chA->c) {
      // printf("%c %c différent\n", '0' + nb, chA->c);
      aChDiff = chA->c;
      bChDiff = '0' + nb;
    }

    chA = chA->precedent;
  }

  int finA = chA == NULL;
  int finB = b == 0;

  // printf("%s\n", (finA)?"A est à la fin":"A n'est pas à la fin");
  // printf("%s\n", (finB)?"B est à la fin":"B n'est pas à la fin");
  // printf("%c %c premiers différents\n", aChDiff, bChDiff);

  if (finA && !finB) {
    return -1 * signe;
  } else if (!finA && finB) {
    return 1 * signe;
  }

  // si ces valeurs seraient égaux ils seraient alors tout les deux '*'
  if (aChDiff != bChDiff) {
    if (aChDiff < bChDiff) {
      return -1 * signe;
    } else {
      return 1 * signe;
    }
  }

  return 0;
}
