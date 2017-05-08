import random

words = []
towrite = []

f = open("words.txt","r")
lines = f.readlines()
f.close()

length = 0
while(length < 600):
	rand = random.randint(0,109562)
	words.append(lines[rand][:-1])
	length += len(lines[rand])


length = 0
for word in words:
	length += len(word)
	if(length < 500):
		towrite.append(word)
	if(length > 500):
		extra = length -500
		padding = len(word)-extra
		towrite.append(word[padding:])
		break

f = open("messsage5.txt","w")
f.write(" ".join(towrite))
f.close()




