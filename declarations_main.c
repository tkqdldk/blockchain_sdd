#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "primalite.h"
#include "protocoleRSA.h"
#include "declarations.h"

int main(void){

	srand(time(NULL));

	//Testing Init Keys
	Key* pKey = malloc(sizeof(Key));
	Key* sKey = malloc(sizeof(Key));
	init_pair_keys(pKey, sKey, 3 ,7);
	printf("Test Init Keys :\n");
	printf("pKey: (%lx,%lx)\n", pKey->val, pKey->n);
	printf("sKey: (%lx,%lx)\n", sKey->val, sKey->n);

	//Testing Key Serialization
	char* chaine = key_to_str(pKey);
	printf("\nTest key serialization:\n");
	printf("key_to_str: %s \n", chaine);
	Key* k = str_to_key(chaine);
	printf("str_to_key: (%lx,%lx)\n", k->val, k->n);

	//Testing Signature
	//Candidate Keys
	Key* pKeyC = malloc(sizeof(Key));
	Key* sKeyC = malloc(sizeof(Key));
	init_pair_keys(pKeyC, sKeyC, 3, 7);
	//Declaration
	char* tmp  = key_to_str(pKey) ;
	char* mess = key_to_str(pKeyC);
	printf("\nTest Signature\n");
	printf("%s vote pour %s\n", tmp, mess);
	Signature* sgn = sign(mess, sKey);
	printf("Signature: ");
	print_long_vector(sgn->content,sgn->size);


	//Testing protected
	char *key, *sign ;

	Protected* pr = init_protected(pKey, mess, sgn);
	//verif
	if( verify ( pr ) ) {
		printf ( "\nSignature valide\n" ) ;
	}else{
		printf ( "\nSignature non valide\n" ) ;
	}
	free(chaine);
	free(pr);
	chaine = protected_to_str ( pr ) ;
	printf ( "\nprotected_to_str : %s \n" , chaine ) ;
	pr = str_to_protected ( chaine ) ;
	key = key_to_str ( pr->pKey );
	sign = signature_to_str ( pr->sgn ) ;
	printf ( "str_to_protected: %s ,%s ,%s \n\n" , key ,pr->mess , sign ) ;

	generate_random_data(5, 2);

	free ( k ) ;
	free ( pr->mess );
	free ( pr->sgn->content );
	free ( pr->sgn );
	free ( pr->pKey );
	free ( sgn->content ) ;
	free ( pr ) ;
	free ( key ) ;
	free ( tmp ) ;
	free ( sgn ) ;
	free ( mess ) ;
	free ( sign ) ;
	free ( pKey ) ;
	free ( sKey ) ;
	free ( pKeyC ) ;
	free ( sKeyC ) ;
	free ( chaine ) ;
	return 0;
}
