

#$-------------------------------------------------------------------------------------$#
# Initialize Key
#	Returns an empty key

def init_key():

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
	
	return cipher_key


#$-------------------------------------------------------------------------------------$#
# Parse Ciphertext
#	Returns an indexed version of ciphertext

def parse_ciphertext():

	ciphertext_characters = []

	ciphertext_filename = input("Enter Ciphertext Filename: ")

	ciphertext_file = open(ciphertext_filename)

	# Gives an indexed version of ciphertext
	for lines in ciphertext_file:
		ciphertext_characters = lines.split(',')
	
	ciphertext_file.close()

	return ciphertext_characters



#$-------------------------------------------------------------------------------------$#
# Parse Plaintext
#	Return an indexed version of plaintext

def parse_plaintext(i):

	plaintext_characters = []
	plaintext_file = open("plaintexts.{}.txt".format(i))

	for lines in plaintext_file:
		for characters in lines:
			plaintext_characters.append(characters)

	plaintext_file.close()

	return plaintext_characters


#$-------------------------------------------------------------------------------------$#
# Find unseen digits
# 	Returns digits not in the ciphertext

def find_unseen_digits(ciphertext_characters):

	unseen_digits = []

	for i in range(0,106):
		if str(i) not in ciphertext_characters:
			unseen_digits.append(i)

	return unseen_digits


#$-------------------------------------------------------------------------------------$#
# Build key
# 	Returns a key based on the plaintext

def build_key(plaintext_characters, ciphertext_characters, cipher_key):

	for i in range(0,500):
		letter = plaintext_characters[i]
		digit = ciphertext_characters[i]
		
		keyvalues = cipher_key.get(str(letter)).get('digits')
		if digit not in keyvalues:
			keyvalues.append(digit)
			cipher_key[str(letter)]['digits'] = keyvalues

	return cipher_key


#$-------------------------------------------------------------------------------------$#
# Check for size inconsistency
#	Check if every digit is mapped

def size_check(cipher_key, unseen_digits):

	sizeok = 0

	missing = 0
	for key, value in cipher_key.items():
		if len(value['digits']) != (value['size']):
			missing += abs(len(value['digits']) - (value['size']))

	if missing == len(unseen_digits):
		sizeok = 1

	return sizeok


#$-------------------------------------------------------------------------------------$#
# Check for key consistency
#	Check that no digit is mapped twice

def consistency_check(cipher_key):

	consistencyok = 1

	for key1, value1 in cipher_key.items():
		for key2, value2 in cipher_key.items():
			if key1 != key2:
				for x in value1['digits']:
					for y in value2['digits']:
						if x == y:
							consistencyok = 0

	return consistencyok





#$-------------------------------------------------------------------------------------$#
#$-------------------------------------------------------------------------------------$#
#$-------------------------------------------------------------------------------------$#
def run():

	ciphertext_characters = []
	ciphertext_characters = parse_ciphertext()

	# For each plaintext in the plaintext dictionary
	for i in range(1,6):

		cipher_key = {}
		cipher_key = init_key()

		plaintext_characters = []
		plaintext_characters = parse_plaintext(i)

		unseen_digits = []
		unseen_digits = find_unseen_digits(ciphertext_characters)

		cipher_key = build_key(plaintext_characters, ciphertext_characters, cipher_key)

		check1 = size_check(cipher_key, unseen_digits)
		check2 = consistency_check(cipher_key)

		if check1 == 1 & check2 == 1:
			print("Plaintext:")
			print(''.join(plaintext_characters))

if __name__ == "__main__":
	run()

























