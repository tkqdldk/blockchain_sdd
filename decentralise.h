#ifndef DECENTRALISE_H
#define DECENTRALISE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "primalite.h"
#include "protocoleRSA.h"
#include "declarations.h"

typedef struct cellKey {
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected {
    Protected* data;
    struct cellProtected* next;
} CellProtected;

typedef struct hashcell{
  Key* key;
  int val;
} HashCell;

typedef struct hashtable{
  HashCell** tab;
  int size;
} HashTable;

CellKey* create_cell_key(Key* key);
CellKey* add_cell_key(CellKey* liste, Key* nKey);
CellKey* read_public_keys(char* file);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* liste);
CellProtected* create_cell_protected(Protected* pr);
void add_cell_protected(CellProtected** cp, Protected* nPr);
CellProtected* read_protected(char* file);
void print_list_protected(CellProtected* cp);
void delete_cell_protected(CellProtected* p);
void delete_list_protected(CellProtected* liste);
void fraude(CellProtected** d);
HashCell* create_hashcell(Key* key);
int hash_function(Key* key, int size);
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif
