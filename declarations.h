#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "primalite.h"
#include "protocoleRSA.h"

typedef struct key_ {
	long val;
	long n;
} Key;

typedef struct signature_ {
    long* content;
    int size;
} Signature;

typedef struct protected_ {
	Key* pKey;
	char* mess;
  Signature* sgn;
} Protected;

void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);
Signature* init_signature(long* content, int size);
Signature* sign(char* mess, Key* sKey);
char* signature_to_str(Signature* sgn);
Signature* str_to_signature(char* str);
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
char* protected_to_str(Protected* p);
Protected* str_to_protected(char* s);
int verify(Protected *pr);
void generate_random_data(int nv, int nc);

#endif
