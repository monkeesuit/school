import random
import math

def init_key():
	
	key = {
	'a':{'digits':[], 'size':7, 'xVal':33.99, 'xStDev':4.99},
	'b':{'digits':[], 'size':1, 'xVal':8.74, 'xStDev':2.98},
	'c':{'digits':[], 'size':2, 'xVal':18.05, 'xStDev':3.97},
	'd':{'digits':[], 'size':4, 'xVal':16.43, 'xStDev':3.93},
	'e':{'digits':[], 'size':10, 'xVal':51.95, 'xStDev':5.97},
	'f':{'digits':[], 'size':2, 'xVal':6.10, 'xStDev':2.61},
	'g':{'digits':[], 'size':2, 'xVal':12.91, 'xStDev':3.54},
	'h':{'digits':[], 'size':5, 'xVal':9.92, 'xStDev':3.05},
	'i':{'digits':[], 'size':6, 'xVal':39.20, 'xStDev':5.35},
	'j':{'digits':[], 'size':1, 'xVal':0.84, 'xStDev':0.93},
	'k':{'digits':[], 'size':1, 'xVal':3.91, 'xStDev':2.00},
	'l':{'digits':[], 'size':3, 'xVal':24.06, 'xStDev':4.62},
	'm':{'digits':[], 'size':2, 'xVal':12.25, 'xStDev':3.43},
	'n':{'digits':[], 'size':6, 'xVal':31.02, 'xStDev':4.89},
	'o':{'digits':[], 'size':6, 'xVal':27.38, 'xStDev':4.89},
	'p':{'digits':[], 'size':2, 'xVal':12.57, 'xStDev':3.53},
	'q':{'digits':[], 'size':1, 'xVal':0.82, 'xStDev':0.89},
	'r':{'digits':[], 'size':5, 'xVal':33.00, 'xStDev':4.86},
	's':{'digits':[], 'size':5, 'xVal':40.00, 'xStDev':5.77},
	't':{'digits':[], 'size':7, 'xVal':29.73, 'xStDev':4.77},
	'u':{'digits':[], 'size':2, 'xVal':14.91, 'xStDev':3.62},
	'v':{'digits':[], 'size':1, 'xVal':4.64, 'xStDev':2.12},
	'w':{'digits':[], 'size':2, 'xVal':3.83, 'xStDev':1.98},
	'x':{'digits':[], 'size':1, 'xVal':1.30, 'xStDev':1.13},
	'y':{'digits':[], 'size':2, 'xVal':7.64, 'xStDev':2.64},
	'z':{'digits':[], 'size':1, 'xVal':1.97, 'xStDev':1.47},
	' ':{'digits':[], 'size':19, 'xVal':52.82, 'xStDev':5.91}
	}

	return key





def init_cipher_dictionary():

	cipher_dictionary = {}

	for i in range(0,106):
		cipher_dictionary[i] = {'count':0 , 'positions':[], 'letter':''}

	return cipher_dictionary





def cipher_parse(cipher_dictionary):

	f = open('cipher.txt', "r")
	
	cipher = f.readline()
	cipher = cipher.split(',')
	
	pos = 0
	for characters in cipher:
		for i in range(0,106):
		
			if int(characters) == i:
				cipher_dictionary[i]['count'] += 1
				cipher_dictionary[i]['positions'].append(pos)
	
		pos += 1

	return cipher





def guess_spaces(cipher_dictionary):

	ok = 'false'
	
	while(ok == 'false'):
	
		chosen = []
		
		
		candidates = []
		
		for i in range(0,106):
		
			if cipher_dictionary[i]['count'] < 4 and cipher_dictionary[i]['count'] > 1:
				candidates.append(i)

		for toget in range(12,-1,-1):
			try:
				random_indexes = random.sample(range(0,len(candidates)), toget)
				break
			except ValueError:
				pass


		for elements in random_indexes:
			chosen.append(candidates[elements])



		candidates = []

		for i in range(0,106):
		
			if cipher_dictionary[i]['count'] < 7 and cipher_dictionary[i]['count'] >= 0 and i not in chosen:
				candidates.append(i)

		remaining = 19 - len(chosen)
		random_indexes = random.sample(range(0,len(candidates)), remaining)

		for elements in random_indexes:
			chosen.append(candidates[elements])




		total = 0
		space_index = []

		for elements in chosen:
			for i in range(0,106):
				if elements == i:
				
					total += cipher_dictionary[i]['count']
					
					for j in range(0,len(cipher_dictionary[i]['positions'])):
						space_index.append(cipher_dictionary[i]['positions'][j])



		if total > 46 and total < 60:
			test1 = 'pass'
		else:
			test1 = 'fail'

		space_index.sort()

		if space_index[0] != 0:
			test2 = 'pass'
		else:
			test2 = 'fail'

		test3 = 'pass'
		for i in range(0,len(space_index)-1):
			if space_index[i+1]-space_index[i] == 1:
				test3 = 'fail'
				break


		total_word_length = 0
		for i in range(0,len(space_index)-1):
			if i == 0:
				total_word_length += space_index[0]
			else:
				word_length = space_index[i+1]-space_index[i]-1
				total_word_length += word_length

		average_word_length = total_word_length/(len(space_index)+1)

		if average_word_length > 7 and average_word_length < 14:
			test4 = 'pass'
		else:
			test4 = 'fail'



		total_variation = 0.00
		for i in range(0,len(space_index)-1):
			if i == 0:
				total_variation += math.pow(space_index[0]-average_word_length, 2)
			else:
				word_length = space_index[i+1]-space_index[i]-1
				total_variation += math.pow(word_length-average_word_length, 2)

		stdev = math.sqrt(total_variation/(len(space_index)+1))
		if stdev < 6:
			test5 = 'pass'
		else:
			test5 = 'fail'



		if test1 == 'pass' and test2 == 'pass' and test3 == 'pass' and test4 == 'pass' and test5 == 'pass':
			ok = 'true'
			
			return chosen

		else:
			ok = 'false'





key = init_key()
cipher_dictionary = init_cipher_dictionary()

cipher = cipher_parse(cipher_dictionary)

chosen_spaces = guess_spaces(cipher_dictionary)
key[' ']['digits'] = chosen_spaces
# guess_low_variation
# guess_high_frequency
# guess_low_frequency
# guess_rest
