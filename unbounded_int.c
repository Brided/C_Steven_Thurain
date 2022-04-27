#include"unbounded_int.h"
#include <ctype.h>

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
  else return 0;
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
  if (nbr->premier == NULL && nbr->dernier == NULL) {
    if (c != '0') {
      nbr->premier = ajout;
      nbr->dernier = ajout;
    }
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

  Exemple: "-0123"    -> 0123
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
  printf("Entier détecté: %s\n",intE);

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

char *unbounded_int2string(unbounded_int i) {
  char *res = malloc(i.len + 1);
  char *resPtr = res;

  *resPtr = i.signe;
  resPtr++;

  for(chiffre *e = i.premier; e != NULL; e = e->suivant, resPtr++) {
    *resPtr = e->c;
  }

  return res;
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
    printf("%c %c\n", chA->c, chB->c);
  }

  if (chA->c < chB->c) {
    return -1 * signe;
  } else if (chA->c > chB->c) {
    return 1 * signe;
  } else {
    return 0;
  }
}
