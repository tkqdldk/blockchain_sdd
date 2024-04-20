#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "primalite.h"

int main(void) {
    // temps d'exécution is_prime_naive
    double temps_cpu;
    long p = 262897;
    int startTime = clock();

    is_prime_naive(p);
    int endTime = clock();
    temps_cpu = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;
    printf("temps d'exécution: %f s\n", temps_cpu);

    FILE *f=fopen("Comparaison.txt","w");

	  clock_t temps_initial,temps_final,temps_initial2,temps_final2;
	  double temps_cpu1, temps_cpu2;

	  if( (f) != NULL ) {
      for( long i = 0 ;i< 10000; i+=100 ){
        //On prend le temps mis pour l'execution de mod_naive(13756683,i,7)
			  temps_initial=clock();
			  modpow_naive(13756683,i,7);
			  temps_final=clock();

  			//on convertit les "ticks consommes par modpow_naive en secondes"
  			temps_cpu1 = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

  			//On prend le temps mis pour l'execution de modpow(13756683,i,7)
  			temps_initial2 = clock();
  			modpow(13756683,i,7);
  			temps_final2=clock();

  			//on convertit les "ticks consommes par modpow en secondes"
  			temps_cpu2 = ((double)(temps_final2-temps_initial2))/CLOCKS_PER_SEC;


  			fprintf(f,"%ld %f %f \n",i,temps_cpu1,temps_cpu2);
		   }
    }else{
      printf("erreur\n");
	  }
	  fclose(f);

    dessiner_courbes();
    /* gnuplot
    set terminal png
    set output "courbes.png"
  	plot "courbe.txt" using 1:2 title 'modpow_naive' with lines
  	replot "courbe.txt" using 1:3 title 'modpow' with lines */

    return EXIT_SUCCESS;
}
