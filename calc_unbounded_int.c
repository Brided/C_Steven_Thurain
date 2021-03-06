#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include"unbounded_int.h"

#define NB_MAX_MOTS 8
#define NB_CHAR_LINE 1024
#define NB_MAX_VARIABLES 1024
#define PILE_RESIZE_TAUX 2/3

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

exp* vider_exp(exp* freed) {
  cellString *cs = freed->premier;
  cellString *suivant;
  while (cs != NULL) {
    suivant = cs->suivant;
    cs->suivant = NULL;
    cs->precedent = NULL;
    free(cs);
    cs = suivant;
  }
  //free(freed);
  //freed = NULL;
  freed->premier=NULL;
  freed->dernier=NULL;
  return freed;
}

void print_exp(exp* exp){
  if(exp==NULL) printf("exp null\n");
  else printf("exp:\n");
  cellString* iter=exp->premier;
  while(iter!=NULL){
    printf("%s,%d  ",iter->mot,iter->type);
    iter=iter->suivant;
  }
  printf("\n");
}

//struct contenant un nom de variable et une valeur associée au nom de variable.
typedef struct varStore{
  char* nomVar;
  unbounded_int valeur;
} varStore;

typedef struct pile {
  int capacite;
  int len;
  varStore **variables;
} pile;

typedef struct charPile{
  int capacite;
  int len;
  char** strings;
} charPile;

charPile init_charPile(){
  charPile cp;
  cp.capacite=NB_MAX_MOTS;
  cp.len=0;
  cp.strings=malloc(sizeof(char*)*cp.capacite);
  if(cp.strings==NULL) perror("init_charPile:malloc");
  return cp;
}

charPile* resize_charPile(charPile* cp, int newSize){
  cp->capacite=newSize;
  cp->strings=realloc(cp->strings,sizeof(char*)*newSize);
  return cp;
}

void vider_charPile(charPile* cp){
  if(cp==NULL || cp->strings==NULL) return;
  for(int i=0;i<cp->len;i++){
    free(cp->strings[i]);
  }
  free(cp->strings);
  cp->len=0;
}

void add_charPile(charPile* cp, char* mot, int len){
  if(cp==NULL || cp->strings==NULL || mot==NULL || strcmp(mot,"")==0) return;
  int cplen=cp->len;
  if( ((cplen+1)/(cp->capacite))>PILE_RESIZE_TAUX) resize_charPile(cp,cp->capacite*2);
  (cp->strings)[cplen]=malloc(len);
  memmove((cp->strings)[cplen],mot,len);
  (cp->len)=(cp->len)+1;
}

char* get_charPile(charPile* cp,int i){
  if(cp==NULL || cp->strings==NULL || i>=cp->len) return NULL;
  return (cp->strings)[i];
}

pile init_pile() {
  pile pile;
  pile.capacite = NB_MAX_VARIABLES;
  pile.len = 0;
  pile.variables = malloc(sizeof(varStore*) * pile.capacite);
  if(pile.variables==NULL) perror("init_pile:malloc");
  return pile;
}

pile *resize_pile(pile *pile, int newSize) {
  pile->capacite = newSize;
  pile->variables = realloc(pile->variables, sizeof(varStore*) * pile->capacite);
  return pile;
}

void free_varStore(varStore* v){
  if(v==NULL) return;
  if(v->nomVar!=NULL) free(v->nomVar);
  free(v);
}

void free_pile(pile* p){
  if(p==NULL) return;
  for(int i=0;i<p->len;i++){
    //printf("freep(%d)",i);
    free_varStore(p->variables[i]);
  }
}

varStore *init_varStore(char *nomVar, char *string) {
  varStore *res = malloc(sizeof(varStore));
  res->nomVar = nomVar;
  res->valeur = string2unbounded_int(string);
  return res;
}

