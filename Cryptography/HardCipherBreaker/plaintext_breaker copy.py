
# Parse ciphertext
def parse_ciphertext():

	ciphertext_characters = []

	ciphertext_filename = input("Enter Ciphertext Filename: ")

	ciphertext_file = open(ciphertext_filename)

	# Gives an indexed version of ciphertext
	for lines in ciphertext_file:
		ciphertext_characters = lines.split(',')

	return ciphertext_characters



#Parse each plaintext and try to build a key

for i in range(1,6):


	# Rebuild Key

	cipher_key = {
			'a': {'digits':[], 'size': 7},
			'b': {'digits':[], 'size': 1},
			'c': {'digits':[], 'size': 2},
			'd': {'digits':[], 'size': 4},
			'e': {'digits':[], 'size': 10},
			'f': {'digits':[], 'size': 2},
			'g': {'digits':[], 'size': 2},
			'h': {'digits':[], 'size': 5},
			'i': {'digits':[], 'size': 6},
			'j': {'digits':[], 'size': 1},
			'k': {'digits':[], 'size': 1},
			'l': {'digits':[], 'size': 3},
			'm': {'digits':[], 'size': 2},
			'n': {'digits':[], 'size': 6},
			'o': {'digits':[], 'size': 6},
			'p': {'digits':[], 'size': 2},
			'q': {'digits':[], 'size': 1},
			'r': {'digits':[], 'size': 5},
			's': {'digits':[], 'size': 5},
			't': {'digits':[], 'size': 7},
			'u': {'digits':[], 'size': 2},
			'v': {'digits':[], 'size': 1},
			'w': {'digits':[], 'size': 2},
			'x': {'digits':[], 'size': 1},
			'y': {'digits':[], 'size': 2},
			'z': {'digits':[], 'size': 1},
			' ': {'digits':[], 'size': 19}
			}


	# Get an indexed version of plaintext

	plaintext_characters = []
	plaintext_file = open("plaintexts.{}.txt".format(i))

	for lines in plaintext_file:
		for characters in lines:
			plaintext_characters.append(characters)


	# Find unseen digits

	unseen_digits = []

	for i in range(0,106):
		if str(i) not in ciphertext_characters:
			unseen_digits.append(i)


	# Loop through both files

	for i in range(0,500):
		letter = plaintext_characters[i]
		digit = ciphertext_characters[i]
		
		# Build key
		
		keyvalues = cipher_key.get(str(letter)).get('digits')
		if digit not in keyvalues:
			keyvalues.append(digit)
			cipher_key[str(letter)]['digit'] = keyvalues


	sizeok = 0
	consistencyok = 0

	# Check for size inconsistency

	missing = 0
	for key, value in cipher_key.items():
		if len(value['digits']) != (value['size']):
			missing += abs(len(value['digits']) - (value['size']))

	if missing == len(unseen_digits):
		sizeok == 1

	# Check for key consistency
	for key1, value1 in cipher_key.items():
		for key2, value2 in cipher_key.items():
			if key1 != key2:
				for x in value1['digits']:
					for y in value2['digits']:
						if x == y:




	plaintext_file.close()






ciphertext_characters = []
ciphertext_characters = parse_ciphertext()






















