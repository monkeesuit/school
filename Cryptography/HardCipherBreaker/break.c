#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "functions.h"

#include "parse.h"
#include "cipher_alphabet.h"
#include "keyspace.h"

#include "guess_space_key.h"




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/








int main(void) {
	
	int i, j, k;
	
	char* dictionary = malloc(2000000 * sizeof(char*));
	
	char* filename= malloc(100 * sizeof(char*));
	
	char* rawciphertext = malloc(1500 * sizeof(char*));
	int* ciphertext = malloc(800 * sizeof(int*));
	
	char* plaintext = malloc(800 * sizeof(char*));
	
	struct digit* dictionary_digit = NULL;
	struct digit* entry_digit;
	
	struct cipher_alphabet* alphabet = NULL;
	
	struct key* dictionary_key = NULL;
	
	
	// read words from dictionary
	dictionary = read("words.txt");
	
	// Read in file containing ciphertext
	printf("[break]> Make sure the cipher .txt file is in the same directory as this !!!\n");
	printf("[break]> Make sure the cipher .txt file is terminated with a ',' !!!\n");
	printf("[break]> If program fails just keep running until it works !!!\n\n");
	filename = input("[break]> Ciphertext Filename $> ");
	
	// Format file for parsing
	rawciphertext = read(filename);
	free(filename);
	ciphertext = intcsv(rawciphertext);
	free(rawciphertext);
	
	// Parse ciphertext recording relevant statistics
	for(i=0; i<500; i++) {
		dictionary_digit = parse_ciphertext(dictionary_digit, ciphertext, i);
	}
	dictionary_digit = unused_digits(dictionary_digit);
	display_digit_structure(dictionary_digit);

	// Build a structure that keeps track of the digits that have been mapped
	alphabet = build_cipher_alphabet();
	display_cipher_alphabet(alphabet);
	
	// Build a structure that represents the key
	dictionary_key = reset_keyspace(dictionary_key);
	display_keyspace(dictionary_key);
	
	// Build a potential key for <space>
	int ok = 0;
	
	while (ok == 0) {
		int* chosen;

		chosen = find_candidates(dictionary_digit);
		ok = *(test_chosen(dictionary_digit, chosen));
		free(chosen);
		printf("\n%d\n", ok);
	}
	
	printf("Done");
	
	
	
	return 1;
}
