


struct digit {
	int digit;
	int count;
	int index[50];
	char letter;
	
	struct digit* next;
};



/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/
//                                PARSE CIPHERTEXT FUNCTIONS
/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/

struct digit* make_digit(struct digit* head, int* cipherdigits, int index);
struct digit* parse_ciphertext(struct digit* head, int* ciphertext, int index);
struct digit* unused_digits(struct digit* head);
struct digit* lookup_digit(struct digit* head, int digit);
void display_digit_structure(struct digit* head);

/*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*/







