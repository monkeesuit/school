#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "arrays.h"
#include "hadriansfunctions.h"

#define EQUIVALENCE_RATIO 12
#define MINIMUM_EQUIVALENCE 12
#define SIZE 500
#define DEVIATION 3






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/

struct key {
	char letter;
	int frequency;
	int remaining;
	int digits[20];
	struct key *next;
};



struct cipher_alphabet {
	int alphabet[110];
	int mapped;
};



struct simpledigit {
	int digit;
	int count;
	
	struct simpledigit *next;
};



struct equivalence {
	int digit;
	int positive;
	int negative;
	
	struct equivalence *next;
};



struct digit {
	int digit;
	int count;
	struct equivalence *equivalence_head;
	
	struct digit *next;
	
	struct simpledigit *left_digit_head;
	struct simpledigit *right_digit_head;
	int *ordered_left;
	int *ordered_right;
};



struct pair {
	int pair[2];
	int count;
	
	struct pair *next;
};





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                MAKE KEY FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






struct key *makekey(struct key *head, char letter, int frequency) {
	int i;
	
	struct key *entry;
	
	entry = malloc(sizeof(struct key));
	
	entry->letter = letter;
	entry->frequency = frequency;
	entry->remaining = frequency;
	for(i=0; i<20; i++) {
		entry->digits[i] = 0;
	}
	entry->next = head;
	head = entry;
	
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                 DECRYPT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// decrypts a cipher digit to an english letter given a key
char decrypt_digit(struct key *key, int digit) {
	int i;
	
	while(key != NULL) {
		for(i=0; i< key->frequency; i++){
			if (key->digits[i] == digit) {
				return key->letter;
			}
		}
		key = key->next;
	}
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                GUESS ZERO FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





void assign_zero_digits(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	
	char zerocharacters[20] = {' ', ' ', ' ', 'h',
		'h', 'w', 'f', 'q',
		'x', 'k', 'y','t',
		'a', 'o', 'e','n'};
	
	int i, j, k;
	int counter_zerodigits;
	
	int zerodigits[20];
	
	char mappedcharacters[20];
	char ch1;
	
	struct digit *digit_pointer;
	struct key *key_pointer;
	
	
	counter_zerodigits = 0;
	
	// loop through cipher alphabet
	for(i=cipher_pointer->mapped; i<106; i++) {
		
		// loop through digit linked list
		digit_pointer = digit_head;
		while(digit_pointer != NULL) {
			
			// if a digit has been seen in the cipher text ignore it
			if(cipher_pointer->alphabet[i] == digit_pointer->digit) {
				break;
			}
			
			// if we reach the end of the list then the digit has not been seen
			digit_pointer	= digit_pointer->next;
			if(digit_pointer == NULL) {
				zerodigits[counter_zerodigits] = cipher_pointer->alphabet[i];
				counter_zerodigits++;
			}
		}
	}
	// got a list of zerodigits
	
	// map zerodigits to zerocharacters
	for (i = 0; i < counter_zerodigits; i++) {
		j = i + rand() % (16 - i);
		ch1 = zerocharacters[i];
		zerocharacters[i] = zerocharacters[j];
		zerocharacters[j] = ch1;
		
		mappedcharacters[i] = zerocharacters[i];
	}
	
	// save mapping to key
	for (i = 0; i < counter_zerodigits; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			if(key_pointer->letter == mappedcharacters[i]) {
				key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = zerodigits[i];
				key_pointer->remaining -= 1;
				
				break;
			}
			
			key_pointer = key_pointer->next;
		}
	}
	
	// remove mapped digits from alphabet
	for (i=0; i < counter_zerodigits; i++) {
		for (j=cipher_pointer->mapped; j<106; j++) {
			if(zerodigits[i] == cipher_pointer->alphabet[j]) {
				k = cipher_pointer->alphabet[cipher_pointer->mapped];
				cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[j];
				cipher_pointer->alphabet[j] = k;
				cipher_pointer->mapped++;
			}
		}
	}
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                GUESS SPACE FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





int* get_equivalence_classes(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	int i, j, k;
	int r1,r2;
	
	int space_candidates[40];
	static int ret_chosen[2];
	
	struct digit *digit_pointer;
	struct key *key_pointer;
	struct equivalence *equivalence_pointer;
	
	
	j = 0;
	
	digit_pointer = digit_head;
	while(digit_pointer != NULL) {
		
		i = 0;
		
		equivalence_pointer = digit_pointer->equivalence_head;
		while(equivalence_pointer != NULL) {
			
			// Score equivalances
			if(equivalence_pointer->positive > 0) {
				if((equivalence_pointer->negative / equivalence_pointer->positive) < EQUIVALENCE_RATIO) {
					i++;
				}
			}
			equivalence_pointer = equivalence_pointer->next;
		}
		
		if(i > MINIMUM_EQUIVALENCE) {
			
			// Low frequency digits may encode <space>
			if(digit_pointer->count < 4) {
				
				space_candidates[j] = digit_pointer->digit;
				j++;
				
			}
		}
		digit_pointer = digit_pointer->next;
	}
	
	// use equivalence score to guess two space characters
	r1 = rand() % j;
	r2 = rand() % j;
	while(r1 == r2) {
		r2 = rand() %j;
	}
	
	ret_chosen[0] = space_candidates[r1];
	ret_chosen[1] = space_candidates[r2];
	
	// save mapping to key
	for (i = 0; i < 2; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			if(key_pointer->letter == ' ') {
				key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = ret_chosen[i];
				key_pointer->remaining -= 1;
				
				break;
			}
			
			key_pointer = key_pointer->next;
		}
	}
	
	// remove mapped digits from alphabet
	for (i=0; i < 2; i++) {
		for (j=cipher_pointer->mapped; j<106; j++) {
			if(ret_chosen[i] == cipher_pointer->alphabet[j]) {
				k = cipher_pointer->alphabet[cipher_pointer->mapped];
				cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[j];
				cipher_pointer->alphabet[j] = k;
				cipher_pointer->mapped++;
			}
		}
	}
	
	return ret_chosen;
}




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






void get_equivalences(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer, int digit) {
	int i, j, k;
	int r1;
	
	int temp;
	int found;
	int killswitch;
	
	int counter_candidates;
	int counter_chosen;
	int space_candidates[20];
	int chosen[10];
	
	struct digit *digit_pointer;
	struct digit *digit_pointer2;
	struct key *key_pointer;
	struct equivalence *equivalence_pointer;
	
	counter_candidates = 0;
	killswitch = 0;
	
	digit_pointer = digit_head;
	while(digit_pointer != NULL) {
		
		if(digit_pointer->digit	== digit) {
			
			// go to their equivalences
			equivalence_pointer = digit_pointer->equivalence_head;
			while(equivalence_pointer != NULL) {
				
				// get  candidate equivalents
				digit_pointer2 = digit_head;
				while(digit_pointer2 != NULL) {
					
					if (equivalence_pointer->digit == digit_pointer2->digit) {
						
						if(digit_pointer2->count < 4) {
							
							if(equivalence_pointer->positive > 0) {
								
								if((equivalence_pointer->negative / equivalence_pointer->positive) < EQUIVALENCE_RATIO) {
									
									// found candidate
									space_candidates[counter_candidates] = digit_pointer2->digit;
									counter_candidates++;
									break;
									
								}
							}
						}
					}
					
					digit_pointer2 = digit_pointer2->next;
				}
				equivalence_pointer =equivalence_pointer->next;
			}
			break;
		}
		digit_pointer = digit_pointer->next;
	}
	
	// pick some candidates to encode to space
	counter_chosen = 0;
	for(i=0; i <= (counter_candidates*4)/10; i++) {
		
		while(1) {
			killswitch += 1;
			
			r1 = i + rand() % (counter_candidates - i);
			found = 0;
			
			// check if its been chosen already
			for(j=0; j<10; j++) {
				
				if(chosen[j] == space_candidates[r1]) {
					found = 1;
				}
				
			}
			
			// check if its already been mapped
			key_pointer = key_head;
			while(key_pointer != NULL) {
				
				if(key_pointer->letter == ' ') {
					
					for(k=0; k < key_pointer->frequency - key_pointer->remaining; k++) {
						
						if(key_pointer->digits[k] == space_candidates[r1]) {
							found = 1;
							break;
						}
					}
				}
				
				key_pointer	= key_pointer->next;
			}
			if(found == 0) {
				break;
			}
			if(killswitch > 10) {
				break;
			}
		}
		
		temp = space_candidates[i];
		space_candidates[i] = space_candidates[r1];
		space_candidates[r1] = temp;
		
		chosen[i] = space_candidates[i];
		counter_chosen++;
	}
	
	// save mapping to key
	for (i = 0; i < counter_chosen; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			if(key_pointer->letter == ' ') {
				key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = chosen[i];
				key_pointer->remaining -= 1;
				
				break;
			}
			
			key_pointer = key_pointer->next;
		}
	}
	
