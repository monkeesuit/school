from scapy.all import *
from subprocess import PIPE, Popen

###[ get_interface_ip_address ]###
 # Pulls IP Address of Given Interface
 # By Calling: ifconfig <interface> | filter
 # Machine Dependent => Has to be Filtered in a Specific Way
 # subprocess.communicate() returns a tuple (stdoutdata,stderrdata)
def get_interface_ip(interface):
	command=['ifconfig', str(interface)]
	backtrackfilter = ['grep', 'inet\ ']	# Add More Filters
	process1 = Popen(command, stdout=PIPE)
	process2 = Popen(backtrackfilter, stdin=process1.stdout, stdout=PIPE)
	process1.stdout.close()
	output = process2.communicate()[0]
	try:
		interface_ip = output.split()[1].split(':')[1]
	except:
		print 'Error: Parsing Interface IP from ifconfig Command.'
		return 0
	return interface_ip

###[ get_interface_mac_address ]###
 # Uses scapy function [get_if_raw_hwaddr] to pull MAC Address
 # Convert it to a readable version
 # Return both values
def get_interface_mac(interface):
	fam, interface_mac = get_if_raw_hwaddr(interface)
	readable_interface_mac = ':'.join(["%02X" %ord(x) for x in interface_mac])
	return interface_mac, readable_interface_mac

###[ gatewayiplookup ]###
 # Looks Up Your GatewayIP
 # Calls: netstat -nr | filter
 # Machine Dependent => Has to be Filtered in a Specific Way
 # subprocess.communicate() returns a tuple (stdoutdata,stderrdata)
def gatewayiplookup():
	command = ['netstat','-nr']
	backtrackfilter = ['grep', 'UG']		# Add More Filters
	process1 = Popen(command, stdout=PIPE)
	process2 = Popen(backtrackfilter, stdin=process1.stdout, stdout=PIPE)
	process1.stdout.close()
	output = process2.communicate()[0]
	try:
		gatewayIP = output.split()[1]
	except:
		print 'Error: Parsing Gateway IP from netstat Command.'
		return 0
	return gatewayIP
