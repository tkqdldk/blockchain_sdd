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

int main() {
    const char *s = "Rosetta code";
    unsigned char *d = SHA256(s, strlen(s),0);
    int i;
    for(i = 0; i < SHA256_DIGEST_LENGTH ; i++) {
        printf("%02x", d[i]);
    }
    putchar('\n') ;

	unsigned char* a = hash_char("Rosetta code");

	for (i = 0; i < SHA256_DIGEST_LENGTH; i ++)
	printf("%02x", a[i]);
	putchar('\n');
	free(a);

	Block *block = read_block("block.txt");
	write_block("Block.txt", block);
	char *blockstr = block_to_str(block);
	printf("block_to_str: \n%s\n", blockstr);
	free(blockstr);
	compute_proof_of_work(block, 3);
	printf("%d\n", verify_block(block, 3));
  //plot "proof_of_work.txt" using 1:2 title 'd' with lines
	delete_block(block);

    return 0;
}