	// remove mapped digits from alphabet
	for (i=0; i < counter_chosen; i++) {
		for (j=cipher_pointer->mapped; j<106; j++) {
			if(chosen[i] == cipher_pointer->alphabet[j]) {
				k = cipher_pointer->alphabet[cipher_pointer->mapped];
				cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[j];
				cipher_pointer->alphabet[j] = k;
				cipher_pointer->mapped++;
			}
		}
	}
	
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





void fill_out_space(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	int i, j, k;
	
	int counter;
	int count;
	int count2;
	int remaining;
	int temp;
	int total;
	
	int space_candidates[80];
	int list[40];
	int chosen[40];
	
	struct digit *digit_pointer;
	struct key *key_pointer;
	
	count = 0;
	
	// fill out <spaces> key space
	
	// determine how many more digits we neeed and the frequency of the digits already chosen
	key_pointer = key_head;
	while(key_pointer != NULL) {
		if(key_pointer->letter == ' ') {
			
			remaining = key_pointer->remaining;
			
			for(i=0; i<key_pointer->frequency	- key_pointer->remaining; i++) {
				
				digit_pointer = digit_head;
				while(digit_pointer != NULL) {
					
					if(key_pointer->digits[i] == digit_pointer->digit) {
						count += digit_pointer->count;
					}
					
					digit_pointer = digit_pointer->next;
				}
				
			}
			
		}
		
		key_pointer	= key_pointer->next;
	}
	
	// get candidates for <space>
	counter = 0;
	digit_pointer = digit_head;
	while(digit_pointer != NULL) {
		
		if(digit_pointer->count < 8) {
			
			for(i=cipher_pointer->mapped; i<106; i++) {
				
				if(cipher_pointer->alphabet[i] == digit_pointer->digit) {
					
					space_candidates[counter] = digit_pointer->digit;
					counter++;
					break;
					
				}
				
			}
			
		}
		
		digit_pointer = digit_pointer->next;
	}
	
	// choose digits for space that comply to some rules
	do {
		count2 =0;
		
		for(i=0; i < counter; i++) {
			list[i] = space_candidates[i];
		}
		
		for (i = 0; i < remaining; i++) {
			j = i + rand() % (counter - i);
			temp = list[i];
			list[i] = list[j];
			list[j] = temp;
			
			chosen[i] = list[i];
		}
		
		for (i = 0; i < remaining; i++) {
			
			digit_pointer = digit_head;
			while(digit_pointer != NULL) {
				
				if(chosen[i] == digit_pointer->digit) {
					
					count2 += digit_pointer->count;
					
				}
				
				digit_pointer = digit_pointer->next;
			}
		}
		total = count + count2;
	} while(total < 50 || total > 58);
	
	// save mapping to key
	for (i = 0; i < remaining; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			if(key_pointer->letter == ' ') {
				key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = chosen[i];
				key_pointer->remaining -= 1;
				break;
			}
			
			key_pointer = key_pointer->next;
		}
	}
	
