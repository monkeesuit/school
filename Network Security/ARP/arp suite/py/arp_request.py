import arp_source as arp

def send(requestedIP):
	# Sends an ARP Request to the requestedIP
	# requestedIP = The IP Address You Wish to ARP (Resolve MAC Address)
	sender = arp.arpclient('eth0') # Create an instance of arpclient found in 																			arp_source.py
	sender.request(requestedIP) # Call the request function of arpclient
