/* Compilado de funciones de manejo de sockets
 inspirada en la librería jsockets de piquer
 y de la documentación en
 http://www.ivanescobar.com/oper2_material/sockunix.pdf */

int socket_accept(int s);

int socket_udp_bind(char *port);
int socket_tcp_bind(char *port);
//int socket_unix_bind(char *port);

int socket_udp_connect(char *server, char *port);
int socket_tcp_connect(char *server, char *port);

int my_udp_bind(char *port);
int my_udp_connect(char *server, char *port);
int my_tcp_bind(char *port);
int my_tcp_connect(char *server, char *port);