	// remove mapped digits from alphabet
	for (i=0; i < remaining; i++) {
		for (j=cipher_pointer->mapped; j<106; j++) {
			if(chosen[i] == cipher_pointer->alphabet[j]) {
				k = cipher_pointer->alphabet[cipher_pointer->mapped];
				cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[j];
				cipher_pointer->alphabet[j] = k;
				cipher_pointer->mapped++;
			}
		}
	}
	
	
	
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






int check_guess(struct digit *digit_head, struct key *key_head, int *ciphertext) {
	int i, j;
	int l, wc, length, total;
	
	int average;
	int std_dev;
	
	struct digit *digit_pointer;
	struct key *key_pointer;
	
	l = 0;
	wc = 0;
	total = 0;
	length = 0;
	
	// check if a digit is a space
	for(i=0; i<500; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			
			for(j=0; j < key_pointer->frequency - key_pointer->remaining; j++) {
				if(key_pointer->digits[j] == ciphertext[i]) {
					if(key_pointer->letter == ' ') {
						wc += 1;
						
						length = i - l + 1;
						total += length;
						l = i;
						
					}
				}
			}
			key_pointer = key_pointer->next;
		}
	}
	average = total/wc;
	
	l = 0;
	wc = 0;
	total = 0;
	length = 0;
	
	// check if a digit is a space
	for(i=0; i<500; i++) {
		
		key_pointer = key_head;
		while(key_pointer != NULL) {
			
			for(j=0; j < key_pointer->frequency - key_pointer->remaining; j++) {
				if(key_pointer->digits[j] == ciphertext[i]) {
					if(key_pointer->letter == ' ') {
						wc += 1;
						
						length = i - l + 1;
						total += pow((length-average),2);
						l = i;
						
					}
				}
			}
			key_pointer = key_pointer->next;
		}
	}
	std_dev = total/wc;
	
	if(sqrt((float)std_dev) < 6) {
		return 1;
	}
	return 0;
}






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





int guess_space_keys(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer, int *ciphertext) {
	int i;
	int check;
	int *space_seeds;
	
	struct key *key_pointer;
	
	struct cipher_alphabet *temp;
	
	
	space_seeds = get_equivalence_classes(digit_head, key_head, cipher_pointer);
	
	for(i=0; i<2; i++){
		get_equivalences(digit_head, key_head, cipher_pointer, *(space_seeds+i));
	}
	
	fill_out_space(digit_head, key_head, cipher_pointer);
	
	check = check_guess(digit_head, key_head, ciphertext);
	
	return check;
	
}






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                GUESS FREQUENT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/




