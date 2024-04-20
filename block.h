#ifndef BLOCK_H
#define BLOCK_H

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

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

typedef struct block_tree_cell{
  Block* block;
  struct block_tree_cell* father;
  struct block_tree_cell* firstChild;
  struct block_tree_cell* nextBro;
  int height;
} CellTree;

void write_block(char *nomfic, Block* block);
Block* read_block(char* nomfic);
char* block_to_str(Block* block);
unsigned char* hash_char(char* chaine);
void compute_proof_of_work(Block *B, int d);
int verify_block(Block* B, int d);
void delete_block(Block* b);
void free_block(Block *block);
CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree* abr);
void delete_node(CellTree* node);
void delete_tree(CellTree* abr);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected* fusion(CellProtected *fus,CellProtected* pr1, CellProtected* pr2);
CellProtected *max_chaines(CellTree* abr,CellProtected *votes);
void submit_vote(Protected *p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree(int nv, int nc);
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif
