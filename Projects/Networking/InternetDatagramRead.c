/*
 *  The structure of Internet domain addresses is defined in the file <netinet/in.h>.
 *  Internet addresses specify a host address (a 32-bit number) and a delivery slot, or 
 *  port, on that machine.
 *  These ports are managed by the system routines that implement a particular protocol.
 *  Unlike UNIX domain names, Internet socket names are not entered into the file system
 *  and, therefore, they do not have to be unlinked after the socket has been closed.
 *  When a message must be sent between machines it is sent to the protocol routine on 
 *  the destination machine, which interprets the address to determine to which socket
 *  the message should be delivered.
 *  Several different protocols may be active on the same machine, but, in general,
 *  they will not communicate with one another.
 *  As a result, different protocols are allowed to use the same port numbers.
 *  Thus, implicitly, an Internet address is a triple including a protocol as well as the 
 *  port and machine address.
 *  An association is a temporary or permanent specification of a pair of communicating
 *  sockets.
 *  An association is thus identified by the tuple <protocol, local machine address, 
 *  local port, remote machine address, remote port>.
 *  An association may be transient when using datagram sockets; the association
 *  actually exists during a send operation.
 *  The protocol for a socket is chosen when the socket is created.
 *  The local machine address for a socket can be any valid network address of the 
 *  machine, if it has more than one, or it can be the wildcard value INADDR_ANY.
 *  If a machine has several network addresses, it is likely that messages sent to any of 
 *  the addresses should be deliverable to a socket.
 *  This will be the case if the wildcard value has been chosen.
 *  Note that even if the wildcard value is chosen, a program sending messages to the 
 *  named socket must specify a valid network address.
 *  One can be willing to receive from ‘‘anywhere,’’ but one cannot send a message 
 *  ‘‘anywhere.’’
 *  To determine a network address to which it can send the message, it looks up the host 
 *  address by the call to gethostbyname().
 *  The returned structure includes the host’s network address, which is copied into the 
 *  structure specifying the destination of the message.
 *  The port number can be thought of as the number of a mailbox, into which the protocol 
 *  places one’s messages.
 *  Certain daemons, offering certain advertised services, have reserved or ‘‘well-
 *  known’’ port numbers.
 *  These fall in the range from 1 to 1023.
 *  Higher numbers are available to general users.
 *  Only servers need to ask for a particular number. 
 *  The system will assign an unused port number when an address is bound to a socket.
 *  This may happen when an explicit bind call is made with a port number of 0, or when
 *  a connect or send is performed on an unbound socket.
 *  Note that port numbers are not automatically reported back to the user.
 *  After calling bind(), asking for port 0, one may call getsockname() to discover what 
 *  port was actually assigned.
 *  The routine getsockname() will not work for names in the UNIX domain.
 *  The format of the socket address is specified in part by standards within the 
 *  Internet domain.
 *  The specification includes the order of the bytes in the address.
 *  Because machines differ in the internal representation they ordinarily use to 
 *  represent integers, printing out the port number as returned by getsockname()
 *  may result in a misinterpretation.
 *  To print out the number, it is necessary to use the routine ntohs() (for network to 
 *  host: short) to convert the number from the network representation to the host’s 
 *  representation.
 *  On some machines, such as 68000-based machines, this is a null operation.
 *  On others, such as VAXes, this results in a swapping of bytes.
 *  Another routine exists to convert a short integer from the host format to the network 
 *  format, called htons(); similar routines exist for long integers.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

/*
 * In the included file <netinet/in.h> a sockaddr_in is defined as follows:
 * struct sockaddr_in {
 * short sin_family;
 * u_short sin_port;
 * struct in_addr sin_addr;
 * char sin_zero[8];
 * };
 *
 * This program creates a datagram socket, binds a name to it, then reads
 * from the socket.
 */

int main() {
    int sock, length;
    struct sockaddr_in name;
    char buffer[1024];
    
    /* Create socket to read with */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket < 0) {
        perror("opening socket");
        exit(10);
    }
    
    /* Name socket using wildcards */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = 0;
    if(bind(sock, &name, sizeof(name))) {
        perror("binding socket");
        exit(10);
    }
    
    /*Find and print out assigned port value */
    length = sizeof(name);
    if(getsockname(sock, &name, &length)) {
        perror("getting name");
        exit(10);
    }
    printf("Socket has port #%d\n", ntohs(name.sin_port));
    
    /* Read from socket */
    length = sizeof(name);
    if(read(sock, buffer, 1024) < 0)
        perror("receiving packet");
    printf("--> %s\n", buffer);
    close(socket);
}




























