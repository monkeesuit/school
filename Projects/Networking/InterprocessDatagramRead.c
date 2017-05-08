/*
 *  Let us now look at two programs that create sockets separately.
 *  The structure used to name UNIX domain sockets is defined in the file <sys/un.h>.
 *  Each program creates a socket with a call to socket().
 *  These sockets are in the UNIX domain.
 *  Once a name has been decided upon it is attached to a socket by the system call bind().
 *  This name will appear in the working directory of the program.
 *  If the routine only uses its socket for sending messages,
 *  then it does not need to have a name because no other process will have to refer to it.
 *  Names in the UNIX domain are path names. 
 *  Like file path names they may be either absolute (e.g./dev/imaginary) or relative (e.g. socket).
 *  Because these names are used to allow processes to rendezvous,
 *  relative path names can pose difficulties and should be used with care.
 *  When a name is bound into the name space, a file (inode) is allocated in the file system.
 *  If the inode is not deallocated, the name will continue to exist even after the bound socket is closed.
 *  This can cause subsequent runs of a program to find that a name is unavailable,
 *  and can cause directories to fill up with these objects.
 *  The names are removed by calling unlink() or using the rm (1) command.
 *  Names in the UNIX domain are only used for rendezvous.
 *  They are not used for message delivery once a connection is established.
 *  Therefore, in contrast with the Internet domain, unbound sockets need not be (and are not) automatically 
 *  given addresses when they are connected.
 *  There is no established means of communicating names to interested parties.
 *   In the example, the program in Figure 5b gets the name of the socket to which it will send its message through its command line arguments. 
 *  Once a line of communication has been created, one can send the names of additional, perhaps new, sockets 
 *  over the link.
 *  Facilities will have to be built that will make the distribution of names less of a problem than it now is.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * In the included file <sys/un.h> a sockaddr_un is defined as follows
 * struct sockaddr_un {
 * short sun_family;
 * char sun_path[108];
 * };
 */

#define NAME "socket"

/*
 *  This program creates a UNIX domain datagram socket,
 *  binds a name to it,
 *  then reads from the socket.
 */

int main() {
    int sock, length;
    struct sockaddr_un name;
    char buffer[1024];
    
    /* Create a socket using UNIX addressing, Datagram communications, standard protocol. */
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("opening socket");
        exit(10);
    }
    
    /* Name socket. */
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, NAME);
    if(bind(sock, &name, sizeof(struct sockaddr_un))) {
        perror("binding name to socket");
        exit(10);
    }
    printf("socket ==> %s is now listening\n", NAME);
    
    /* Read from socket. */
    if(read(sock,buffer, 1024) < 0)
        perror("receiving packet");
    printf("message ==> %s\n", buffer);
    close(sock);
    unlink(NAME);
}















