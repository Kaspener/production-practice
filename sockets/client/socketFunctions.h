#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define BUFFERSIZE 1024
#define LISTEN_QUEUE 5
#define MAX_CLIENTS 20
#define MAX_NAME_LENGTH 16
#define MESSAGE_SIZE 1
#define TYPE_SIZE 1

#define BROADCASTSERVER '2'
#define PRIVATESERVER '3'
#define USERLIST '4'
#define ERRORMESSAGE '5'

#define OKEY '0'
#define NO_PLACE '1'
#define NO_NAME '2'

#define NAMEC1  '0'
#define NAMEC2  '0'
#define NAMEC3  '1'

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen);

int GetSockName(int sockfd, struct sockaddr_in *restrict addr, socklen_t *restrict addrlen);

int Listen(int sockfd, int backlog);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);



#endif // SOCKET_FUNCTIONS_H