void guess_frequent_keys(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	char frequentletters[5] = {'c', 'b', 'l', 's', 'u'};
	
	int i, j, k, l;
	
	int counter;
	int temp;
	int count;
	int size;
	int x_value;
	
	int candidates[80];
	int list[20];
	int chosen[20];
	
	struct digit *digit_pointer;
	struct key * key_pointer;
	
	
	
	
	// Guess digits per letter
	for(i=0; i<5; i++) {
		
		// Get candidates
		j=10;
		
		do{
			
			counter = 0;
			j--;
			
			digit_pointer = digit_head;
			while(digit_pointer != NULL) {
				
				if(digit_pointer->count > j) {
					
					for(k=cipher_pointer->mapped; k<106; k++) {
						
						if(cipher_pointer->alphabet[k] == digit_pointer->digit) {
							
							candidates[counter] = digit_pointer->digit;
							counter++;
							break;
							
						}
						
					}
					
				}
				
				digit_pointer = digit_pointer->next;
			}
		} while(counter<20);
		
		
		// get letters stats
		for(j=0; j<27; j++) {
			
			if(frequentletters[i] == english_letters[j]) {
				
				x_value = expected_value[j];
				
			}
		}
		
		// get remaining digits for letter from its key
		key_pointer = key_head;
		while(key_pointer != NULL) {
			if(key_pointer->letter == frequentletters[i]) {
				size = key_pointer->remaining;
			}
			key_pointer = key_pointer->next;
		}
		
		
		// Guess digits
		do {
			for(j=0; j < counter; j++) {
				list[j] = candidates[j];
			}
			
			for (j = 0; j < size; j++) {
				k = j + rand() % (counter - j);
				temp = list[j];
				list[j] = list[k];
				list[k] = temp;
				
				chosen[j] = list[j];
			}
			
			for (j = 0; j < size; j++) {
				
				digit_pointer = digit_head;
				while(digit_pointer != NULL) {
					
					if(chosen[j] == digit_pointer->digit) {
						
						count += digit_pointer->count;
						
					}
					
					digit_pointer = digit_pointer->next;
				}
			}
			
			
		} while(count < x_value - DEVIATION && count > x_value + DEVIATION);
		
		
		// save mapping to key
		for (j = 0; j < size; j++) {
			
			key_pointer = key_head;
			while(key_pointer != NULL) {
				if(key_pointer->letter == frequentletters[i]) {
					key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = chosen[j];
					key_pointer->remaining -= 1;
					break;
				}
				
				key_pointer = key_pointer->next;
			}
		}
		
		// remove mapped digits from alphabet
		for (j=0; j < size; j++) {
			for (k=cipher_pointer->mapped; k<106; k++) {
				if(chosen[j] == cipher_pointer->alphabet[k]) {
					l = cipher_pointer->alphabet[cipher_pointer->mapped];
					cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[k];
					cipher_pointer->alphabet[k] = l;
					cipher_pointer->mapped++;
				}
			}
		}
		
	}
	
}




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                GUESS INFREQUENT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/




void guess_infrequent_keys(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	char infrequentletters[8] = {'y', 'k', 'q', 'f', 'z', 'h', 'w', 'x'};
	
	int i, j, k, l;
	
	int counter;
	int temp;
	
	int count;
	int size;
	int x_value;
	
	int candidates[80];
	int list[20];
	int chosen[20];
	
	struct digit *digit_pointer;
	struct key * key_pointer;
	
	
	// for each letter
	for(i=0; i<8; i++) {
		
		// Get candidates
		j=1;
		do{
			
			counter = 0;
			j++;
			
			digit_pointer = digit_head;
			while(digit_pointer != NULL) {
				
				if(digit_pointer->count < j) {
					
					for(k=cipher_pointer->mapped; k<106; k++) {
						
						if(cipher_pointer->alphabet[k] == digit_pointer->digit) {
							
							candidates[counter] = digit_pointer->digit;
							counter++;
							break;
							
						}
						
					}
					
				}
				
				digit_pointer = digit_pointer->next;
			}
		} while(counter<20);
		
		
		
		// get letters stats
		for(j=0; j<27; j++) {
			
			if(infrequentletters[i] == english_letters[j]) {
				
				x_value = expected_value[j];
				
			}
		}
		
		// get remaining from key
		key_pointer = key_head;
		while(key_pointer != NULL) {
			
			if(key_pointer->letter == infrequentletters[i]) {
				size = key_pointer->remaining;
				
			}
			key_pointer = key_pointer->next;
		}
		
		
		
		// Guess digits
		do {
			for(j=0; j < counter; j++) {
				list[j] = candidates[j];
			}
			
			for (j = 0; j < size; j++) {
				k = j + rand() % (counter - j);
				temp = list[j];
				list[j] = list[k];
				list[k] = temp;
				
				chosen[j] = list[j];
			}
			
			for (j = 0; j < size; j++) {
				
				digit_pointer = digit_head;
				while(digit_pointer != NULL) {
					
					if(chosen[j] == digit_pointer->digit) {
						
						count += digit_pointer->count;
						break;
						
					}
					
					digit_pointer = digit_pointer->next;
				}
			}
			
			
		} while(count < x_value - DEVIATION && count > x_value + DEVIATION);
		
		
		// save mapping to key
		for (j = 0; j < size; j++) {
			
			key_pointer = key_head;
			while(key_pointer != NULL) {
				if(key_pointer->letter == infrequentletters[i]) {
					key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = chosen[j];
					key_pointer->remaining -= 1;
					break;
				}
				
				key_pointer = key_pointer->next;
			}
		}
		
		// remove mapped digits from alphabet
		for (j=0; j < size; j++) {
			for (k=cipher_pointer->mapped; k<106; k++) {
				if(chosen[j] == cipher_pointer->alphabet[k]) {
					l = cipher_pointer->alphabet[cipher_pointer->mapped];
					cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[k];
					cipher_pointer->alphabet[k] = l;
					cipher_pointer->mapped++;
				}
			}
		}
	}
}






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                GUESS REST FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/




