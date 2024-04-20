#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.h"
#include "protocoleRSA.h"

int main(void) {
	//Generation de cle :
	//printf("ici\n");
	long p = random_prime_number(3, 7, 5000);
	long q = random_prime_number(3, 7, 5000);
	// printf("ici %ld %ld\n", p, q);
	while(p == q) {
    	q = random_prime_number(3, 7, 5000);
    	//printf("ici %ld\n", q);
	}

	//printf("ici\n");
	long n, s, u;
	generate_key_values(p, q, &n, &s, &u);
	//Pour avoir des cles positives ;
	if (u < 0) {
    	long t = (p - 1) * (q - 1);
    	u = u + t; // on aura toujours s*u mod t = 1
  	}

	// Affichage des clés en hexadecimal
	printf("cle publique = (%lx, %lx) \n", s, n);
	printf("cle privee = (%lx, %lx) \n", u, n);

	// Chiffrement :
	char mess[10] = "Hello";
	int len = strlen(mess);
	printf("%s\n","fonction encrypt");
	long* crypted = encrypt(mess, s, n);

	printf("Initial message : %s\n", mess);
	printf("Encoded representation : \n");
	printf("%s\n","fonction print_long_vector");
	print_long_vector(crypted, len);

	// Dechiffrement :
	printf("%s\n","fonction decrypt");
	char* decoded = decrypt(crypted, len, u, n);

	printf("Decoded : %s\n", decoded);

  	return 0;
}
