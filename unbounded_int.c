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
  res->c = c;
  res->suivant = NULL;
  res->precedent = NULL;
  return res;
}

static void ajouter_chiffre_debut(unbounded_int *nbr, char c) {
  chiffre *ajout = init_chiffre(c);
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
    nbr->premier = ajout;
    nbr->dernier = ajout;
  } else {
    ajout->precedent = nbr->dernier;
    nbr->dernier->suivant = ajout;
    nbr->dernier = ajout;
  }
  nbr->len++;
}

unbounded_int string2unbounded_int(const char *e) {
  int length = strlen(e);
  int count = 0;
  unbounded_int res = init_unb_int();

  if (*e == '+' || *e == '-') {
    res.signe = *e;
    count++;
  }

  while (count < length) {
    ajouter_chiffre_fin(&res, *(e + count));
    count++;
  }

  return res;
}

unbounded_int ll2unbounded_int(long long i) {
  unbounded_int res = init_unb_int();
  int nb;

  if (i < 0) {
    res.signe = '-';
    i *= -1;
  }

  while (i > 10) {
    nb = i % 10;
    i /= 10;

    ajouter_chiffre_debut(&res, '0' + nb);
  }

  return res;
}