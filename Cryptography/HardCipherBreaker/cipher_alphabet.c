#include <stdio.h>
#include <string.h>
#include <stdlib.h>






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


struct cipher_alphabet {
	int alphabet[200];
	int counter;
};


/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                CIPHERALPHABET FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/




struct cipher_alphabet* build_cipher_alphabet() {
	int i;
	
	struct cipher_alphabet* alphabet = malloc(sizeof(struct cipher_alphabet*));
	
	for(i=0; i<106; i++) {
		alphabet->alphabet[i] = i;
	}
	alphabet->counter = 0;
	
	return alphabet;
}




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





void display_cipher_alphabet(struct cipher_alphabet* alphabet) {
	int i;
	
	printf("\nAlphabet\n[");
	for(i=0; i<106; i++) {
		printf(" %d,", alphabet->alphabet[i]);
	}
	printf("]\n");
	
}


/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





void update_cipher_alphabet(struct cipher_alphabet* alphabet, int* mapped_digits, int size) {
	int i, j, k;
	
	for(i=0; i<size; i++) {
		for(j=alphabet->counter; j<106; j++) {
			
			if(mapped_digits[i] == alphabet->alphabet[j]) {
				
				k = alphabet->alphabet[alphabet->counter];
				alphabet->alphabet[alphabet->counter] = alphabet->alphabet[j];
				alphabet->alphabet[j] = k;
				
				alphabet->counter++;
			}
		}
	}
	
}


/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/








