#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include"unbounded_int.h"

/*
  Cherche la chaine de charactère égale à searched. Retourne sa position si une telle chaine existe, -1 sinon.
  Si plusieurs chaines correspondent à searched retourne la première occurence.

                                          "-i"    "-o"
  Exemples: ./calc_unbounded_int -i    ->   1      -1
            ./calc_unbounded_int -i -o ->   1       2
            ./calc_unbounded_int -o    ->  -1       1
*/
int search_args_options(int argc, char *argv[], char *searched) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], searched) == 0) {
      return i;
    }
  }
  return -1;
}

//struct contenant un mot, avec pointeurs vers les cellules suivant et précedent. Le type sera utilisé pour indiquer si c'est une variable/un entier/...
typedef struct cellString{
  char* mot;
  int type;
  struct cellString* precedent;
  struct cellString* suivant;
} cellString;

//struct contenant une expression, une liste de cellules/mots.
typedef struct expression{
  cellString* premier;
  cellString* dernier;
} exp;

int nullouvide_cellString(cellString* c){
  if(c==NULL || c->mot==NULL || strlen(c->mot)==0) return 1;
  return 0;
}

cellString* init_cellString(char* s,int t){
  if(s==NULL || strlen(s)==0) return NULL;
  cellString* res=malloc(sizeof(cellString));
  if(res==NULL){
    perror("init_cellString/malloc\n");
    return NULL;
  }
  res->mot=s;
  res->type=t;
  res->precedent=NULL;
  res->suivant=NULL;
  return res;
}

exp* init_exp(){
  exp* res=malloc(sizeof(exp));
  if(res==NULL){
    perror("init_exp/malloc\n");
    return NULL;
  }
  res->premier=NULL;
  res->dernier=NULL;
  return res;
}

/*
  Ajoute un cellString à une struct expression.
  Si fin>0, l'ajoute à la fin, sinon au début.
*/
void add_mot(exp* exp, cellString* c,int fin){
  if(exp==NULL) return;
  if(nullouvide_cellString(c)!=0) return;

  if(exp->premier==NULL){
    exp->premier=c;
    exp->dernier=c;
    return;
  }
  if(fin>0){
    (exp->dernier)->suivant=c;
    c->precedent=(exp->dernier);
    exp->dernier=c;
    return;
  }
  c->suivant=(exp->premier);
  (exp->premier)->precedent=c;
  exp->premier=c;
}

/*
  Vérifie si la chaîne de caractère est éligible d'être le nom d'une variable ou un entier.
  Retourne 2 si elle contient des lettres ET des chiffres, 1 si elle contient que des chiffres donc elle est un entier, 0 sinon.
*/
int est_variable_entier(char* v){
  if(v==NULL || strlen(v)==0) return 0;
  int var=2;
  for(int i=0;i<strlen(v);i++){
    if(isdigit(v[i])==0){
      if(isalpha(v[i])==0) return 0;
      else var=1;
    }
  }
  return var;
}

int est_operation(char* v){
  if(v==NULL || strlen(v)!=1) return 0;
  if(*v!='+' && *v!='-' && *v!='*') return 0;
  else return 1;
}

void parse_line(char* line) {
  printf("%s", line);
}

int main(int argc, char *argv[]) {
  /* code */
  int inputArgPos = search_args_options(argc, argv, "-i");
  int outputArgPos = search_args_options(argc, argv, "-o");
  // printf("%d\n", inputArgPos);
  // printf("%d\n", outputArgPos);

  assert(inputArgPos + 1 != outputArgPos);
  assert(outputArgPos + 1 != inputArgPos);

  assert(inputArgPos + 1 != argc);
  assert(outputArgPos + 1 != argc);

  FILE *inputFile;
  FILE *outputFile;

  if (inputArgPos != -1) {
    inputFile = fopen(argv[inputArgPos + 1], "r");
    if(inputFile == NULL) {
      printf("fichier %s non trouvé\n", argv[inputArgPos + 1]);
      return 1;
    }
  } else {
    inputFile = stdin;
  }

  if (outputArgPos != -1) {
    outputFile = fopen(argv[outputArgPos + 1], "w");
  } else {
    outputFile = stdout;
  }

  char line[1024];

  while (fgets(line, sizeof(line), inputFile)) {
    parse_line(line);
  }

  // char *listeMots[] = {"556", "lala", "lala55", "la_la55"};
  //
  // for (int i = 0; i < 4; i++) {
  //   printf("%s\n", listeMots[i]);
  //   printf("%d\n\n", est_variable_entier(listeMots[i]));
  // }
  //
  // printf("%d\n", est_operation("*"));

  return 0;
}
