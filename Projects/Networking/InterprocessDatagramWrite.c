#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA "how now brown cow..."

int main(argc, argv)
    int argc;
    char *argv[];
    {
    int sock;
    struct sockaddr_un name;
    
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("opening socket");
        exit(1);
    }
    
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, argv[1]);
    
    if(sendto(sock, DATA, sizeof(DATA), 0, &name, sizeof(struct sockaddr_un)) < 0) {
        perror("sending message");
    }
    close(sock);
}