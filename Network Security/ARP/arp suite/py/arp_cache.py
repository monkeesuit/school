import arp_source as arp

def cache(service,ip='',mac=''):
	# Creates an instance of arpclient that will 
	# handle the ARP Cache aka ARP Table
	# Services are: 
	# (1) Display Table - no address specified 
	# (2) Table Look Up - either address specified
  # (3) Create Table Entry - both addresses specified
	# (4) Delete Table Entry - both addresses specified

	cacheworker = arp.arpclient('eth0') 

	if service == 1:
		cacheworker.querycache(1)
	if service == 2:
		cacheworker.querycache(2,ip,mac)
	if service == 3:
		cacheworker.updatecache(1,ip,mac)
	if service == 4:
		cacheworker.updatecache(2,ip,mac)
