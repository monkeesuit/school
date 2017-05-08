


f = open("../words_stats.txt", 'r')

counter = 0

for lines in f:
	if counter == 27:
		break
	counter += 1

	size = float(lines.split('\t')[3].split()[1])
	expected_value = float(lines.split('\t')[1].split()[1])
	std_dev = float(lines.split('\t')[2].split()[1])

	cd_expected_value = round(expected_value/size, 2)
	cd_std_dev = round(std_dev/size, 2)



	print("{} \tcdX= {} \t cdStDev= {}".format(lines.strip(), cd_expected_value, cd_std_dev))