void guess_rest_keys(struct digit *digit_head, struct key *key_head, struct cipher_alphabet *cipher_pointer) {
	int i, j, k, l;
	
	int counter;
	int count;
	int temp;
	int size;
	int x_value;
	
	int candidates[100];
	int list[20];
	int chosen[20];
	
	struct digit *digit_pointer;
	struct key * key_pointer;
	
	// for letters
	for(i=0; i<27; i++) {
		
		// Get candidates
		counter = 0;
		
		digit_pointer = digit_head;
		while(digit_pointer != NULL) {
			
			for(j=cipher_pointer->mapped; j<106; j++) {
				
				if(cipher_pointer->alphabet[j] == digit_pointer->digit) {
					
					candidates[counter] = digit_pointer->digit;
					counter++;
					break;
					
				}
				
			}
			
			digit_pointer = digit_pointer->next;
		}
		
		
		
		// Guess digits per letter
		x_value = expected_value[i];
		
		
		// get size of key space from key
		key_pointer = key_head;
		while(key_pointer != NULL) {
			
			if(key_pointer->letter == english_letters[i]) {
				
				size = key_pointer->remaining;
				
			}
			key_pointer = key_pointer->next;
		}
		
		
		
		// Guess digits
		do {
			
			// copy candidates into list
			for(j=0; j < counter; j++) {
				list[j] = candidates[j];
			}
			
			// pick enough digits
			for (j = 0; j < size; j++) {
				k = j + rand() % (counter - j);
				temp = list[j];
				list[j] = list[k];
				list[k] = temp;
				
				chosen[j] = list[j];
			}
			
			// count frequecy of digits assigned to letter to make sure they fall in proper range
			for (j = 0; j < size; j++) {
				
				digit_pointer = digit_head;
				while(digit_pointer != NULL) {
					
					if(chosen[j] == digit_pointer->digit) {
						
						count += digit_pointer->count;
						break;
						
					}
					digit_pointer = digit_pointer->next;
				}
			}
			
			
		} while(count < x_value - DEVIATION && count > x_value + DEVIATION);
		
		
		// save mapping to key
		for (j = 0; j < size; j++) {
			
			key_pointer = key_head;
			while(key_pointer != NULL) {
				if(key_pointer->letter == english_letters[i]) {
					key_pointer->digits[key_pointer->frequency - key_pointer->remaining] = chosen[j];
					key_pointer->remaining -= 1;
					break;
				}
				
				key_pointer = key_pointer->next;
			}
		}
		
		// remove mapped digits from alphabet
		for (j=0; j < size; j++) {
			for (k=cipher_pointer->mapped; k<106; k++) {
				if(chosen[j] == cipher_pointer->alphabet[k]) {
					l = cipher_pointer->alphabet[cipher_pointer->mapped];
					cipher_pointer->alphabet[cipher_pointer->mapped] = cipher_pointer->alphabet[k];
					cipher_pointer->alphabet[k] = l;
					cipher_pointer->mapped++;
				}
			}
		}
	}
}



/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                SIMPLEDIGIT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






// ******[ simpledigit_make ]******
struct simpledigit* simpledigit_make(struct simpledigit *head, int *cipherdigits, int index) {
	struct simpledigit *entry;
	
	entry = malloc(sizeof(struct simpledigit));
	entry->digit = cipherdigits[index];
	entry->count = 1;
	entry->next = head;
	head = entry;
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






// ******[ simpledigit_handler ]******
struct simpledigit* simpledigit_handler(struct simpledigit *head, int *cipherdigits, int index) {
	struct simpledigit *entry;
	
	entry = head;
	
	// Check if digit in digit linked list
	while(entry != NULL) {
		// if it is found
		if (entry->digit == cipherdigits[index]) {
			
			// increment its count by 1
			entry->count += 1;
			
			return head;
		} else {
			entry = entry->next;
		}
	}
	
	// Build a new digit linked list entry
	head = simpledigit_make(head, cipherdigits, index);
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/








/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                DIGIT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






// ******[ digit_make ]******
struct digit* digit_make(struct digit *head, int *cipherdigits, int index) {
	struct digit *entry;
	
