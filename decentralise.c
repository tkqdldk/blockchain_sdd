#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.h"
#include "protocoleRSA.h"
#include "declarations.h"
#include "decentralise.h"

CellKey* create_cell_key(Key* key) {
  /*Alloue et initialise une cellule de liste chainee
  */
    CellKey* c = (CellKey*) malloc(sizeof(CellKey*));
    struct cellKey* nxt = (struct cellKey*) malloc(sizeof(struct cellKey*));
    if(c == NULL) {
        return NULL;
    }
    c->data = key;
    c->next = nxt;
    return c;
}

CellKey* add_cell_key(CellKey* liste, Key* nKey) {
  	/*Ajoute une cle en tete de liste
  	*/
    CellKey* new = create_cell_key(nKey);
    new->next = liste;
    return new;
}

CellKey* read_public_keys(char* file) {
   	/*Retourne une liste chainee contenant toutes les cles publiques du fichier
   	*/
    FILE* f = fopen(file, "r");
    if(f == NULL) {
      printf("Erreur d'ouverture fichier \n");
    	return NULL;
    }
    CellKey *ck = NULL;
    char buffer[256];
    char s[100];
    Key *key ;

    while(fgets(buffer,256,f) != NULL) {
      sscanf(buffer,"%s ",s);
      key = str_to_key(buffer);
      add_cell_key(ck,key);
    }
    fclose(f);
    return ck;
}

void print_list_keys(CellKey* LCK) {
  /*Permet d'afficher une liste chainee de cles
  */
    if(LCK == NULL) {
        printf("la liste est vide\n");
        exit(EXIT_FAILURE);
    }
    CellKey* courant = LCK;
    while(courant != NULL) {
        char* printKey = key_to_str(courant->data);
        printf("%p - %s\n", courant, printKey);
        courant = courant->next;
    }
}

void delete_cell_key(CellKey* c) {
  /*Supprime une cellule de liste chainee de cles
  */
    if(c == NULL) {
        printf("la liste est vide\n");
        exit(EXIT_FAILURE);
    }
    free(c->data);
    c->data = NULL;
    c = c->next;
}

void delete_list_keys(CellKey* liste) {
  /*Supprime une liste chainee de cles
  */
    if(liste == NULL) {
        printf("la liste est vide\n");
        exit(EXIT_FAILURE);
    }
    CellKey* courant = liste;
    while(courant != NULL) {
        delete_cell_key(liste);
        courant = courant->next;
    }
}

CellProtected* create_cell_protected(Protected* pr) {
  /*Alloue et initialise une cellule de liste chainee de protected
  */
    CellProtected* p = (CellProtected*) malloc(sizeof(CellProtected*));
    struct cellProtected* nxt = (struct cellProtected*) malloc(sizeof(struct cellProtected*));
    if(p == NULL) {
        return NULL;
    }
    p->data = pr;
    p->next = nxt;
    return p;
}

void add_cell_protected(CellProtected** cp, Protected* nPr) {
  /*Ajoute une declaration signee en tete de liste
  */
  CellProtected *new = create_cell_protected(nPr);
  new->next = *cp;
  *cp = new;
}

CellProtected* read_protected(char* file) {
	/*Lit le fichier file et cree une liste de toutes les declarations signees du fichier
   	*/
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        exit(EXIT_FAILURE);
    }
    CellProtected *cp = NULL;
    char buffer[256];
    Protected *pr;

    while (fgets(buffer, 256, f)) {
       pr = str_to_protected(buffer);
       add_cell_protected(&cp,pr);
    }
    fclose(f);
    return cp;
}

void print_list_protected(CellProtected* cp){
  /*Fonction d'affichage d'une liste de declarations signees
  */
  CellProtected *tmp = cp;
  if (!cp){ //si liste vide
      printf("liste vide !\n");
  }
  while (tmp) { //on parcours et on affiche
    char *str_pr = protected_to_str(tmp->data);
    printf("%s\n", str_pr);
    tmp = tmp->next;
    free(str_pr);
  }
}

void delete_cell_protected(CellProtected* p) {
  /*Supprime une cellule de liste chainee de protected
  */
    if(p == NULL) {
        printf("la liste est vide\n");
        exit(EXIT_FAILURE);
    }
    free(p->data);
    p = p->next;
}

void delete_list_protected(CellProtected* liste) {
  /*Supprime une liste chainee de declaratiosn signees
  */
    if(liste == NULL) {
        printf("la liste est vide");
        exit(EXIT_FAILURE);
    }
    CellProtected* courant = liste;
    while(courant != NULL) {
        delete_cell_protected(liste);
        courant = courant->next;
    }
}

