#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "primalite.h"
#include "protocoleRSA.h"
#include "declarations.h"
#include "decentralise.h"

int NOMBRE_CANDIDATS=5;
int NOMBRE_DECLARATIONS=1000;

int main(void) {
	generate_random_data(5,2);
  	CellKey* listeVoters = read_public_keys("keys.txt");
	printf("fichiers ouverts1\n");
	CellKey* listeCandidates = read_public_keys("candidates.txt")  ;
	printf("fichiers ouverts2\n");
	CellProtected* listeDeclaration = read_protected("declarations.txt") ;
	printf("fichiers ouverts3\n");

	Key* winner = compute_winner(listeDeclaration, listeCandidates, listeVoters, NOMBRE_CANDIDATS, NOMBRE_DECLARATIONS);
	printf("compute_winner OK\n");

	char* keys = key_to_str(winner);
	printf("L'élu est %s\n", keys);

	free(keys);
	//printf("delete 1\n");
	delete_cell_key(listeVoters);
	//printf("delete2\n");
	delete_cell_key(listeCandidates);
	//printf("delete3\n");
	/*
	 * delete_celles_protected creee rarement des erreurs de segmentations :
	 * peut etre on a free un element du protected dans hv quand on les supprime apres avoir vote
	 * solution copier sur je sais plus qui qui modifie juste la valeur de val et les supprimes pas
	 * peut etre que c est la key audessus
	 * sinon chez pas
	 */
	delete_cell_protected(listeDeclaration);
    return 0 ;
}
