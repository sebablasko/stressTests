#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

/*
El cliente:
--Datos:
	-Envia una cantidad DATA de datos, esta cantidad supera a la que espera recibir el servidor
--Threads:
	-Se envia información a través de P Threads, y se envían DATA/P datos por Thread
*/

//Definiciones
#define BUF_SIZE 10
#define MAX_PACKS 10000000
#define MY_SOCK_PATH "unix_sock"

//Variables
struct timeval dateInicio, dateFin;
char buf[BUF_SIZE];
int mostrarInfo = 0;
double segundos;

main(int argc, char **argv) {
	int i;

	/* Llenar de datos el buffer a enviar */
	for(i = 0; i < BUF_SIZE; i++)
		buf[i]='a'+i;

	/* Abrir socket */
	int unix_fd;
	unix_fd = unix_udp_connect(MY_SOCK_PATH);
	if(unix_fd < 0){
		fprintf(stderr, "Error al abrir el socket Unix");
		exit(1);
	}

	//Medir Fin
	gettimeofday(&dateFin, NULL);

	for(i = 0; i < MAX_PACKS; i++){
		if(write(unix_fd, buf, BUF_SIZE) != BUF_SIZE) {
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