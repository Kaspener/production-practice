#include "socketFunctions.h"
#include <stdlib.h>
#include <stdio.h>

int Socket(int domain, int type, int protocol)
{
    int serverSocket = socket(domain, type, protocol);
    if (serverSocket == -1)
    {
        perror("The server socket cannot be opened");
    }
    return serverSocket;
}

int Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen){
    int res = bind(sockfd, (struct sockaddr *)addr, addrlen);
    if (res == -1){
        perror("Bind failed");
    }
    return res;
}


int Listen(int sockfd, int backlog) {
    int res = listen(sockfd, backlog);

    if ( res == -1) {
        perror("Listen failed");
    }
    return res;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res= connect(sockfd, addr, addrlen);
    if (res == -1) {
        perror("Connect failed");
    }
    return res;
}
