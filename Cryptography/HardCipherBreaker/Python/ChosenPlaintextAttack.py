import random
import string
import math

letters = list(string.ascii_lowercase)
letters.append(' ')

counter_total_letters = [0]*27

counter_message_letters = [0]*27

expected_value_letters = [0]*27

variance_letters = [0]*27

words = []
message = []

length = 0
total = 0

message_length = 500

number_of_messages_to_make = 100000
progress_marker = number_of_messages_to_make/10




# Get dictionary words
f = open("../words.txt","r")
lines = f.readlines()
f.close()






# Get Averages
print("Calculating Averages...")
finished = 0
for i in range(0,number_of_messages_to_make):
	
	# Show completion status
	if(i%progress_marker==0):
		finished += 1
		print("{}% Done".format(round(finished * 10),1))
	
	# Zero out arrays that need to be reset
	words = []
	message = []
	counter_message_letters = [0]*27

	# Pick some words
	length = 0
	while(length < message_length + 100):
		rand = random.randint(0,109562)
		words.append(lines[rand][:-1])
		length += len(lines[rand])

	# Format it so message is of proper length
	length = 0
	for word in words:
		length += len(word) + 1
		if(length < message_length):
			message.append(word)
		if(length > message_length):
			extra = length - message_length
			padding = len(word)-extra
			message.append(word[padding:])
			break

	# Count each letter
	for word in message:
		for character in word:
			for letter in letters:
				if character == letter:
					counter_message_letters[letters.index(letter)] += 1

	# Calculate total for all charcter other than <space>
	total = 0
	i = 0
	for number in counter_message_letters:
		total += number
	
		counter_total_letters[i] += number
	
		i += 1

	# Use count to calculate number of space characters
	counter_message_letters[26] = message_length - total
	counter_total_letters[26] += counter_message_letters[26]






# Record averages
for i in range(0,27):
	expected_value_letters[i] = counter_total_letters[i]/number_of_messages_to_make







# Calculate standard deviation
finished = 0
print("\nCalculating Standard Deviation...")
for i in range(0,number_of_messages_to_make):
	
	# Show completion status
	if(i%progress_marker==0):
		finished += 1
		print("{}% Done".format(round(finished * 10),1))
	
	# Zero out arrays that need to be reset
	words = []
	message = []
	counter_message_letters = [0]*27
	
	# Pick some words
	length = 0
	while(length < message_length + 100):
		rand = random.randint(0,109562)
		words.append(lines[rand][:-1])
		length += len(lines[rand])
	
	# Format it so message is of proper length
	length = 0
	for word in words:
		length += len(word) + 1
		if(length < message_length):
			message.append(word)
		if(length > message_length):
			extra = length - message_length
			padding = len(word)-extra
			message.append(word[padding:])
			break

	# Count each letter
	for word in message:
		for character in word:
			for letter in letters:
				if character == letter:
					counter_message_letters[letters.index(letter)] += 1

	# Calculate variance for all charcter other than <space>
	# and keep count of how many non-<space> character
	total = 0
	i = 0
	for number in counter_message_letters:
		total += number
		
		if i == 26:
			break
		variance_letters[i] += math.pow(counter_message_letters[i] - expected_value_letters[i], 2)
		
		i += 1

	# Use count to calculate number of space characters
	# and then calculate <space> variance
	counter_message_letters[26] = message_length - total
	variance_letters[26] += math.pow(counter_message_letters[26] - expected_value_letters[26], 2)











# Show results of tests
for i in range(0,27) :
	print("{}: \tX= {} \tStDev= {}".format(letters[i],
																				 expected_value_letters[i],
																				 round(math.sqrt(variance_letters[i]/number_of_messages_to_make), 2)
																				 ))











