#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arrays.h"




/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


struct key {
	char letter;
	int digits[25];
	int size;
	int index;
	
	struct key* next;
};


/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/








/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                KEY FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ make keyspace ]******

struct key* make_keyspace(struct key* head, char letter, int size) {
	int i;
	
	struct key* entry;
	entry = malloc(sizeof(struct key));
	
	entry->letter = letter;
	entry->size = size;
	entry->index = 0;
	for(i=0; i<25; i++) {
		entry->digits[i] = -1;
	}
	
	// Point this instance to digit structure
	entry->next = head;
	// And make this instance the head of the digit structure
	head = entry;
	
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ reset keyspace ]******

struct key* reset_keyspace(struct key* head) {
	int i;
	
	struct key* entry;
	struct key* temp;

	if(head != NULL) {
		entry = head;
		
		while(entry != NULL) {
			temp = entry;
			entry = entry->next;
			free(temp);
		}
		head = NULL;
	}
	
	for(i=0; i<27; i++) {
		head = make_keyspace(head, english_letters[i], frequency[i]);
	}
	
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ reset key entry ]******

void reset_key_entry(struct key* head, char letter) {
	int i;

	struct key* entry;

	entry = head;
	while(entry != NULL) {
		
		if(entry->letter == letter) {
			
			entry->index = 0;
			for(i=0; i<25; i++) {
				entry->digits[i] = -1;
			}
			break;
		}
		
		entry = entry->next;
	}

}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ write key entry ]******

void write_key_entry(struct key* head, char letter, int* digits, int size) {
	int i;
	
	struct key* entry;
	
	entry = head;
	while(entry != NULL) {
		
		if(entry->letter == letter) {
			
			for(i=0; i<size; i++) {
				entry->digits[i] = digits[i];
			}
			entry->index += size;
		}
	}

	
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ display keyspace ]******

void display_keyspace(struct key* head) {
	int i;
	
	struct key* entry;
	
	entry = head;
	while(entry != NULL) {
		printf("%c\n",entry->letter);
		
		printf("\tDigits\n\t[");
		for(i=0; i<entry->size; i++) {
			printf(" %d,", entry->digits[i]);
		}
		printf("]\n");
		
		entry = entry->next;
	}
	
	
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





