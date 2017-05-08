import cmd
import arp_mitm as mitm
import arp_sslstrip as sslstrip
import arp_listen as listen
import arp_request as request
import arp_cache as cache
import arp_reconnaissance as recon

class cacheinteractive(cmd.Cmd):
	prompt = 'arp:cache $ '
	doc_header = 'Functions (help [function] for more info)'
	undoc_header = 'Undocumented Funtions'

	def help_display(self):
		print '[USAGE] display'
		print '[INFO]\t Displays the ARP Cache'
	def help_lookup(self):
		print '[USAGE] lookup, then follow displayed instructions.'
		print '[INFO]\tLooks up an IP or MAC Address in the ARP Cache'
		print	'\tand displays the addresses pairing if it is present.'
	def help_add(self):
		print '[USAGE] add, then follow displayed instructions.'
		print '[INFO]\tAdds an IP, MAC Address pairing to the ARP Cache'
	def help_remove(self):
		print '[USAGE] remove, then follow displayed instructions.'
		print '[INFO]\tRemoves an IP, MAC Address pairing from the ARP Cache'
	def help_exit(self):
		print 'Exits shell.'
	def help_help(self):
		print 'Displays useful information.'

	def do_display(self, line):
		cache.cache(1)

	def do_lookup(self,line):
		print 'Would You Like to Look Up An IP Address or MAC Address'
		choice = raw_input('Choose ip/mac: ')
		if choice == 'ip':
			address = raw_input('Enter Address: ')
			line = cache.cache(2,ip=address)
		elif choice == 'mac':
			address = raw_input('Enter Address: ')
			cache.cache(2,mac=address)
		else:
			print 'Invalid Choice!'

	def do_add(self,line):
		userip = raw_input('Enter IP Address: ')
		usermac = raw_input('Enter MAC Address: ')
		cache.cache(3,ip=userip,mac=usermac)

	def do_remove(self,line):
		userip = raw_input('Enter IP Address: ')
		usermac = raw_input('Enter MAC Address: ')
		cache.cache(4,ip=userip,mac=usermac)

	def do_exit(self, line):
		return True

	def cmdloop(self):
		try:
			cmd.Cmd.cmdloop(self)
		except KeyboardInterrupt:
			print ''
			return True

class arpinteractive(cmd.Cmd):
	prompt = 'arp $ '
	intro = 'Welcome to ARP Interactive.\nPress Ctrl+C  or type "exit" to Exit.'
	doc_header = 'Functions (help [function] for more info)'
	undoc_header = 'Undocumented Funtions'
	
	def help_listen(self):
		print '[USAGE] listen'
		print '[INFO]\tSpawns an arpclient to handle ARP Messages'
		print '\tand manage ARP Cache.'
	def help_request(self):
		print '[USAGE] request [ip]'
		print '[INFO]\tGenerate an ARP Request for the specified IP Address.'
	def help_cache(self):
		print '[USAGE] cache'
		print '[INFO]\tStart an interactive arp:cache session.'
	def help_mitm(self):
		print '[USAGE] mitm'
		print '[INFO]\tPerform a Man in the Middle attack.'
		print '\trecon is helpful.'
	def help_sslstrip(self):
		print '[USAGE] sslstrip'
		print '[INFO]\tPerform a sslstrip attack.'
		print '\trecon is helpful.'
	def help_recon(self):	
		print '[USAGE] recon [ip-range]'
		print '[INFO]\tDisplays hosts on network and which is gateway.'
		print '\tWildcard * is allowed'
	def help_exit(self):
		print 'Exits Shell.'
	def help_help(self):
		print 'Displays useful information.'

	def do_listen(self, line):
		listen.listen()

	def do_request(self, line):
		request.send(line)

	def do_cache(self, line):
		cacheinteractive().cmdloop()

	def do_mitm(self, line):
		print 'Select two targets (use recon command).'
		print 'For each, enter their IP and MAC address when prompted.'
		aIP = raw_input('Target A\'s IP Address: ')
		aMAC = raw_input('Target A\'s MAC Address: ')
		bIP = raw_input('Target B\'s IP Address: ')
		bMAC = raw_input('Target B\'s MAC Address: ')
		mitm.mitm(aIP,aMAC,bIP,bMAC)

	def do_sslstrip(self, line):
		print 'Select a target and the gateway router (use recon command).'
		print 'For each, enter their IP and MAC address when prompted.'
		gIP = raw_input('Gateways\'s IP Address: ')
		gMAC = raw_input('Gateways\'s MAC Address: ')
		tIP = raw_input('Target\'s IP Address: ')
		tMAC = raw_input('Target\'s MAC Address: ')
		sslstrip.sslstrip(gIP,gMAC,tIP,tMAC)

	def do_recon(self, line):
		recon.run(line)

	def do_exit(self, line):
		return True

	def cmdloop(self):
		try:
			cmd.Cmd.cmdloop(self)
		except KeyboardInterrupt:
			print ''
			return True

def run():
	arpinteractive().cmdloop()
