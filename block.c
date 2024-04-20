#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include <openssl/sha.h>
#include "primalite.h"
#include "protocoleRSA.h"
#include "declarations.h"
#include "decentralise.h"
#include "block.h"

void write_block(char *nomfic, Block* block){
  /*Permet d'ecrire un bloc dans un fichier*/
  FILE *f = fopen(nomfic, "w+");
  if (f == NULL) {
    printf("ERREUR D'OUVERTURE DE FICHIER");
    exit(EXIT_FAILURE);
  }
  char *strkey = key_to_str(block->author);
  fprintf(f, "%s\t%s\t%s\t%d\n", strkey,block->hash,block->previous_hash, block->nonce);
  free(strkey);
  CellProtected *tmp = block->votes;
  while (tmp) {
    char *strprotected = protected_to_str(tmp->data);
    fprintf(f, "%s\n", strprotected);
    free(strprotected);
    tmp = tmp->next;
  }
  fclose(f);
}

Block* read_block(char* nomfic){
  /*Permet de lire un bloc depuis un fichier*/
    FILE* f=fopen(nomfic,"r");
    if (!f){
        printf("erreur d'ouverture du fichier %s\n", nomfic);
        return NULL;
    }
    Block* b=(Block*) malloc (sizeof(Block));
    if(!b){
        printf("erreur d'allocation du block\n");
        fclose(f);
        return NULL;
    }
    char buf[256];
    fgets(buf, 256, f);
    char tmp[256], tmp1[256], tmp2[256], tmp3[256];
    if(sscanf(buf, "%s", tmp) == 1) {
        b->author=str_to_key(tmp);
    }
    fgets(buf, 256, f);
    if(sscanf(buf, "%s", tmp) == 1) {
        printf("hash : %s\n", tmp);
        b->hash=(unsigned char*)strdup(tmp);
        printf("copy : %s\n", b->hash);
    }
    fgets(buf, 256, f);
    if(sscanf(buf, "%s", tmp) == 1) {
        b->previous_hash=(unsigned char*)strdup(tmp);
    }
    int i;
    fgets(buf, 256, f);
    if(sscanf(buf, "%d", &i) == 1) {
        b->nonce=i;
    }
    CellProtected* declarations=(CellProtected*) malloc (sizeof(CellProtected));
    while(fgets(buf, 256, f)){
        if (sscanf(buf, "%s %s %s", tmp1, tmp2, tmp3)==3){
            Protected* pr=str_to_protected(buf);
            add_cell_protected(&declarations, pr);
        }
        else{
            printf("erreur de lecture\n");
            fclose(f);
            delete_list_protected(declarations);
            free(b->hash);
            free(b->previous_hash);
            free(b->author);
            free(b);
        }
    }
    b->votes=declarations;
    fclose(f);
    return b;
}

char* block_to_str(Block* block){
  /*Genere une chaine de caracteres representant un bloc*/
  char str[11];
  char *auteur =key_to_str(block->author);
  char *finalchar=(char*)calloc(sizeof(char),10000);
  if(!finalchar) printf("Erreur d'allocation \n");
  finalchar[0] = '\0';
  strcat(finalchar,auteur);
  strcat(finalchar,"\n");

  if(block->previous_hash){
    strcat(finalchar,(char*)block->previous_hash);
  }
  else{
    strcat(finalchar,"(null)");
  }

  strcat(finalchar,"\n");
  CellProtected * tmp = block->votes;
  char *votes;

  while(tmp){
    votes=protected_to_str(tmp->data);
    strcat(finalchar,votes);
    strcat(finalchar,"\n");
    free(votes);
    tmp=tmp->next;

  }

  sprintf(str, "%d", block->nonce);
  strcat(finalchar,str);
  strcat(finalchar,"\n");

  free(auteur);

  return finalchar;

}

unsigned char* hash_char(char* chaine){
  /*Retourne la valeur hachee d'une chaine de caracteres obtenur par l'lgorithme SHA256 */
    return strdup(SHA256(chaine, strlen(chaine), 0));
}

