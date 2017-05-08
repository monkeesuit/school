from scapy.all import *
import random

###[ networkAddress ]###
def networkAddress(version=4):
	if version != 4 and version != 6:
		print('Not a valid IP Version...')
		print('Valid Versions are 4 or 6...')
		print('Default is 4...')
		
		return 0

	else:
		generated_IP_address = RandIP()._fix()
		generated_IP_address = str(generated_IP_address)

		if version == 4:
			
			return generated_IP_address

		elif version == 6:
			byte1 = random.randint(0,255)
			byte2 = random.randint(0,255)
			byte1 = str(byte1)
			byte2 = str(byte2)
			generated_IP_address = generated_IP_address+'.'+byte1+'.'+byte2

			return generated_IP_address
	
		else:

			return 0

###[ hardwareAddress ]###
def hardwareAddress():
	generated_mac_address = RandMAC()._fix()
	generated_mac_address = str(generated_mac_address)
	decoded_mac_address = generated_mac.replace(':','').decode('hex')
	
	return generated_mac_address, decoded_mac_address
