#include <stdio.h>

#define DATA "How now brown cow..."

/*  This script opens a pipe with the pipe() call,
 *  then the parent forks using a fork() call, creating a child.
 *  The child will communicate with the parent using the pipe.
 *  A PIPE is a ONE-WAY COMMUNICATION DEVICE.
 *  It will write to the output socket 
 *  (the second socket of the array returned by the pipe, socket[1])
 *  And read from the input socket
 *  (the first socket of the array returned by the pipe, socket[0])
 *  BUT NOT VISA VERSA.
 */

int main() {
    int sockets[2], child;
    
    /*  Create a pipe.
     *  Should return an array of size two,
     *  containing the input and output sockets.
     */
    if(pipe(sockets)<0) {
        perror("opening stream socket pair");
        exit(10);
    }
    
    /*  Fork the process,
     *  assigning the newly created process to the variable child.
     */
    if((child = fork()) == -1) {
        perror("fork");
        exit(10);
    }
    
    /*  If the process executing at this point is the parent process,
     *  then it will close the output socket.
     *  Read the input in the input socket and return the message.
     *  Then it will close the input socket as well.
     */
    if(child != 0) {
        char buffer[1024];
        
        /*  This is still the parent.
         *  It read the childs message. */
        close(sockets[1]);
        if(read(sockets[0], buffer, 1024) < 0)
            perror("reading messages");
        printf("--> %s\n", buffer);
        close(sockets[0]);
    
    /*  Else if this is the child process,
     *  which starts executing the instruction after the fork call where it was created
     *  and which inherits all data from the parent process at the time of creation,
     *  then it will close the input socket,
     *  and write the message into the output socket.
     *  Then it will close the output socket as well.
     */
    } else {
        
        /*  This is the child.
        *   It writes the message to the parent. */
        close(sockets[0]);
        if(write(sockets[1], DATA, sizeof(DATA)) < 0)
            perror("writing message");
        close(sockets[1]);
    }
}

/*  The parent process makes a call to the system routine pipe(). 
 *  This routine creates a pipe and places descriptors for the sockets for the two ends of the pipe in the process’s descriptor table. 
 *  Pipe() is passed an array into which it places the index numbers of the sockets it created. 
 *  The two ends are not equivalent. 
 *  The socket whose index is returned in the LOW WORD of the array is opened for READING ONLY,
 *  while the socket in the HIGH END is opened ONLY FOR WRITING.
 *  This corresponds to the fact that the standard input is the first descriptor of a process’s descriptor table and the standard output is the second. 
 *
 *  After creating the pipe, the parent creates the child with which it will share the pipe by calling fork().
 *  The parent process’s descriptor table points to both ends of the pipe. 
 *  After the fork, BOTH parent’s and child’s descriptor tables point to the pipe.
 *  The child can then use the pipe to send a message to the parent.
 *
 *  Just what is a pipe? 
 *  It is a one-way communication mechanism, with one end opened for reading and the other end for writing. 
 *  Therefore, parent and child need to agree on which way to turn the pipe, from parent to child or the other way around.
 *
 *  Using the same pipe for communication both from parent to child and from child to parent would be possible 
 *  (since both processes have references to both ends), but very complicated.
 *  If the parent and child are to have a two-way conversation, the parent creates two pipes, one for use in each direction. 
 *  (In accordance with their plans, both parent and child in the example above close the socket that they will not use. 
 *  It is not required that unused descriptors be closed, but it is good practice.)
 *
 *  A pipe is also a stream communication mechanism; that is, all messages sent through the pipe are placed in order and reliably delivered. 
 *  When the reader asks for a certain number of bytes from this stream, he is given as many bytes as are available, up to the amount of the request. 
 *  Note that these bytes may have come from the same call to write() or from several calls to write() which were concatenated.
 */