void compute_proof_of_work(Block *B, int d){
  /*Rend un bloc valide*/
    B->nonce = 0;
    while(1){
        char* blockstr = block_to_str(B);
        unsigned char* hashed = hash_char(blockstr);
        int i=0;
        while(i < d){
            if(hashed[i]=='0'){
                i++;
            }
            else{
                (B->nonce)++;
                break;
            }
        }
        if (i==d) break;
    }
}//Courbe du temps

int verify_block(Block* B, int d){
  /*Verifie qu'un bloc est valide*/
    int i=0;
    while (i<d){
        if (B->hash[i]!='0'){
            return 0;
        }
        i++;
    }
    return 1;
}

void delete_block(Block* b){
  /*Supprime un bloc sans liberer la memoire associee au champs author*/
    free(b->hash);
    free(b->previous_hash);
    CellProtected *tmp;
    while(b->votes){
        tmp = b->votes->next;
        free(b->votes);
        b->votes = tmp;
    }
    free(b);
}

void free_block(Block *block) {
    delete_block(block);
    free(block);
}

CellTree* create_node(Block* b){
  /*Permet de creer et d'initialiser un noeud*/
  CellTree *abr=(CellTree*)malloc(sizeof(CellTree));
  if(abr==NULL){
    printf("Erreur d'allocation \n");
    exit(EXIT_FAILURE);
  }
  abr->block=b;
  abr->father=NULL;
  abr->firstChild=NULL;
  abr->nextBro=NULL;
  abr->height=0;
  return abr;
}

int update_height(CellTree* father, CellTree* child){
  /*Permet de mettre a jour la hauteur du noeud father quand l'un de ses fils a ete modifie*/
  if(father->height==fmaxf(father->height,child->height+1)){
    return 0;
  }
  return 1;
}

void add_child(CellTree* father, CellTree* child){
  /*Ajoute un fils a un noeud, en mettant a jour la hauteur de tous les ascendants*/
  child->nextBro=father->firstChild;
  father->firstChild=child;
  child->father=father;
  CellTree* pere=father;
  CellTree* enfant=child;
  while(pere){
    if(update_height(pere,enfant)){
      pere->height=enfant->height+1;
    }
    enfant=pere;
    pere=enfant->father;
  }
}

void print_tree(CellTree* abr){
  /*Affiche un arbre*/
  if(abr==NULL){
    return;
  }
  else{
    char* blockstr = block_to_str(abr->block);
    printf("====================================\n");
    printf("bloc: %s, hauteur: %d\n", blockstr, abr->height);
    printf("====================================\n");
    free(blockstr);
    if(abr->firstChild){
      print_tree(abr->firstChild);
    }
    if(abr->nextBro){
      print_tree(abr->nextBro);
    }
  }
}

void delete_node(CellTree* node){
  /*Supprime un noeud*/
  free_block(node->block);
  free(node);
}

void delete_tree(CellTree* abr){
  /*Supprime un arbre*/
  if(abr==NULL){
    return;
  }
  delete_tree(abr->firstChild);
  delete_tree(abr->nextBro);
  delete_node(abr);
}

CellTree* highest_child(CellTree* abr){
  /*Renvoie le noeud fils avec la plus grande hauteur*/
  CellTree* child=abr->firstChild;
  CellTree *highest=child;
  int h=child->height;
  while(child){
    if(child->height>h){
      h=child->height;
      highest=child;
    }
    child=child->nextBro;
  }
  return highest;
}

CellTree* last_node(CellTree* abr){
  /*Permet de retourner le dernier bloc de cette plus longue chaine*/
  if(abr->height>0){
    return last_node(highest_child(abr));
  }
  return abr;
}