void fraude(CellProtected** d) {
  	/*Supprime toute déclaration frauduleuse*/
  	if(d == NULL) {
    	return;
  	}
    else {
    	CellProtected* tmp = (CellProtected*) malloc(sizeof(CellProtected*));
    	tmp = *d;
    	while(*d) {
        	if(!verify((*d)->data)) {
          	*d = (*d)->next;
          	delete_cell_protected(*d);
        	}
    	}
    	CellProtected* suiv = (*d)->next;
    	while(suiv) {
      		if(!verify(suiv->data)){
        		suiv = suiv->next;
        		delete_cell_protected(suiv->next);
        		tmp->next = suiv;
      		}
      		else {
        		tmp = tmp->next;
        		suiv = suiv->next;
      		}
    	}
	}
}

HashCell* create_hashcell(Key* key) {
  	/*Alloue et initialise une cellule de la table de hachage
  	*/
  	HashCell *h = (HashCell*)malloc(sizeof(HashCell));
  	if(h == NULL) {
    	printf("Erreur d'allocation");
    	exit(EXIT_FAILURE);
  	}
  	h->key = key;
  	h->val = 0;
  	return h;
}

int hash_function(Key* key, int size) {
  	/*Retourne la positon d'un element de la table de hachage
  	*/
  	return key->val % size;
}

int find_position(HashTable* t, Key* key) {
	if(!key){printf("key NULL\n");return -1;}
	if(!t){printf("table NULL\n");return -1;}
	
	// on calcule f(val)
	unsigned int pos = hash_function(key, t->size);
	if (t->tab[pos] == NULL ||( t->tab[pos]->key->val==key->val && t->tab[pos]->key->n==key->n))	{ 
		// si la position est libre ou si la clé se situe déjà à f(val) on renvoie f(val)
			
		return pos;
	}
		
	unsigned int pos2;
	for(int i = 1; i<(t->size);i++){
		pos2 =(pos+i)%(t->size);
		// on parcours les cases suivantes jusqu'à trouver une case libre ou si la clé est déjà présente
		if( t->tab[pos2] == NULL){
			//printf("Clé non présente dans le tableau\n");
			return pos2;
		}
		if(t->tab[pos2]->key==key){
			return pos2;
		}
	}
		
	return pos;
}

HashTable* create_hashtable(CellKey* keys, int size) {
  	/*Cree et initialise une table de hachage de taille size*/
  	HashTable *hash = (HashTable*) malloc(sizeof(HashTable));
  	hash->tab = (HashCell**) malloc(sizeof(HashCell*) * size);
  	if (hash->tab == NULL) {
    	printf("Erreur d'allocation !");
    	exit(EXIT_FAILURE);
  	}
  	hash->size = size ;
  	CellKey *tmp = keys;
  	for (int i = 0; i < hash->size; i++) {
    	hash->tab[i] = NULL;
  	}

  	while (tmp) {
    	int pos =  find_position(hash, tmp->data);
    	hash->tab[pos] = create_hashcell(tmp->data);
    	tmp = tmp->next;
  	}
  	return hash;
}

void delete_hashtable(HashTable* h)	{
	/*Supprime une table de hachage*/
  	for(int i = 0; i < h->size; i++){
    	free(h->tab[i]);
  	}
  	free(h->tab);
  	free(h);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV) {
	/*Calcule le vainqueur de l'election etant donnes une liste de declarations avec signatures valides, une liste de candidats, et une liste de personnes autorisees a voter*/
  HashTable *hv = create_hashtable(voters, sizeV);
  HashTable *hc = create_hashtable(candidates, sizeC);

  Key *keyV=(Key*)malloc(sizeof(Key*));
  Key *keyC=(Key*)malloc(sizeof(Key*));
  int posV, posC;

  // On parcours les déclarations
  while (decl != NULL) {
    keyV = (decl->data)->pKey;
    posV = find_position(hv, keyV);

    // On vérifie que les électeurs existent et qu'ils n'ont pas votés
    if (((hv->tab)[posV] != NULL) && (((hv->tab)[posV])->val == 0)) {
      keyC = str_to_key((decl->data)->mess);
      posC = find_position(hc, keyC);

      // on vérifie que le candidat existe
      if ((hc->tab)[posC] != NULL) {
        // On ajoute un vote et on incrémente la valeur de l'électeur pour qu'il ne vote pas de nouveau
        ((hc->tab)[posC])->val += 1;
        ((hv->tab)[posV])->val = 1;
      }

      free(keyC);
    }

    decl = decl->next;
  }


  // on cherche le gagnant
  int max = 0;
  int position;
  Key *keyTmp=(Key*)malloc(sizeof(Key*));
  Key *keygagnant=(Key*)malloc(sizeof(Key*));

  while (candidates != NULL) {
    // on calcule la position des candidats dans la table
    position = find_position(hc, candidates->data);

    // si le candidat à plus de vote que celui en tête précedemment
  if(hc->tab[position]){
    if (((hc->tab)[position])->val > max) {
      max = ((hc->tab)[position])->val;
      keyTmp = ((hc->tab)[position])->key;
      init_key(keygagnant, keyTmp->val, keyTmp->n);
    }
  }

  candidates = candidates->next;
  }

  delete_hashtable(hv);
  delete_hashtable(hc);

  return keygagnant;
}