	entry = malloc(sizeof(struct digit));
	entry->digit = cipherdigits[index];
	entry->count = 1;
	entry->left_digit_head = simpledigit_make(entry->left_digit_head, cipherdigits, index-1);
	entry->right_digit_head = simpledigit_make(entry->right_digit_head, cipherdigits, index+1);
	entry->next = head;
	head = entry;
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






// ******[ digit_handler ]******
struct digit* digit_handler(struct digit *head, int *cipherdigits, int index) {
	struct digit *entry;
	
	
	entry = head;   // point to the beginning of the digit linked list
	
	
	// Check if digit in digit linked list
	while(entry != NULL) {
		if (entry->digit == cipherdigits[index]) {      // if digit has been found
			// increment its count by 1
			entry->count += 1;
			
			if (index == 0) {
				entry->right_digit_head = simpledigit_handler(entry->right_digit_head, cipherdigits, index+1);
			} else if (index == 499) {
				entry->left_digit_head = simpledigit_handler(entry->left_digit_head, cipherdigits, index-1);
			} else {
				entry->left_digit_head = simpledigit_handler(entry->left_digit_head, cipherdigits, index-1);
				entry->right_digit_head = simpledigit_handler(entry->right_digit_head, cipherdigits, index+1);
			}
			return head;
		} else {                                        // else check the next entry
			entry = entry->next;
		}
	}
	// The while loop has ended without returning
	
	// Build a new digit linked list entry
	head = digit_make(head, cipherdigits, index);
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                PAIR FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


// ******[ pair_make ]******
struct pair* pair_make(struct pair *head, int *cipherdigits, int index) {
	struct pair *entry;
	
	entry = malloc(sizeof(struct digit));
	entry->pair[0] = cipherdigits[index];
	entry->pair[1] = cipherdigits[index+1];
	entry->count = 1;
	entry->next = head;
	head = entry;
	return head;
}






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






// ******[ pair_handler ]******
struct pair* pair_handler(struct pair *head, int *cipherdigits, int index) {
	struct pair *entry;
	
	
	entry = head;   // point to the beginning of the digit linked list
	
	
	// Check if digit in digit linked list
	while(entry != NULL) {
		// if digit has been found
		if (entry->pair[0] == cipherdigits[index] && entry->pair[1] == cipherdigits[index +1]) {
			// increment its count by 1
			entry->count += 1;
			return head;
		} else {                                        // else check the next entry
			entry = entry->next;
		}
	}
	// The while loop has ended without returning
	
