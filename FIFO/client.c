#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Definiciones
#define BUF_SIZE 10
#define FIFOPIPENAME "test_pipe"

//Variables
struct timeval dateInicio, dateFin;
char buf[BUF_SIZE];
int mostrarInfo = 0;
int MAX_PACKS = 1;
double segundos;

main(int argc, char **argv) {

	if(argc < 2){
		fprintf(stderr, "Syntax Error: Esperado: ./client MAX_PACKS\n");
		exit(1);
	}

	//Recuperar total de paquetes a enviar
	MAX_PACKS = atoi(argv[1]);

	/* Llenar de datos el buffer a enviar */
	int i;
	for(i = 0; i < BUF_SIZE; i++)
		buf[i]='a'+i;

	/* Abrir dispositivo */
	int fifo_fd;
	fifo_fd = open(FIFOPIPENAME, 1);
	if(fifo_fd < 0){
		fprintf(stderr, "Error al abrir el pipe\n");
		exit(1);
	}

	//Medir Fin
	gettimeofday(&dateInicio, NULL);

	for(i = 0; i < MAX_PACKS; i++){
		if(write(fifo_fd, buf, BUF_SIZE) != BUF_SIZE) {
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