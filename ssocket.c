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



#include <unistd.h>

/* accept */
int socket_accept(int s){
    struct sockaddr_storage from;
    unsigned int size = sizeof from;
    return(accept(s, (struct sockaddr *) &from, &size));
}


/* bind */
int socket_udp_bind(char *port) {
	return socket_bind(SOCK_DGRAM, port);
}

int socket_tcp_bind(char *port) {
	return socket_bind(SOCK_STREAM, port);
}

/*
int socket_unix_bind(char *port){
    int s;
    struct sockaddr_un addr;

    s = socket(AF_UNIX, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);

    if(bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "bind failed\n");
        exit(1);
   }

   return s;
}
*/

int socket_bind(int type, char *port){
    struct addrinfo hints, *res, *ressave;
    int n, sockfd;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = type;
    hints.ai_flags    = AI_PASSIVE | AI_V4MAPPED | AI_ALL;

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


//---------------------------------

int my_udp_bind(char *port){
    int sockfd;
    struct sockaddr_in addr;

    //crear Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "UDP socket error:: No se pudo crear el socket\n");
        return -1;
    }

    //Set Family
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //verificar que hace

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "socket error:: No se pudo hacer bind\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int my_udp_connect(char *server, char *port){
    int sockfd;
    struct sockaddr_in addr;

    //crear Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "socket error:: No se pudo crear el socket\n");
        return -1;
    }

    //Set Family
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(server); //verificar que hace

    if(connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket error:: No se pudo hacer connect\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int my_tcp_bind(char *port){
    int sockfd, sockfd_accepted;
    struct sockaddr_in addr, from;

    //crear Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "TCP socket error:: No se pudo crear el socket\n");
        return -1;
    }

    //Set Family
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //verificar que hace

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "socket error:: No se pudo hacer bind\n");
        close(sockfd);
        return -1;
    }

    listen(sockfd, 5);

    int from_len = sizeof(from);
    sockfd_accepted = accept(sockfd, (struct sockaddr*) &from, &from_len);
    if (from_len<0){
        fprintf(stderr, "socket error:: Error de accept\n");
        close(sockfd);
        return -1;
    }

    return sockfd_accepted;
}

int my_tcp_connect(char *server, char *port){
    int sockfd;
    struct sockaddr_in addr;

    //crear Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        fprintf(stderr, "TCP socket error:: No se pudo crear el socket\n");
        return -1;
    }

    //Set Family
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(server); //verificar que hace

    if(connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "TCP socket error:: No se pudo hacer connect\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}