	// Build a new digit linked list entry
	head = pair_make(head, cipherdigits, index);
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/







/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                ORDERING FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


// ******[ frequent_cipherdigits ]******
int* frequent_cipherdigits(struct digit *head) {
	int i;
	
	int max;
	int previous_max;
	
	static int ordered[120] = {0};
	
	struct digit *entry;
	
	i = 0;
	max = 0;
	previous_max = 501;
	
	// loop until all digits have been seen
	while (max  > -1) {
		max = 0;
		
		// find the max
		entry = head;
		while(entry != NULL) {
			if (entry->count > max && entry->count < previous_max) {
				max = entry->count;
			}
			entry = entry->next;
		}
		
		// if max found -> find every cipherdigit whose count = max
		if (max != 0) {
			
			// loop through entire digit linked list
			entry = head;
			while(entry != NULL) {
				
				if (entry->count == max) {
					ordered[i] = entry->digit;
					i += 1;
				}
				
				entry = entry->next;
			}
			
			// update previous_max so that on next iteration we find the next largest #
			previous_max = max;
			
			// if no max found -> Done
		} else if (max == 0) {
			return ordered;
		}
	}
	
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ frequent_simpledigits ]******
int* frequent_simpledigits(struct simpledigit *head) {
	int i;
	
	int max;
	int previous_max;
	
	static int ordered[50];
	
	struct simpledigit *entry;
	
	
	max = 0;
	previous_max = 501;
	
	for (i=0; i<50; i++) {
		ordered[i] = 0;
	}
	i = 0;
	
	// loop until all digits have been seen
	while (max  > -1) {
		max = 0;
		
		// find the max
		entry = head;
		while(entry != NULL) {
			if (entry->count > max && entry->count < previous_max) {
				max = entry->count;
			}
			entry = entry->next;
		}
		
		// if max found -> find every digit whose count = max
		if (max != 0) {
			
			// loop through entire digit linked list
			entry = head;
			while(entry != NULL) {
				
				if (entry->count == max) {
					ordered[i] = entry->digit;
					i += 1;
				}
				
				entry = entry->next;
			}
			
			// update previous_max so that on next iteration we find the next largest #
			previous_max = max;
			
			// if no max found -> Done
		} else if (max == 0) {
			return ordered;
		}
	}
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/







/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                EQUIVALENCE
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


// ******[build_equivalence_classes]******
void build_equivalence_classes(struct digit *head) {
	int i;
	
	struct digit *entry;
	struct digit *other;
	
	struct simpledigit *simpledigit_entry;
	struct simpledigit *simpledigit_other;
	
	struct equivalence *equivalence_head;
	struct equivalence *equivalence_entry;
	
	int equivalence, difference;
	
	
	
	// calculate scores
	
	
	// point to the beginning of the digit linked list
	entry = head;
	while(entry != NULL) {
		
		// Initialize the entries equivalence structure
		equivalence_head = NULL;
		
		// have a second pointer to the beginning of list
		other = head;
		while(other != NULL) {
			
			// initialize scores for (entry, other) pair
			equivalence = 0;
			difference = 0;
			
			//  if they are pointing to different objects
			if(entry->digit != other->digit) {
				
				
				
				//            ***[CALCULATE DIFFERENCE SCORE]***
				
				
				//      [+] LEFT [+]
				
				// loop through entries preceding digits
				simpledigit_entry = entry->left_digit_head;
				while(simpledigit_entry != NULL) {
					
					// loop through others preceding digits
					simpledigit_other = other->left_digit_head;
					while(simpledigit_other != NULL) {
						
						// if they share a preceding digit go to next
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							break;
						}
						simpledigit_other = simpledigit_other->next;
						
						// if end of linked list reached then the two do not share the digit
						if (simpledigit_other == NULL) {
							difference += 1;
						}
					}
					simpledigit_entry = simpledigit_entry->next;
				}
				
				// loop through entries preceding digits
				simpledigit_other = other->left_digit_head;
				while(simpledigit_other != NULL) {
					
					// loop through others preceding digits
					simpledigit_entry = entry->left_digit_head;
					while(simpledigit_entry != NULL) {
						
						// if they share a preceding digit go to next
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							break;
						}
						simpledigit_entry = simpledigit_entry->next;
						
						// if end of linked list reached then the two do not share the digit
						if (simpledigit_entry == NULL) {
							difference += 1;
						}
					}
					simpledigit_other = simpledigit_other->next;
				}
				
				
				//      [+] RIGHT [+]
				
				// loop through entries preceding digits
				simpledigit_entry = entry->right_digit_head;
				while(simpledigit_entry != NULL) {
					
					// loop through others preceding digits
					simpledigit_other = other->right_digit_head;
					while(simpledigit_other != NULL) {
						
						// if they share a preceding digit
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							break;
						}
						simpledigit_other = simpledigit_other->next;
						
						
						if (simpledigit_other == NULL) {
							difference += 1;
						}
					}
					simpledigit_entry = simpledigit_entry->next;
				}
				
				// loop through entries preceding digits
				simpledigit_other = other->right_digit_head;
				while(simpledigit_other != NULL) {
					
					// loop through others preceding digits
					simpledigit_entry = entry->right_digit_head;
					while(simpledigit_entry != NULL) {
						
						// if they share a preceding digit
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							break;
						}
						simpledigit_entry = simpledigit_entry->next;
						
						
						if (simpledigit_entry == NULL) {
							difference += 1;
						}
					}
					simpledigit_other = simpledigit_other->next;
				}
				
				
				
				//          ***[CALCULATE EQUIVALENCE SCORE]***
				
				
				//      [+] LEFT [+]
				
				// loop through entries preceding digits
				simpledigit_entry = entry->left_digit_head;
				while(simpledigit_entry != NULL) {
					
					// loop through others preceding digits
					simpledigit_other = other->left_digit_head;
					while(simpledigit_other != NULL) {
						
						// if they share a preceding digit
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							
							// count how many of that digit they share
							if(simpledigit_entry->count < simpledigit_other->count) {
								equivalence += simpledigit_entry->count;
							} else {
								equivalence += simpledigit_other->count;
							}
							break;
						}
						simpledigit_other = simpledigit_other->next;
					}
					simpledigit_entry = simpledigit_entry->next;
				}
				
				
				//      [+] RIGHT [+]
				
				// loop through entries subsequent digits
				simpledigit_entry = entry->right_digit_head;
				while(simpledigit_entry != NULL) {
					
					// loop through others subsequent digits
					simpledigit_other = other->right_digit_head;
					while(simpledigit_other != NULL) {
						
						// if they share a subsequent digit
						if(simpledigit_other->digit == simpledigit_entry->digit) {
							
							// count how many they share
							if(simpledigit_entry->count < simpledigit_other->count) {
								equivalence += simpledigit_entry->count;
							} else {
								equivalence += simpledigit_other->count;
							}
							break;
						}
						simpledigit_other = simpledigit_other->next;
					}
					simpledigit_entry = simpledigit_entry->next;
				}
				
				
				//          ***[BUILD EQUIVALENCE ENTRY]***
				
				equivalence_entry = malloc(sizeof(struct equivalence));
				equivalence_entry->digit = other->digit;
				equivalence_entry->positive = equivalence;
				equivalence_entry->negative = difference;
				equivalence_entry->next = entry->equivalence_head;
				entry->equivalence_head = equivalence_entry;
				
			}
			
