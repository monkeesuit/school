# ARP Suite - Run ARP Commands From Command Line
import sys
import arp_mitm as mitm
import arp_sslstrip as sslstrip
import arp_listen as listen
import arp_request as request
import arp_cache as cache
import arp_reconnaissance as recon
import arp_interactive as interactive

if __name__ == "__main__":
	arguments = sys.argv[1:]

	if '-h' in arguments or '--help' in arguments:
		print '[INFO]\tARP Suite\n'
		print '[USAGE] arp.py -c/i/L/r\n'
		print '[FUNCTIONS]'
 		print ' -c --cache       = Work with ARP Cache.'
		print ' -i --interactive = Runs Interactive ARP Suite.'
		print ' -L --listen      = Runs an arpclient in listen Mode.'
		print ' -r --request     = Generate an ARP Request Message.'
		print '\n\t* Use --h with any of these functions to learn more about them.'
		print '\t\tex. arp.py -c --h'
		print ''
		sys.exit(1)

	if '-i' in arguments or '--interactive' in arguments:
		interactive.run()
		sys.exit(1)

	if '-L' in arguments or'--listen' in arguments:
		if '--h' in arguments:
			print '[INFO]\tCreates an instance of arpclient in listen mode.'
			print '\tHandles ARP Messages and ARP Table.'
			print ''
			print '[USAGE] arp.py -l\n'
			print '[ARGUMENTS]'
			print '\tNONE'
			sys.exit(1) 
		listen.listen()
		sys.exit(1)

	if '-r' in arguments or '--request' in arguments:
		if '--h' in arguments:
			print '[INFO]\tCreate an ARP Request message to given IP Address.'
			print '\tMake sure there is an instance of arpclient in listen mode' 
			print '\tto handle ARP messages and manipulate ARP table ("arp.py -l").'
			print ''
			print '[USAGE] arp.py -r --ip [ip]\n'
			print '[ARGUMENTS]'
			print '\t"--ip" = IP Address You Wish To Resolve'
			print ''
			sys.exit(1)
		if '--ip' in arguments:
			option_index = arguments.index('--ip')
			ip = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -r --h"'
			sys.exit(0)
		request.send(ip)
		sys.exit(1)

	if '-c' in arguments or '--cache' in arguments:
		if '--h' in arguments:
			print '[INFO]\tWork with the ARP Cache\n'
			print '[USAGE] arp.py -c --d/l/a/r --i [ip] --m [mac]\n'
			print '[ARGUMENTS]'
			print '"--d" = Display ARP Cache.'
			print '"--l" = Look Up ARP Cache. Must Specify Either Address'
			print '"--a" = Add ARP Cache Entry. Must Specify Both Addresses'
			print '"--r" = Remove ARP Cache Entry. Must Specify Both Addresses'
			print '"--i" = An IP Address'
			print '"--m" = A MAC Address'
			print ''
		# Display
		if '--d' in arguments:
			cache.cache(1)
		# Look Up
		if '--l' in arguments:
			if '--i' in arguments:
				option_index = arguments.index('--i')
				ipoption = arguments[option_index+1]
				cache.cache(2,ip=ipoption)
				sys.exit(1)
			elif '--m' in arguments:
				option_index = arguments.index('--m')
				macoption = arguments[option_index+1]
				cache.cache(2,mac=macoption)
				sys.exit(1)
			else:
				print 'Missing Argument!'
				print 'See help for cache by typing "python arp.py -c --h"'
				sys.exit(0)
		# ADD an Entry
		if '--a' in arguments:
			if '--i' in arguments: # use --i to indicate you are giving an ip address
				option_index = arguments.index('--i')
				ipoption = arguments[option_index+1]
			else:
				print 'Missing Argument!'
				print 'See help for cache by typing "python arp.py -c --h"'
				sys.exit(0)	
			if '--m' in arguments: # use --m to indicate you are giving a mac address
				option_index = arguments.index('--m')
				macoption = arguments[option_index+1]		
			else:
				print 'Missing Argument!'
				print 'See help for cache by typing "python arp.py -c --h"'
				sys.exit(0)
			cache.cache(3,ip=ipoption,mac=macoption)
			sys.exit(1)
		# REMOVE an Entry
		if '--r' in arguments:
			if '--i' in arguments: # use --i to indicate you are giving an ip address
				option_index = arguments.index('--i')
				ipoption = arguments[option_index+1]
			else:
				print 'Missing Argument!'
				print 'See help for cache by typing "python arp.py -c --h"'
				sys.exit(0)	
			if '--m' in arguments: # use --m to indicate you are giving a mac address
				option_index = arguments.index('--m')
				macoption = arguments[option_index+1]		
			else:
				print 'Missing Argument!'
				print 'See help for cache by typing "python arp.py -c --h"'
				sys.exit(0)
			cache.cache(4,ip=ipoption,mac=macoption)
			sys.exit(1)


	if '-m' in arguments or '--mitm' in arguments:
		if '--h' in arguments:
			print '[Info]\tLaunch an ARP Poisoning Man in the Middle Attack.\n'
			print '[Usage] arp.py -m --aI [ip] --aM [mac] --bI [ip] --bM [mac]\n'
			print '[Arguments]'
			print '\t"--aI" = target A\'s IP Address'
			print '\t"--aM" = target A\'s MAC Address'
			print '\t"--bI" = target B\'s IP Address'
			print '\t"--bM" = target B\'s MAC Address'
			print ''
			sys.exit(1)
		if '--aI' in arguments:
			option_index = arguments.index('--aI')
			aIP = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--aM' in arguments:
			option_index = arguments.index('--aM')
			aMAC = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--bI' in arguments:
			option_index = arguments.index('--bI')
			bIP = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--bM' in arguments:
			option_index = arguments.index('--bM')
			bMAC = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		mitm.mitm(aIP,aMAC,bIP,bMAC)
		sys.exit(1)

	if '--sslstrip' in arguments:
		if '--h' in arguments:
			print '[Info]\tLaunch a SSL Strip Attack.\n'
			print '[Usage] arp.py --sslstrip --gI [ip] --gM [mac] --tI [ip] --tM [mac]\n'
			print '[Arguments]'
			print '\t"--gI" = gateway\'s IP Address'
			print '\t"--gM" = gateway\'s MAC Address'
			print '\t"--tI" = target\'s IP Address'
			print '\t"--tM" = target\'s MAC Address'
			print ''
			sys.exit(1)
		if '--gI' in arguments:
			option_index = arguments.index('--gI')
			gIP = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--gM' in arguments:
			option_index = arguments.index('--gM')
			gMAC = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--tI' in arguments:
			option_index = arguments.index('--tI')
			tIP = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		if '--tM' in arguments:
			option_index = arguments.index('--tM')
			tMAC = arguments[option_index+1]
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py -m --h"'
			sys.exit(0)
		sslstrip.sslstrip(gIP,gMAC,tIP,tMAC)
		sys.exit(1)

	if '--recon' in arguments:
		if '--h' in arguments:
			print '[Info]\tLearn Address of Those on Network.\n'
			print '[Usage] arp.py --recon --ip [iprange], wildcards * allowed\n'
			print '[Arguments]'
			print '\t"--ip" = A Range of IP Adresses to Scan'
		if '--ip' in arguments:
			option_index = arguments.index('--ip')
			iprange = arguments[option_index+1]
			recon.run(str(iprange))
			sys.exit(1)
		else:
			print 'Missing Argument!'
			print 'See help for mitm by typing "python arp.py --recon --h"'
			sys.exit(0)



