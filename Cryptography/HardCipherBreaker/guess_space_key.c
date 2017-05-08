#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

struct digit {
	int digit;
	int count;
	int index[50];
	char letter;
	
	struct digit* next;
};

/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
int* test_chosen(struct digit* head, int* chosen) {
	int i, j;
	
	int* success;
	int* fail;
	success[0] = 1;
	fail[0] = 0;
	
	int temp;
	int total = 0;
	int cursor = 0;
	int indicies[200];
	
	int character_count = 0;
	float average_word_length;
	int variance_count = 0;
	float variance;
	
	struct digit* entry;


	// Count Spaces
	
	for (i=0; i<19; i++) {
		entry = head;
		while (entry != NULL) {
			if (entry->digit == chosen[i]) {
			
				total += entry->count;
				
				if (entry->count > 0) {
					for (j=0; j<entry->count; j++) {
						indicies[cursor] = entry->index[j];
						cursor ++;
					}
				}
				
				break;
			}
			
			entry = entry->next;
		}
	}
	
	
	// Sort Indicies
	
	for (i=0; i < (cursor-1); i++) {
		for (j=0; j < (cursor - i - 1); j++) {
			if (indicies[j] > indicies[j+1]) {
			
				temp = indicies[j];
				indicies[j] = indicies[j+1];
				indicies[j+1] = temp;
			}
		}
	}
	printf("\nSorted \n[");
	for (i=0; i < (cursor-1); i++) {
		printf(" %d,", indicies[i]);
	}
	printf("]\n");
	
	
	// Calculate Average
	
	if (indicies[0] != 0) {
		for (i=0; i < (cursor-1); i++) {
		
			if (i == 0) {
				character_count += indicies[0];
			} else if (i != 0) {
				character_count += indicies[i] - indicies[i-1] - 1;
				
				// Check that we haven't picked consecutive characters to be spaces
				if (indicies[i] - indicies[i-1] - 1 == 0) {
					printf("Consecutive Character\n");
					return fail;
				}
				
			}
		}
		character_count += 499 - indicies[cursor-1];
		average_word_length	= character_count/cursor;
	// Check that we havent picked the first character to be a space
	} else {
		printf("First Character\n");
		return fail;
	}
	printf("\n Calculated Average\n");
	
	
	// Calculate Variance
	
	for (i=0; i < (cursor-1); i++) {
		if (i == 0) {
			variance_count += pow(indicies[0] - average_word_length, 2);
		} else if (i != 0) {
			variance_count += pow(indicies[i] - indicies[i-1] - 1 - average_word_length, 2);
		}
	}
	variance_count += pow(499 - indicies[cursor-1] - average_word_length, 2);
	variance = variance_count/cursor;
	printf("\n Calculated Variance\n");
	
	printf("\nTotal = %d\n", total);
	printf("\nAverage = %f\n", average_word_length);
	printf("\nStDev = %f\n", sqrt(variance));
	
	
	// Do checks
	
	// Check that there is a reasonable amount of <spaces>
	if (total >= 45 && total <= 60) {} else {
		return fail;
	}
	
	// Check that the average word length is reasonable
	if (average_word_length >= 7 && average_word_length <= 12) {} else {
		return fail;
	}
	
	// Check that the variance of the word length is reasonable
	if (sqrt(variance) <= 4) {} else {
		return fail;
	}
	
	// If these three charactersitc are observed then this set passes
	return success;
}

/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
int* pick_candidates(int* candidates, int size, int amount, int already_chosen) {
	
	srand((unsigned) time(0));
	
	int i;
	int random;
	int temp;
	
	int* chosen = malloc(25 * sizeof(int*));
	
	for (i=0; i<amount; i++) {
		random = rand() % (size-i-already_chosen);
		
		chosen[i] = candidates[random+i+already_chosen];
		
		temp = candidates[random+i+already_chosen];
		candidates[random+i+already_chosen] = candidates[i+already_chosen];
		candidates[i+already_chosen] = temp;

	}
	
	return chosen;
}

/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
int* find_candidates(struct digit* head) {
	int i;
	
	int* chosen = malloc(20 * sizeof(int*));
	
	int* chosen1;
	int* chosen2;
	int* chosen3;
	
	i = 0;
	int* candidates = malloc(200 * sizeof(int*));
	
	struct digit* entry;
	
	entry = head;
	while (entry != NULL) {
		if (entry->count >= 2 && entry->count <= 4) {
			candidates[i] = entry->digit;
			i++;
		}
		entry = entry->next;
	}

	chosen1 = pick_candidates(candidates, i, 10, 0);
	
	i = 0;
	//int* candidates = malloc(200 * sizeof(int*));
	
	entry = head;
	while (entry != NULL) {
		if (entry->count >= 1 && entry->count <= 5) {
			candidates[i] = entry->digit;
			i++;
		}
		entry = entry->next;
	}

	chosen2 = pick_candidates(candidates, i, 5, 10);
	
	i = 0;
	//int* candidates = malloc(200 * sizeof(int*));
	
	entry = head;
	while (entry != NULL) {
		if (entry->count >= 0 && entry->count <= 6) {
			candidates[i] = entry->digit;
			i++;
		}
		entry = entry->next;
	}

	chosen3 = pick_candidates(candidates, i, 4, 15);
	
	for (i=0; i<10; i++) {
		chosen[i] = chosen1[i];
	}
	for (i=0; i<5; i++) {
		chosen[i+10] = chosen2[i];
	}
	for (i=0; i<4; i++) {
		chosen[i+15] = chosen3[i];
	}
	
	return chosen;
}

/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
