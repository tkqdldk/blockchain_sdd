#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "primalite.h"

long extended_gcd(long s, long t, long *u, long *v) {
  /*
  Algorithme d'Euclide
  */
    if(t == 0) {
        *u = 1;
        *v = 0;
        return s;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t, s%t, &uPrim, &vPrim);
    *u = vPrim;
    *v = uPrim - (s/t) * vPrim;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long *s, long *u) {
  	/*
    Permet de generer la cle publique pkey=(s,n) et la cle secrete skey=(u,n) a
  	partir des nombres premiers p et q en suivant le protocole RSA
    */
  	*n = p*q;
  	int t = (p-1) * (q-1);
  	long v;

  	while(extended_gcd(*s, t, u, &v) != 1) {
    	*s = rand_long(1, t);
  	}
}

long* encrypt(char* chaine, long s, long n) {
  /*Chiffre la chaine de caractere avec la cle publique pKey=(s,n)
  */
    int size = (strlen(chaine));
    long *encrypted = (long*) malloc(sizeof(long)*size);
    if(encrypted==NULL){
      return NULL;
    }
    for(int i = 0; i < size; i++) {
        encrypted[i] = modpow((long)chaine[i], s, n);
    }
    return encrypted;
}

char* decrypt(long* crypted, int size, long u, long n) {
  /*Dechiffre un message a l'aide de la cle secrete sKey=(u,n)
  */
    char* decrypted = (char*) malloc(sizeof(char)*(size+1));
    for(int i = 0; i < size; i++) {
        decrypted[i] = (char)modpow(crypted[i], u, n);
    }
    decrypted[size] = '\0';
    return decrypted;
}

void print_long_vector(long *result, int size) {
    printf("Vector: [");
    for(int i = 0; i < size; i++) {
        printf("%lx\t", result[i]);
    }
    printf("]\n");
}
