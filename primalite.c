#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.h"

int is_prime_naive(long p) {
  /*
  Renvoie 1 si p est premier, 0 sinon
  */
    if(p%2 == 0 || p < 3) {
        printf("%ld n'est pas un nombre premier\n", p);
        return 0;
    }
    else{
        for(int i = 3; i < p; i++) {
            if(p%i == 0) {
                printf("%ld n'est un pas nombre premier\n", p);
                return 0;
            }
        }
        printf("%ld est un nombre premier\n", p);
        return 1;
    }
}
long modpow_naive(long a, long m, long n) {
  /*
  Calcule a^m mod n par iteration avec la methode naive
  */
    long res = a;
    for(int i = 1; i < m; i++) {
        res = res * a;
    }
    res = res % n;
    return res;
}

long modpow(long a, long m, long n) {
  /*
  Calcule a^m mod n par recursion
  */
    a = a % n;
    if(m == 0) return 1;
    if(m == 1) return a;
    if(m%2 == 0) {
        long x = modpow(a, m/2, n);
        return (x * x) % n;
    } else {
        long x = modpow(a, m/2, n);
        return (((x * x) %n) *a) %n;
    }
}
//Test de primalite de Miller-Rabin

int witness(long a, long b, long d, long p) {
  /*
  Teste si a est un temoin de Miller pour un entier a donne
  */
    long x = modpow(a, d, p);
    if(x == 1) {
        return 0;
    }
    for(long i = 0; i < b; i++) {
        if(x == p-1) {
            return 0;
        }
    }
    return 1;
}

long rand_long(long low, long up) {
  /*
  Retourne un entier long genere aleatoirement entre low et up inclus
  */
    return rand() % (up - low + 1) +low;
}

int is_prime_miller(long p, int k) {
  /*Realise le test de Miller-Rabin en generant k valeurs de a, retourne 1 si aucun temoin n'a ete trouve
  */
    if(p == 2) {
        return 1;
    }
    if(!(p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while(!(d & 1)) {
        d = d / 2;
        b = b + 1;
    }
    //On genere k valeurs pour a, et on teste si c'est un temoin :
    long a;
    int i;
    for(i = 0; i < k; i++) {
        a = rand_long(2, p - 1);
        if(witness(a, b, d, p)) {
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k) {
  /*
  Retourne un nombre premier de taille k entre low_size et up_size
  */
    long low = 2, up = 2;
    for(int i = 1; i < low_size-1 ; i++) {
        low *= 2;
    }
    for(int i = 1; i < up_size ; i++) {
        up *= 2;
    }
    up = up - 1;

    long p = rand_long(low, up);
    int i = 0;
    while(i != 1) {
        p = rand_long(low, up);
        i = is_prime_miller(p, k);
    }
    return p;
}

void dessiner_courbes(){
    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu1;
    double temps_cpu2;
    long m = 0;
    FILE *f = fopen("courbes1.txt","w");
    if (f == NULL) {
        printf("ERREUR D'OUVERTURE DU FICHIER ! \n");
        exit(EXIT_FAILURE);
    }
    while (m < 400) { 
        //premiere courbe 
        temps_initial = clock(); 
        modpow_naive(2,m, 100);
        temps_final = clock(); 
        temps_cpu1 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        //deuxieme courbe 
        temps_initial = clock(); 
        modpow(2,m, 100);
        temps_final = clock(); 
        temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        fprintf(f, " %ld %f %f\n" ,m, temps_cpu1, temps_cpu2 );
        m += 20;
    } 
}
