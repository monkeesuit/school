#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#define DATA "how now brown cow..."

/*
 * Here I send a datagram to a receiver whose name I get from the command
 * line arguments. The form of the command line is dgramsend hostname
 * portnumber
 */

int main(argc, argv)
int argc;
char *argv[];
{

    int sock;
    struct sockaddr_in name;
    struct hostent *hp, *gethostbyname();
    
    /* Create socket on which to send. */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("opening datagram socket");
        exit(1);
    }
    
    /*
     * Construct name, with no wildcards, of the socket to send to.
     * Getnostbyname() returns a structure including the network address
     * of the specified host. The port number is taken from the command
     * line.
     */
    
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
        fprintf(stderr, "%s: unknown host0", argv[1]);
        exit(2);
    }
    bcopy(hp->h_addr, &name.sin_addr, hp->h_length);
    name.sin_family = AF_INET;
    name.sin_port = htons(atoi(argv[2]));
    
    /* Send message. */
    if (sendto(sock, DATA, sizeof(DATA), 0, &name, sizeof(name)) < 0)
        perror("sending datagram message");
    close(sock);
}

/* Input(WRITE FUNCTION):
    slr:NetworkingScripts sammishra$ ./netwrite 0 60987 */

/* Output (READ FUNCTION):
    slr:NetworkingScripts sammishra$ ./netread
    Socket has port #60987
    --> The sea is calm tonight, the tide is full . . . */