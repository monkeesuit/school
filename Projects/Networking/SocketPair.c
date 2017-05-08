/*  
 *  A pipe is a pair of connected sockets for one-way stream communication.
 *  One may obtain a pair of connected sockets for two-way stream communication by calling the routine 
    socketpair().
 *  Socketpair() takes as arguments a specification of a domain, a style of communication, and a protocol.
 *  Pipes and socketpairs are a simple solution for communicating between a parent and child or between
    child processes. 
 *  What if we wanted to have processes that have no common ancestor with whom to set up communication? 
 *  Neither standard UNIX pipes nor socketpairs are the answer here, 
 *  since both mechanisms require a common ancestor to set up the communication. 
 *  We would like to have two processes separately create sockets and then have messages sent between them.
 *  Sockets created by different programs use names to refer to one another;
 *  names generally must be translated into addresses for use. 
 *  The space from which an address is drawn is referred to as a domain.
 *
 *  There are several domains for sockets. 
 *  Two that will be used in the examples here are the: 
 *      UNIX domain(or AF_UNIX, for Address Format UNIX)
 *      Internet domain (or AF_INET).
 *
 *  In the UNIX domain, a socket is given a path name within the file system name space. 
 *                      A file system node is created for the socket and other processes may then refer to the 
 *                      socket by giving the proper pathname.
 *                      UNIX domain names, therefore, allow communication between any two processes that work in 
 *                      the same file system.
 *
 *   The Internet domain is the UNIX implementation of the DARPA Internet standard protocols IP/TCP/UDP. 
 *                  Addresses in the Internet domain consist of a machine network address and an identifying 
 *                  number, called a port. 
 *                  Internet domain names allow communication between machines.
 *
 *  Communication follows some particular ‘‘style.’’ 
 *  Currently, communication is either through astream or by datagram.
 *
 *  Stream communication implies several things. 
 *      Communication takes place across a connection between two sockets. 
 *      The communication is reliable, error-free, and, as in pipes, no message boundaries are kept. 
 *      Reading from a stream may result in reading the data sent from one or several calls to
 *      write() or only part of the data from a single call, 
 *      if there is not enough room for the entire message,
 *      or if not all the data from a large message has been transferred. 
 *      The protocol implementing such a style will retransmit messages received with errors. 
 *      It will also return error messages if one tries to send a message after the connection has been broken.
 *
 *  Datagram communication does not use connections. 
 *      Each message is addressed individually. 
 *      If the address is correct, it will generally be received, although this is not guaranteed.
 *      Often datagrams are used for requests that require a response from the recipient. 
 *      If no responsearrives in a reasonable amount of time, the request is repeated. 
 *      The individual datagrams will be kept separatewhen they are read, 
 *      that is, message boundaries are preserved.
 *
 *  A protocol is a set of rules, data formats and conventions that regulate the transfer of data between
 *  participants in the communication. 
 *  In general, there is one protocol for each socket type (stream, datagram, etc.) within each domain. 
 *  The code that implements a protocol keeps track of the names that are bound to sockets, 
 *  sets up connections and transfers data between sockets, perhaps sending the data across a network.
 *  This code also keeps track of the names that are bound to sockets. 
 *  It is possible for several protocols, differing only in low level details, to implement the same style of 
 *  communication within a particular domain. 
 *  Although it is possible to select which protocol should be used, for nearly all uses it is sufficient to 
 *  request the default protocol. 
 *  This has been done in all of the example programs.
 *  One specifies the domain, style and protocol of a socket when it is created.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#define DATA1 "how now brown cow..."
#define DATA2 "sam i am , i do not like green eggs and ham..."

/*
 *  This program will create a pair of sockets then forks. 
 *  The parent and child will then be able to communicate in a two way manner.
 *  Socketpairs are very similar to pipes.
 *  But where pipes are a one-way communication object,
 *  socketpairs are instead a two-way communication object.
 */

int main() {
    int sockets[2], child;
    char buffer[1024];
    
    /* 
     *  Create a socketpair using the Address Format of UNIX,
     *                              the STREAM communictaion style, 
     *                              and the default protocol 
     */
    if(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        perror("opening socket stream pair");
        exit(1);
    }
    
    /* fork the process and create the child */
    if ((child = fork()) == -1) {
        perror(" at fork");
    }
    
    /*
     *  This is the parent.
     *  It will close the socket.
     *  Then it will read the input in the socketpair created by the child.
     *  Then it will write the ouput to the socketpair for the child to recieve.
     */
    if(child != 0){
        close(sockets[0]);
        if(read(sockets[1], buffer, 1024, 0) < 0)
            perror("reading message");
        printf("==> %s\n", buffer);
        if(write(sockets[1], DATA2, sizeof(DATA2)) < 0)
            perror("writing message");
        close(sockets[1]);
    } else {    /* This is the child  */
        close(sockets[1]);
        if(write(sockets[0], DATA1, sizeof(DATA1)) < 0)
            perror("writing message");
        if(read(sockets[0], buffer, 1024, 0) < 0)
            perror("reading message");
        printf("==> %s\n", buffer);
        close(sockets[0]);
    }
}

 
 
 
 
 
 
 
 
 
 
 
