/* Compilado de funciones de manejo de sockets
 inspirada en la documentación de
 http://www.ivanescobar.com/oper2_material/sockunix.pdf */

/*		Internet sockets	*/
int udp_bind(char *port);
int udp_connect(char *server, char *port);
int tcp_bind(char *port);
int tcp_connect(char *server, char *port);

/*		Unix sockets	*/