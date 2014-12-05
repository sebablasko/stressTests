/* Compilado de funciones de manejo de sockets
 inspirada en la librería jsockets de piquer */

int socket_accept(int s);

int socket_udp_bind(char *port);
int socket_tcp_bind(char *port);
int socket_bind(int type, char *port);

int socket_udp_connect(char *server, char *port);
int socket_tcp_connect(char *server, char *port);
int socket_connect(int type, char *server, char *port);