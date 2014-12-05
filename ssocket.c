#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> 
#include <string.h>
#include "ssocket.h"


/* accept */
int socket_accept(int s){
    struct sockaddr_storage from;
    unsigned int size = sizeof from;
    return(accept(s, (struct sockaddr *) &from, &size));
}


/* bind */
int socket_socket_udp_bind(char *port) {
	return socket_socket_bind(SOCK_DGRAM, port);
}

int socket_socket_tcp_bind(char *port) {
	return socket_socket_bind(SOCK_STREAM, port);
}

int socket_bind(int type, char *port){
    struct addrinfo hints, *res, *ressave;
    int n, sockfd;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags    = AI_PASSIVE | AI_V4MAPPED | AI_ALL;
    hints.ai_family   = AF_INET;
    hints.ai_socktype = type;

    n = getaddrinfo(NULL, port, &hints, &res);

    if (n <0) {
        fprintf(stderr, "getaddrinfo error:: [%s]\n", gai_strerror(n));
        return -1;
    }

    ressave = res;
    sockfd = -1;

    while(res) {
    	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    	if (sockfd >= 0) {
            if(bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                break;
            close(sockfd);
            sockfd = -1;
        }
        res = res->ai_next;
    }

    if (sockfd < 0) {
        freeaddrinfo(ressave);
        fprintf(stderr, "socket error:: could not open socket\n");
        return -1;
    }

    if(type == SOCK_STREAM)
    	listen(sockfd, 5);

    freeaddrinfo(ressave);
    return sockfd;
}


/* connect */
int socket_udp_connect(char *server, char *port){
	return socket_connect(SOCK_DGRAM, server, port);
}

int socket_tcp_connect(char *server, char *port){
	return socket_connect(SOCK_STREAM, server, port);
}

int socket_connect(int type, char *server, char *port){
    struct addrinfo hints, *res, *ressave;
    int n, sockfd;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_flags    = AI_V4MAPPED | AI_ALL;

    n = getaddrinfo(server, port, &hints, &res);

    if (n <0) {
        fprintf(stderr, "getaddrinfo error:: [%s]\n", gai_strerror(n));
        return -1;
    }

    ressave = res;

    while(res) {
    	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    	if (sockfd >= 0) {
            if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
                break;
            close(sockfd);
            sockfd = -1;
        }
        res = res->ai_next;
    }

    if (sockfd < 0) {
        freeaddrinfo(ressave);
        fprintf(stderr, "socket error:: could not open socket\n");
        return -1;
    }

    freeaddrinfo(ressave);
    return sockfd;
}