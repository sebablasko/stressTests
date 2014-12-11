#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

/*
El cliente:
--Datos:
	-Envia una cantidad DATA de datos, esta cantidad supera a la que espera recibir el servidor
*/

//Definiciones
#define BUF_SIZE 10
#define MAX_PACKS 10000000
#define FIRST_PORT "1820"

//Variables
struct timeval dateInicio, dateFin;
char buf[BUF_SIZE];
char* IP_DEST;
int mostrarInfo = 0;
double segundos;

main(int argc, char **argv) {
	if(argc < 1){
		fprintf(stderr, "Syntax Error: Esperado: ./client IP_DEST\n");
		exit(1);
	}
	IP_DEST = argv[1];

	/* Llenar de datos el buffer a enviar */
	int i;
	for(i = 0; i < BUF_SIZE; i++)
		buf[i]='a'+i;

	/* Abrir socket */
	int socket_fd;
	socket_fd = tcp_connect(IP_DEST, FIRST_PORT);
	if(socket_fd < 0){
		fprintf(stderr, "Error al hacer connect del socket TCP");
		exit(1);
	}

	//Medir Fin
	gettimeofday(&dateFin, NULL);

	for(i = 0; i < MAX_PACKS; i++){
		if(write(socket_fd, buf, BUF_SIZE) != BUF_SIZE) {
			break;
		}
	}

	gettimeofday(&dateFin, NULL);

	segundos=(dateFin.tv_sec*1.0+dateFin.tv_usec/1000000.)-(dateInicio.tv_sec*1.0+dateInicio.tv_usec/1000000.);
	if(mostrarInfo){
		printf("Tiempo Total = %g\n", segundos);
		printf("QPS = %g\n", MAX_PACKS*1.0/segundos);
	}else{
		printf("%g \n", segundos);
	}
	exit(0);
}