			other = other->next; // check the next other->digit
		}
		entry = entry->next;  // go to the next entry->digit
	}
}




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/










int main(void) {
	srand(time(NULL));
	
	int i, j, k, x;
	int l, r;
	int l1, r1;
	int status;
	
	int *cipher_text;
	char message[SIZE];
	
	char word[100];
	char word1[100];
	int index;
	int index1;
	
	int score;
	int tries;
	
	int killswitch;
	
	char ch1, ch2;
	
	char *filename;
	char *rawcipher = malloc(600 * sizeof(char*));
	char *dictionary = malloc(100000 * sizeof(char*));
	
	struct key *key_head;
	struct key *key_entry;
	struct key *temp;
	
	struct digit *digit_head = NULL;
	struct pair *pair_head = NULL;
	
	struct digit *digit_entry, *digit_entry_alternate;
	struct pair *pair_entry;
	
	struct equivalence *equivalence_entry;
	struct simpledigit *simpledigit;
	
	struct cipher_alphabet *cipher_alphabet_pointer= NULL;
	
	cipher_alphabet_pointer = malloc(sizeof(struct cipher_alphabet));
	
	
	
	
	
	
	// read words from dictionary
	dictionary = read("words.txt");
	
	// Read in ciphertext
	printf("[break]> Make sure the cipher .txt file is in the same directory as this !!!\n");
	printf("[break]> Make sure the cipher .txt file is terminated with a ',' !!!\n");
	printf("[break]> If program fails just keep running until it works !!!\n\n");
	filename = input("[break]> Ciphertext Filename $> ");
	
	rawcipher = read(filename);
	cipher_text = intcsv(rawcipher);
	
	
	
	
	
	// analyze digits
	for (i=0; i<499; i++) {
		digit_head = digit_handler(digit_head, cipher_text, i);
	}
	
	
	// analyze pairs
	for (i=0; i<498; i++) {
		pair_head = pair_handler(pair_head, cipher_text, i);
	}
	
	
	// build equivalence classes
	build_equivalence_classes(digit_head);
	
	
	
	
	
	// guess some messages
	tries = 0;
	do {
		printf("\n\nTrying Key %d\n", tries);
		tries += 1;
		score = 0;
		
		
		

		
		
		// guess a key
		killswitch = 0;
		do {
			killswitch++;
			
			// reset cipher alphabet
			for(i=0; i<106; i++) {
				cipher_alphabet_pointer->alphabet[i] = i;
			}
			cipher_alphabet_pointer->mapped = 0;
			
			
			
			// if key exists reset it
			if(key_head != NULL) {
				key_entry = key_head;
				
				while(key_entry != NULL) {
					temp = key_entry;
					key_entry = key_entry->next;
					free(temp);
				}
			}
			
			
			// build key
			key_head = NULL;
			
			for(i=0; i<27; i++) {
				key_head = makekey(key_head,english_letters[i], frequency[i]);
			}
			
			assign_zero_digits(digit_head, key_head, cipher_alphabet_pointer);
			
			status = guess_space_keys(digit_head, key_head, cipher_alphabet_pointer, cipher_text);
			
			if(killswitch == 10) {
				break;
			}
		} while(status == 0);
		
		
		guess_frequent_keys(digit_head, key_head, cipher_alphabet_pointer);
		
		guess_infrequent_keys(digit_head, key_head, cipher_alphabet_pointer);
		
		guess_rest_keys(digit_head, key_head, cipher_alphabet_pointer);
		
		
		
		// decrypt message
		for (i=0; i<SIZE; i++) {
			message[i] = decrypt_digit(key_head, cipher_text[i]);
		}

		
		
		// Score
		
		l =0;
		l1 = 0;
		
		// loop through gussed message
		for (i=0; i<SIZE; i++) {
			
			if(message[i] == ' ') {
				
				r = i;
				
				index = 0;
				
				// geting each word from guessed message
				for(j=l; j<r; j++) {
					
					word[index] = message[j];
					index++;
					
				}
				word[index] = '\0';
				
				// loop through dictionary
				for (k=0; k<1000000; k++) {
					
					if(dictionary[k] == '\n') {
						
						r1 = k;
						
						index1 = 0;
						
						// getting each word from dictionary
						for(x=l1; x<r1; x++) {
							
							word1[index1] = dictionary[x];
							index1++;
							
						}
						word1[index1] = '\0';
						
						if(strlen(word)>3) {
							if(strcmp(word, word1) == 0) {
								printf("Found: %s\n", word);
								score += 1;
							}
						}
						
						memset(word1, 0, sizeof(word1));
						
						l1 = r1 + 1;
					}
				}
				
				// if equal score + 1
				// if score > threshold -> rememeber key
				
				memset(word, 0, sizeof(word));
				
				l = r + 1;
				
			}
		}
	} while (score < 2);
	
	
	// If success
	printf("Passed!\n");
	
	// Print decrypted message
	printf("\nPlaintext:\n%s\n\n", message);
	
	// Print key
	key_entry = key_head;
	while(key_entry != NULL) {
		printf("%c\n[", key_entry->letter);
		for(i=0; i < key_entry->frequency; i++) {
			printf("%d,", key_entry->digits[i]);
		}
		printf("]\n");
		key_entry	= key_entry->next;
	}



	return 0;
}

