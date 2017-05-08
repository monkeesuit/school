import arp_source as arp
import arp_reconnaissance as recon
from subprocess import Popen
import os
from arp_source import *

def mitm(aIP,aMAC,bIP,bMAC):
	# Runs the mitm() function found in arpclient in arp_source.py
	eve=arpclient('eth0')
	eve.mitm(aIP,aMAC,bIP,bMAC)
