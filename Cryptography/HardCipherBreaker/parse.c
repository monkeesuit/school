#include <stdio.h>
#include <string.h>
#include <stdlib.h>






/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/


struct digit {
	int digit;
	int count;
	int index[50];
	char letter;
	
	struct digit* next;
};


/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/








/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                PARSE CIPHERTEXT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ make digit ]******

struct digit* make_digit(struct digit* head, int cipherdigit, int index, int count) {
	
	// Allocate space for new instance of digit structure
	struct digit* entry;
	entry = malloc(sizeof(struct digit));
	
	// Build instance
	entry->digit = cipherdigit;
	entry->count = count;
	entry->index[0] = index;
	entry->letter = '\0';
	
	// Point this instance to digit structure
	entry->next = head;
	// And make this instance the head of the digit structure
	head = entry;
	
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ parse ciphertext ]******

struct digit* parse_ciphertext(struct digit* head, int* ciphertext, int index) {
	
	// Walk through digit structure looking for the digit found in the ciphertext
	struct digit* entry;
	entry = head;
	while(entry != NULL) {
		
		// If there is an instance of this digit in our digit structure record its info
		if (entry->digit == ciphertext[index]) {
			
			entry->index[entry->count] = index;
			entry->count += 1;

			return head;
			
		} else {
			entry = entry->next;
		}
	}
	// The while loop has completed - there is no instance of this digit in the digit structure
	
	// If there is no instance of this digit make a new instance for this digit
	head = make_digit(head, ciphertext[index], index, 1);
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ unused digits ]******

struct digit* unused_digits(struct digit* head) {
	// Loop through range of digits to see if they have been seen
	int i;
	struct digit* entry;

	for (i=0; i<107; i++) {
	
		// Walk the digit dictionary
		entry = head;
		while (entry != NULL) {
			if (i == entry->digit) {
				break;
			}
		entry = entry->next;
		}
		// If we reach the end of the dictionary without finding an instance make one
		if (entry == NULL) {
			head = make_digit(head, i, 0, 0);
		}
	}
	return head;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/




// ******[ FUNCTION ]******
// ******[ lookup digit ]******

struct digit* lookup_digit(struct digit* head, int digit) {
	
	// Walk through digit structure looking for the digit
	struct digit* entry;
	entry = head;
	while(entry != NULL) {
		
		if (entry->digit == digit) {
			
			return entry;
			
		} else {
			entry = entry->next;
		}
	}
	// The while loop has completed -
	// there is no instance of this digit in the digit structure
	
	// If there is no instance return NULL
	printf("Lookup Failed!!!\n\n");
	return NULL;
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/





// ******[ FUNCTION ]******
// ******[ display digit structure ]******

void display_digit_structure(struct digit* head) {
	int total = 0;
	
	struct digit* entry;
	entry = head;
	while(entry != NULL) {
		
		printf("Digit: %d\n", entry->digit);
		printf("Count: %d\n", entry->count);
		printf("\n");
		
		total += entry->count;

		entry = entry->next;
	}
	printf("Total: %d\n\n", total);
}





/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/







