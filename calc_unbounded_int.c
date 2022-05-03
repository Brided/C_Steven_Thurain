#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>

#include"unbounded_int.h"

#define NB_MAX_MOTS 20
#define NB_MAX_LINES 20
#define NB_CHAR_LINE 1024

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

//struct contenant un nom de variable et une valeur associée au nom de variable.
typedef struct varStore{
  char* nomVar;
  int valeur;
} varStore;

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


int est_operation(char* v){
  if(v==NULL || strlen(v)!=1) return 0;
  if(*v=='+' || *v=='-' || *v=='*') return 1;
  if(*v=='=') return 2;
  else return 0;
}



/*
  Vérifie si la chaîne de caractère est éligible d'être le nom d'une variable ou un entier.
  Retourne:
  - 4 si '='
  - 3 si c'est une opération (+,-,*),
  - 2 si elle contient des lettres et potentiellement des chiffres,
  - 1 si elle contient que des chiffres donc elle est un entier, 
  - 0 pour tout autre.
*/
int est_variable_entier(char* v){
  if(v==NULL || strlen(v)==0) return 0;
  int op=est_operation(v);
  if(op==1) return 3;
  else if(op==2) return 4;
  int var=1;
  for(int i=0;i<strlen(v);i++){
    if(isdigit(v[i])==0){
      if(isalpha(v[i])==0) return 0;
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

int parse_line(char* readLine, char** putLine) {
  int len=strlen(readLine);
  int l=0;
  int nbMots=0;
  for(int i=0;i<len;i++){
    char c=readLine[i];
    if(isblank(c)){
      if(l>0){	
	putLine[nbMots]=malloc(sizeof(char)*(l+1));
	if(putLine[nbMots]==NULL){
	  perror("parse_line/malloc\n");
	  return -1;
	}
	copyMot(readLine,putLine[nbMots],i-l,i-1);
	nbMots++;
	l=0;
	//printf("%s\n",putLine[nbMots-1]);
      }      
    }
    else{
      if(c=='=' || est_operation(&c)!=0){
	if(l>0){	
	  putLine[nbMots]=malloc(sizeof(char)*(l+1));
	  if(putLine[nbMots]==NULL){
	    perror("parse_line/malloc\n");
	    return -1;
	  }
	  copyMot(readLine,putLine[nbMots],i-l,i-1);
	  nbMots++;
	  l=0;
	  //printf("%s\n",putLine[nbMots-1]);
	}
	putLine[nbMots]=malloc(sizeof(char)*2);
	if(putLine[nbMots]==NULL){
	  perror("parse_line/malloc\n");
	  return -1;
	}
        putLine[nbMots][0]=c;
	putLine[nbMots][1]='\0';
	nbMots++;
	//printf("%s\n",putLine[nbMots-1]);
      }
      else l++;
    }
  }
  if(l>0){
    putLine[nbMots]=malloc(sizeof(char)*(l+1));
    if(putLine[nbMots]==NULL){
      perror("parse_line/malloc\n");
      return -1;
    }
    copyMot(readLine,putLine[nbMots],len-l,len-2);
    l=0;
  }
  //printf("%s : %d\n",putLine[nbMots],nbMots+1);
  nbMots++;
  return nbMots;
}



int main(int argc, char *argv[]) {
  /* code */
  int inputArgPos = search_args_options(argc, argv, "-i");
  int outputArgPos = search_args_options(argc, argv, "-o");
  printf("%d\n", inputArgPos);
  printf("%d\n", outputArgPos);

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

  char line[NB_CHAR_LINE];  
  struct expression* expressions[NB_MAX_LINES];
  int nbMots;
  int e=0;
  
  while (fgets(line, sizeof(line), inputFile)) {
    char* mots[NB_MAX_MOTS];
    nbMots=parse_line(line,mots);
    if(nbMots==-1){
      return 1;
    }
    expressions[e]=init_exp();
    for(int i=0;i<nbMots;i++){
      //printf("mot : %s\n",mots[i]);      
      add_mot(expressions[e],mots[i],1);         
    }
    printf("Ligne %d :\n",e);
    print_exp(expressions[e]);   
    e++;
  }

  char *listeMots[] = {"556", "lala", "lala55", "la_la55"};
  //
  for (int i = 0; i < 4; i++) {
    printf("%s\n", listeMots[i]);
    printf("%d\n\n", est_variable_entier(listeMots[i]));
  }
  
  printf("%d\n", est_operation("*"));

  fclose(inputFile);
  fclose(outputFile);
  return 0;
}