//cherche le varStore de nom string dans la pile p et le retourne si il existe,
//sinon ajoute un varStore de valeur 0 à la pile et le retourne.
varStore *search_var(pile *p, char *string) {
  if (p->len == 0) {
    varStore *res = init_varStore(string, "0");
    p->variables[0] = res;
    p->len++;
    return res;
  }
  if (p->len > p->capacite * PILE_RESIZE_TAUX) {
    resize_pile(p, p->capacite * 2);
  }
  for (int i = 0; i<p->len; i++) {
    if (strcmp(p->variables[i]->nomVar, string) == 0) {
      return (p->variables[i]);
    }
  }
  varStore *res = init_varStore(string, "0");
  p->variables[p->len] = res;
  p->len++;
  return res;
}

//cherche le varStore de nom string dans la pile p
//met sa valeur à newInt et le retourne
varStore *edit_var(pile *p, char *string, unbounded_int newInt) {
  varStore *res = search_var(p, string);
  res->valeur = newInt;
  return res;
}

unbounded_int getVarInt(pile *p, cellString *cell){
  if (cell->type == 2) {
    varStore *searched = search_var(p, cell->mot);
    return searched->valeur;
  } else if (cell->type == 1) {
    return string2unbounded_int(cell->mot);
  }
  return string2unbounded_int("");
}

//effectue une opération de type op sur v1 et v2 et retourne le résultat
unbounded_int var_operation(pile *p, cellString *cell1, char op, cellString *cell2) {
  unbounded_int var1 = getVarInt(p, cell1);
  unbounded_int var2 = getVarInt(p, cell2);
  unbounded_int intRes;
  switch (op) {
    case '+':
      intRes = unbounded_int_somme(var1, var2);
      return intRes;

    case '-':
      intRes = unbounded_int_difference(var1, var2);
      return intRes;

    case '*':
      intRes = unbounded_int_produit(var1, var2);
      return intRes;

    default:
      printf("opération non reconnue\n");
      return intRes;
  }
}

void afficher_var(varStore *vs, FILE *outputFile) {
  fprintf(outputFile, "%s = ", vs->nomVar);
  if (vs->valeur.len == 1 && vs->valeur.premier->c == '0') {
    fprintf(outputFile, "0\n");
    return;
  }
  if (vs->valeur.signe == '-') {
    fputc(vs->valeur.signe, outputFile);
  }
  for (chiffre *e = vs->valeur.premier; e != NULL; e = e->suivant) {
    fputc(e->c, outputFile);
  }
  fprintf(outputFile, "\n");
}

//affiche la variable de nom printed dans la pile
void afficher_pile_var(pile *p, char *printed, FILE *outputFile) {
  varStore *vs = search_var(p, printed);
  afficher_var(vs, outputFile);
}

int nullouvide_cellString(cellString* c){
  if(c==NULL || c->mot==NULL || strlen(c->mot)==0) return 1;
  return 0;
}

int est_operation(char* v){
  if(v==NULL || strlen(v)!=1) return 0;
  if(*v=='+' || *v=='-' || *v=='*') return 1;
  if(*v=='=') return 2;
  else return 0;
}

/*
  Vérifie si la chaîne de caractère est éligible d'être le nom d'une variable ou un entier.
  Retourne:
  - 5 si "print"
  - 4 si '='
  - 3 si c'est une opération (+,-,*),
  - 2 si elle contient des lettres et potentiellement des chiffres,
  - 1 si elle contient que des chiffres donc elle est un entier,
  - 0 pour tout autre.
*/
int est_variable_entier(char* v){
  if(v==NULL || strlen(v)==0) return 0;
  if(!strcmp(v,"print")) return 5;
  int op=est_operation(v);
  if(op==1) return 3;
  else if(op==2) return 4;
  int var=1;
  for(int i=0;i<strlen(v);i++){
    if(isdigit(v[i])==0){
      if ((v[i]=='-' || v[i]=='+') && i==0){
      } else if(isalpha(v[i])==0){
        return 0;
      }
      else var=2;
    }
  }
  return var;
}

