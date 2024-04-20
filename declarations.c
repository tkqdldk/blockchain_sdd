#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.h"
#include "protocoleRSA.h"
#define N 5000

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

void init_key(Key* key, long val, long n) {
	/*Initialise une cle deja allouee
	*/
	key->val = val;
	key->n = n;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size) {
    /*initialise les valeurs d'une structure Key en utilisant le protocole RSA */
	long p = random_prime_number(low_size, up_size, N);
	long q = random_prime_number(low_size, up_size, N);

	while(p == q) {
		q = random_prime_number(low_size, up_size, N);
	}

	long n, s, u;
	generate_key_values(p, q, &n, &s, &u);
	if(u < 0) {
		long t = (p - 1) * (q - 1);
		u += t;
	}
    init_key(sKey, u, n);
	init_key(pKey, s, n);
}

char* key_to_str(Key* key) {
    /* passe une variable de type Key à sa représentation sous forme de chaîne de caractères */
	char s[256];
    if(key == NULL) {
        return NULL;
    }
    sprintf(s, "(%lx, %lx)", key->val, key->n);
	return strdup(s);
}

Key* str_to_key(char* str) {
    /* passe d'une représentation sous forme de chaîne de caractères à sa variable de type Key */
    long val, n;
    Key* nKey = (Key*) malloc(sizeof(Key*));
    char buffer[256];
    for(int i = 0; i < strlen(str); i++) {
        buffer[i] = str[i];
    }

    buffer[strlen(str) + 1] = '\0';
    sscanf(buffer, "(%lx, %lx)", &val, &n);
    init_key(nKey, val, n);
    return nKey;
}

Signature* init_signature(long* content, int size) {
	/*Rempli une signature avec un tableau de long deja alloue
	*/
    Signature* s = (Signature*) malloc(sizeof(Signature*));
    if(s == NULL) {
        return NULL;
    }
		s->content=content;
    s->size = size;

    return s;
}

Signature* sign(char* mess, Key* sKey) {
	/*Cree une signature a partir d'un message mess
	*/
    Signature* s = init_signature(encrypt(mess, sKey->val, sKey->n), strlen(mess));
    return s;
}

char* signature_to_str(Signature* sgn) {
		/*Passe d'une signature a une representation sous forme de chaine
		*/
    char* result = malloc(10 * sgn->size * sizeof(char));
    result[0] = '#';
    int pos = 1;
    char buffer[256];
    for(int i = 0; i < sgn->size; i++) {
        sprintf(buffer, "%lx", sgn->content[i]);
        for(int j = 0; j < strlen(buffer); j++) {
            result[pos] = buffer[j];
            pos = pos + 1;
        }
        result[pos] = '#';
        pos = pos + 1;
    }
    result[pos] = '\0';
    result = realloc(result, (pos+1) * sizeof(char));
    return result;
}

Signature* str_to_signature(char* str) {
	/*Passe d'une chaine de caractere a une signature
	*/
    int len = strlen(str);
    long* content = (long*) malloc(sizeof(long) * len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i = 0; i < len; i++) {
        if(str[i] != '#') {
            buffer[pos] = str[i];
            pos = pos + 1;
        } else {
            if(pos != 0) {
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc(content, num * sizeof(long));
    return init_signature(content, num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn) {
		/*Alloue et initialise une structure protected
		*/
    Protected* pr = (Protected*) malloc(sizeof(Protected));
    if (pr == NULL) {
        printf("Erreur d'allocation !");
        exit(EXIT_FAILURE);
    }
    pr->pKey = pKey;
    pr->mess = strdup(mess);
    pr->sgn = sgn;
    return pr;
}

char* protected_to_str(Protected* p) {
	/*Passe d'une structure protected a sa representation sous forme de chaine de caractere
	*/
    char s[256]="";
    char* key = key_to_str(p->pKey);
    char* sgn = signature_to_str(p->sgn);
    sprintf(s,"(%s, %s, %s)", key, p->mess, sgn);
    free(key);
    free(sgn);
    return strdup(s);
}

Protected* str_to_protected(char* s) {
		/*Passe d'une chaine de caractere a une structure protected
		*/
    char key_str[256];
    char mess_str[256];
    char sgn_str[256];
    Key* key;
    Signature* sgn;
    Protected* newPr;

    sscanf(s, "(%s, %s, %s)", key_str, mess_str, sgn_str);
    key = str_to_key(key_str);
    sgn = str_to_signature(sgn_str);
    newPr = init_protected(key, mess_str, sgn);
    return newPr;
}

int verify(Protected *pr) {
	/*Verifie que la siganture contenue dans une structure protected correspond bien a la personne et au message
	*/
	char* b = decrypt( pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n );
	int i = strcmp( pr->mess,b );
	free(b);
	if ( i == 0 ){
		 return 1 ;
	 }
  return 0;
}

void generate_random_data(int nv, int nc) {
	/*Cree 3 fichiers, contenant respectivement les cles,les candidats et toutes les declarations signees*/
	int a;
	Key* pTable[nv];
	Key* sTable[nv];
	Key* candidates[nc];
	Key *pKey, *sKey;
	Signature *sgn;
	Protected* pr;
	char *b1, *b2;

	//Keys
	FILE *f1 = fopen("keys.txt", "w");
	if (f1 == NULL) {
        printf("Erreur d'ouverture fichier \n");
        exit(EXIT_FAILURE);
    }

	for(int i = 0; i < nv; i++) {
		pKey = (Key*) malloc(sizeof(Key));
		sKey = (Key*) malloc(sizeof(Key));

		if (!pKey || !sKey) {
			fprintf(stderr, "Erreur d'allocation\n");
			return;
		}
    	init_pair_keys(pKey, sKey, 7, 10);
    	pTable[i] = pKey;
		sTable[i] = sKey;
		b1 = key_to_str(pKey);
		b2 = key_to_str(sKey);
		fprintf(f1, "(%lx,%lx) (%lx,%lx)\n", pKey->val, pKey->n, sKey->val, sKey->n);
		free(b1);
    	free(b2);
	}
	fclose(f1);

	//Candidates
	FILE *f2 = fopen("candidates.txt", "w");
	if (f2 == NULL) {
		printf("Erreur d'ouverture fichier \n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < nc; i++) {
		a = (int) rand() % nv;
		candidates[i] = pTable[a];
		b1 = key_to_str(candidates[i]);
		fprintf(f2, "%s\n", b1);
		free(b1);
	}
	fclose(f2);

	//Declarations
	FILE *f3 = fopen("declarations.txt","w");
	if (f3 == NULL) {
		printf("Erreur d'ouverture fichier \n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < nv; i++) {
		a = (int) rand() % nc;
		b1 = key_to_str(candidates[a]);
		sgn = sign(b1, sTable[i]);
		pr = init_protected(pTable[i], b1, sgn);
		b2 = protected_to_str(pr);
		fprintf(f3, "%s\n", b2);
		free(sgn->content);
		free(sgn);
		free(b1);
		free(b2);
		free(pr);
	}

	fclose(f3);
	for(int i = 0; i < nv; i++) {
		free(pTable[i]);
		free(sTable[i]);
	}
	return;
}
