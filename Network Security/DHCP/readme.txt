dhcp_client_v3.py is a script that partially implements a dhcp client and also can perform a network attack.

calling the script 

The script takes a range of IP addresses and for each one it spawns an instance of the dhcp_client class.  
Each one of these dhcp_client instances is given a fake mac address and then the instance sends out a discover message asking to be assigned the IP address it is an instance of. 
If it receives more than one offer, it tries to get the offer of the IP address it is an instance of else.  
If that IP address is not offered at all but some other address is offered, then the instance will take that one instead.  
If no offer is made then the instance ends.
Once the instance has an offer it makes a dhcp request for the offered address and if the instance receives an ACK then it records the binding in a log file and appends the bound MAC and IP address to a list of bound MAC addresses and a list of bound IP addresses respectively.
After attempting to bind to each ip address once, the script updates the list of IPs still unbound and repeats the process.
This continues until all the IPs are bound.

There is a minor congestion control algorithm that slows down the rate at which packets are sent and times out after enough unanswered sent messages.  
The rate also accelerates if the server is responsive. 

Screenshots:
bogus.mac.ledger = shows the log file of the leases my bogus mac addresses aquired
command_line_call = shows the command to run the script
dhcpd[~}.leases.before = the clean file
leases.after.(1/2) = shows the ip addresses bound to my bogus MAC addresses
starvation.(1/2/3) = shows the program running
wireshark.(discover/request/offer) = shows the wireshark capture of one DHCP transaction
xp_starved = shows the xp machine unable to aquire a network address