void copyMot(const char* src, char* dest, int dep, int fin){
  int l=0;
  if(dep>fin || strlen(src)<=fin) return;
  for(int i=dep;i<=fin;i++){
    //printf("%c",src[i]);
    dest[l]=src[i];
    l++;
  }
  dest[fin+1]='\0';
  //printf("\n%s\n",dest);
}

/*
  Ajoute un cellString à une struct expression.
  Si fin>0, l'ajoute à la fin, sinon au début.
*/
void add_mot(exp* exp, char* mot,int fin){
  if(exp==NULL) return;
  cellString* c=init_cellString(mot,est_variable_entier(mot));
  if(c==NULL) return;
  if(nullouvide_cellString(c)!=0) return;

  if(exp->premier==NULL){
    exp->premier=c;
    exp->dernier=c;
    return;
  }
  if(fin>0){
    c->precedent=(exp->dernier);
    (exp->dernier)->suivant=c;
    exp->dernier=c;
    return;
  }
  c->suivant=(exp->premier);
  (exp->premier)->precedent=c;
  exp->premier=c;
}

int parse_line(char* readLine, charPile* putLine) {
  int len=strlen(readLine);
  int l=0;
  int nbMots=0;
  char* mot;
  
  for(int i=0;i<len-1;i++){
    char c=readLine[i];
    char d=readLine[i+1];

    if(isblank(c)){
      if(l>0){
      	mot=malloc(sizeof(char)*(l+1));
      	if(mot==NULL){
      	  perror("parse_line/malloc\n");
      	  return -1;
      	}
	copyMot(readLine,mot,i-l,i-1);
	//printf("deb %s\n",mot);
      	add_charPile(putLine,mot,l+1);
      	nbMots++;
      	l=0;
	mot=NULL;
      }
    }

    else{
      //printf("c : %c,%d :\n",c,l);
      if(c=='=' || (i!=len-1 && c=='-' && est_operation(&d) != 0) || est_operation(&c)!=0){
      	if(l>0){
      	  mot=malloc(sizeof(char)*(l+1));
      	  if(mot==NULL){
      	    perror("parse_line/malloc\n");
      	    return -1;
      	  }
	  copyMot(readLine,mot,i-l,i-1);
	  //printf("mid1 %s\n",mot);
	  add_charPile(putLine,mot,l+1);
      	  nbMots++;
      	  l=0;
	  mot=NULL;
      	}
      	mot=malloc(sizeof(char)*2);
      	if(mot==NULL){
      	  perror("parse_line/malloc\n");
      	  return -1;
      	}
        mot[0]=c;
	mot[1]='\0';
	//printf("mid2 %s\n",mot);
	add_charPile(putLine,mot,2);
      	nbMots++;
	mot=NULL;
      }
      else l++;
    }
  }

  if(l>0){
    mot=malloc(sizeof(char)*(l+1));
    if(mot==NULL){
      perror("parse_line/malloc\n");
      return -1;
    }
    //printf("len %d :\n",l);
    copyMot(readLine,mot,len-l-1,len-2);
    mot[l]='\0';
    //printf("fin %s\n",mot);    
    add_charPile(putLine,mot,l+1);    
    nbMots++;
  }  
  return nbMots;
}

unbounded_int parse_calcul(pile *p, cellString *calcul1) {
  if (calcul1 != NULL) {
    if (calcul1->suivant == NULL) {
      if (calcul1->type == 1 || calcul1->type == 2) {
        unbounded_int res = getVarInt(p, calcul1);
        return res;
      }
    } else if (calcul1->suivant->type == 3) {
      cellString *calcul2;
      if ((calcul2 = calcul1->suivant->suivant) != NULL) {
        if (calcul2->type == 1 || calcul2->type == 2){
          char op = calcul1->suivant->mot[0];
          unbounded_int res = var_operation(p, calcul1, op, calcul2);
          return res;
        }
      }
    }
  }
  return string2unbounded_int("*");
}

