import arp_source as arp
from threading import Thread

def listen():
	# Listens for and handles ARP Messages
	number_workers = 5
	master = arp.arpclient('eth0')

	for i in range(number_workers): # Spawn 5 Workers to Handle Messages
		thread = Thread(target=master.worker) # worker() function of arpclient
		thread.daemon = True
		thread.start()

	master.listen() # call arpclient listen() function

