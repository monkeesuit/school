import arp_source as arp
import shared_functions as sf
import subprocess
import arp_source as arp
from scapy.all import *

def learnhosts(iprange): # Network Sweepers
	print 'Learning Hosts...'
	print iprange
	arping(iprange) 

def gateway(): # Learn Gateway Router IP and MAC Address
	print ''
	print '###[ GATEWAY ]###'
	# CommandLineLookup
	gatewayIP = sf.gatewayiplookup()
	# ARPCacheLookUp
	client = arp.arpclient('eth0')
	response,line = client.querycache(2,mac=str(gatewayIP))
	if response == 0:
		while response == 0:
			client.request(gatewayIP)
			response,line = client.querycache(2,mac=str(gatewayIP))

def run(iprange):
	learnhosts(iprange)
	gateway()

	