void exec_exp(pile *p, exp* exp, FILE *outputFile) {
  cellString *premier = exp->premier;
  if (premier == NULL) {
    return;
  }
  cellString *second = premier->suivant;
  if (second == NULL) {
    print_exp(exp);
    fprintf(stderr, "expression trop courte\n");
    return;
  }
  // printf("%s\n", premier->mot);

  if (premier->type == 5) {
    if (second->type == 2) {
      afficher_pile_var(p, second->mot, outputFile);
    } else {
      print_exp(exp);
      fprintf(stderr, "argument print incorrect\n");
    }
    return;
  }

  else if (premier->type == 2 && second->type == 4) {
    cellString *trois = second->suivant;
    unbounded_int resCalc = parse_calcul(p, trois);

    if (resCalc.signe == '*') {
      print_exp(exp);
      fprintf(stderr, "instruction érronée (n'oubliez pas d'espacer les variables et opérations.)\n");
      return;
    }

    edit_var(p, premier->mot, resCalc);
    return;
  }

  print_exp(exp);
  fprintf(stderr, "instruction érronée (n'oubliez pas d'espacer les variables et opérations.)\n");
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

  char* EXIT_PROMPTS[]={"EXIT","exit","QUIT","quit"};
  int exit_len=4;

  if (inputArgPos != -1) {
    inputFile = fopen(argv[inputArgPos + 1], "r");
    if(inputFile == NULL) {
      fprintf(stderr, "fichier %s non trouvé\n", argv[inputArgPos + 1]);
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

  pile memoire = init_pile();
  char line[NB_CHAR_LINE];
  exp* expressions=init_exp();
  int nbMots;
  int e=1;
  

  while (fgets(line, sizeof(line), inputFile)) {
    charPile mots=init_charPile();
    nbMots=parse_line(line,&mots);
    printf("Ligne %d,len=%d :\n",e,nbMots);
    if(nbMots==-1){
      return 1;
    }
    if(nbMots==1){
      for(int i=0;i<exit_len;i++){
	char* mot0=get_charPile(&mots,0);
	if(strcmp(mot0,EXIT_PROMPTS[i])==0){
	  free_pile(&memoire);
	  fclose(inputFile);
	  fclose(outputFile);
	  return 0;
	}
      }
    }
    expressions=init_exp();
    for(int i=0;i<nbMots && i<NB_MAX_MOTS;i++){
      //printf("mot : %s\n",get_charPile(&mots,i));
      add_mot(expressions,get_charPile(&mots,i),1);
    }    
    //print_exp(expressions[e]);
    exec_exp(&memoire, expressions, outputFile);
    /*
    for (int i = 0; i < memoire.len; i++) {
      afficher_var(memoire.variables[i], stdout);
    }
    */
    vider_exp(expressions);
    e++;
  }

  printf("\nmemoire finale:\n");
  for (int i = 0; i < memoire.len; i++) {
    afficher_var(memoire.variables[i], stdout);
  }

  // pile p1le = init_pile();
  // varStore *b = edit_var(&p1le, "b", string2unbounded_int("2025"));
  // varStore *c = edit_var(&p1le, "c", string2unbounded_int("-2"));
  // varStore *a = search_var(&p1le, "a");
  //
  // varStore *d = var_operation(&p1le, "d", "b", '*', "c");
  //
  // afficher_pile_var(&p1le, "truce", outputFile);

  // char *listeMots[] = {"556", "lala", "lala55", "la_la55"};
  // for (int i = 0; i < 4; i++) {
  //   printf("%s\n", listeMots[i]);
  //   printf("%d\n\n", est_variable_entier(listeMots[i]));
  // }
  //
  // printf("%d\n", est_operation("*"));

  free_pile(&memoire);
  fclose(inputFile);
  fclose(outputFile);
  return 0;
}
