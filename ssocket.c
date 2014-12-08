#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/un.h>
#include <errno.h>
#include "ssocket.h"

//#include <stdlib.h>
//#include <strings.h> 
//#include <string.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <netdb.h>
//#include <sys/types.h>
//#include <sys/wait.h>


/* 	Internet Sockets 	*/
int internet_socket(int type, char *server, char *port, struct sockaddr_in *addr){
	int sockfd;
	memset(addr, 0, sizeof(*addr));

	//Crear Socket
	sockfd = socket(AF_INET, type, 0);
	if(sockfd < 0){
        fprintf(stderr, "socket error:: No se pudo crear el socket\n");
        return -1;
    }

    //Set Parametros
    (*addr).sin_family = AF_INET;
    (*addr).sin_port = htons(atoi(port));
    if(server != NULL)
	    (*addr).sin_addr.s_addr = inet_addr(server);
	else
		(*addr).sin_addr.s_addr = htonl(INADDR_ANY);

    return sockfd;
}


/*		UDP 	*/

int udp_socket(char *server, char *port, struct sockaddr_in *addr){
	return internet_socket(SOCK_DGRAM, server, port, addr);
}

int udp_bind(char *port){
    int sockfd;
    struct sockaddr_in addr;

    sockfd = udp_socket(NULL, port, &addr);

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "socket error:: No se pudo hacer bind\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int udp_connect(char *server, char *port){
    int sockfd;
    struct sockaddr_in addr;

    //crear Socket
    sockfd = udp_socket(server, port, &addr);

    if(connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "socket error:: No se pudo hacer connect\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}



/*		TCP 	*/

int tcp_socket(char *server, char *port, struct sockaddr_in *addr){
    return internet_socket(SOCK_STREAM, server, port, addr);
}

int tcp_bind(char *port){
    int sockfd, sockfd_accepted;
    struct sockaddr_in addr, from;

    //crear Socket
    sockfd = tcp_socket(NULL, port, &addr);

    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "socket error:: No se pudo hacer bind. Error: %s\n", strerror(errno));
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

    close(sockfd);

    return sockfd_accepted;
}

int tcp_connect(char *server, char *port){
    int sockfd;
    struct sockaddr_in addr;

    //crear Socket
    sockfd = tcp_socket(server, port, &addr);

    if(connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "TCP socket error:: No se pudo hacer connect. Error: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }

    return sockfd;
}





/*	 UNIX Sockets 	*/
int my_unix_bind(char *namePathFile){
	int sockfd;
	struct sockaddr_un addr;

	memset(&addr, 0, sizeof(addr));

	//crear Socket
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); //TODO: USAR A UDP O TCP

	//Set Parameters
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, namePathFile, sizeof(addr.sun_path)-1);


    if(bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0){
        fprintf(stderr, "socket error:: No se pudo hacer bind\n");
        close(sockfd);
        return -1;
    }

    return sockfd;
}