CellProtected* fusion(CellProtected* fus,CellProtected* pr1, CellProtected* pr2){
  /*Fusionne deux listes chainees de declarations signees*/
  //CellProtected *fus=(CellProtected*)malloc(sizeof(CellProtected*));
  //CellProtected *pr=(CellProtected*)malloc(sizeof(CellProtected*));
  CellProtected* pr=pr1;
  if(fus->data==NULL && pr){
    fus=create_cell_protected(pr->data);
    pr=pr->next;
  }
  while(pr){
    add_cell_protected(&fus,pr->data);
    pr=pr->next;
  }
  pr=pr2;
  if(fus->data==NULL && pr){
    fus=create_cell_protected(pr->data);
    pr=pr->next;
  }
  while(pr){
    add_cell_protected(&fus,pr->data);
    pr=pr->next;
  }
  return fus;
}//Complexite et modifs*

CellProtected *max_chaines(CellTree* abr,CellProtected *votes){
  /*Retourne la liste obtenue par fusion des listes chainees de declarations contenues dans les blocs de la plus longue chaine*/
  if(abr->height>0){
    CellTree* highest=highest_child(abr);
    CellProtected* decla=fusion(votes,highest->block->votes,abr->block->votes);
    return max_chaines(highest,decla);
  }
  return votes;
}

void submit_vote(Protected *p){
  /*Permet à un citoyen de soumettre un vote*/
  FILE *f=fopen("Pending_votes.txt","a");
  char* s=protected_to_str(p);
  fprintf(f,"%s\n",s);
  free(s);
  fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){
  /*cree un block valide contenant les votes en attente et ecrit le bloc obtenu dans un fichier 'Pending_block'*/
  Block *block=(Block*)malloc(sizeof(Block*));
  block->votes=read_protected("Pending_votes.txt");
  block->author=author;
  block->hash=hash_char("");

  CellTree *lastnode=last_node(tree);
  block->previous_hash=strdup((char*)lastnode->block->hash);
  compute_proof_of_work(block,d);

  CellTree *child=create_node(block);
  add_child(lastnode,child);
  remove("Pending_votes.txt");
  write_block("Pending_block.txt",block);
}

void add_block(int d, char* name){
  /*Verifie que le bloc representé par le fichier 'Pending_block' est valide*/
  Block *block = read_block("Pending_block.txt");
  printf("=== Dans add_block ===\n");
  printf("hash = %s, previous hash = %s\n", block->hash, block->previous_hash);
  if (verify_block(block, d)) {
    FILE *f = fopen(name, "w");
    write_block(name, block);
    char rep[50] = "Blockchain/";
    rename(name, strcat(rep, name));
    fclose(f);
  }
  free_block(block);
}

CellTree* read_tree(int nv, int nc){
  /*Construit l'arbre correspondant aux blocs contenus dans le répertoire 'Blockchain'*/
  CellTree **T = (CellTree**)malloc(sizeof(CellTree*) * (nv + nc));
    DIR *rep = opendir("./Blockchain/");
    int i = 0;
    if (rep != NULL) {
      struct dirent *dir;
      while ((dir = readdir(rep))) {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
          printf("Chemin du fichier : ./Blockchain/%s\n", dir->d_name);
          char dir_name[50] = "./Blockchain/";
          Block *block = read_block(strcat(dir_name, dir->d_name));
          CellTree *ct = create_node(block);
          T[i] = ct;
          i++;
        }
      }
      closedir(rep);
    }
    int size = i;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (T[j]->block->previous_hash == T[i]->block->hash) {
          add_child(T[i], T[j]);
        }
      }
    }
    for (int i = 0; i < size; i++) {
      if (T[i]->father == NULL) {
        CellTree *racine = T[i];
        free(T);
        return racine;
      }
    }
  return NULL;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
  /*Determine le gagnant de l'election en se basant sur la plus longue chaine de l'arbre*/
  CellProtected *votes = (CellProtected *)malloc(sizeof(Protected));
  CellProtected *decl = max_chaines(tree, votes);
  fraude(&votes);
  Key *gkey = compute_winner(decl, candidates, voters, sizeC, sizeV);
  free(votes);
  return gkey;
}
