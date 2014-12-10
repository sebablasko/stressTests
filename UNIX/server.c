#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//Definiciones
#define BUF_SIZE 10
#define MAX_PACKS 1000000
#define MY_SOCK_PATH "unix_sock"

//Variables
int first_pack = 0;
struct timeval dateInicio, dateFin;
pthread_mutex_t lock;
int mostrarInfo = 0;
int NTHREADS = 1;
double segundos;

llamadaHilo(int unix_fd){
	char buf[BUF_SIZE];
	int lectura;

	int paquetesParaAtender = MAX_PACKS/NTHREADS;
	int i;
	for(i = 0; i < paquetesParaAtender; i++) {
		//lectura = recv(unix_fd, buf, BUF_SIZE, 0);
		lectura = read(unix_fd, buf, BUF_SIZE);
		if(lectura <= 0) {
			fprintf(stderr, "Error en el read del socket (%d)\n", lectura);
			exit(1);
		}
		if(first_pack==0) { 
			pthread_mutex_lock(&lock);
			if(first_pack == 0) {
				if(mostrarInfo)	printf("got first pack\n");
				first_pack = 1;
				//Medir Inicio
				gettimeofday(&dateInicio, NULL);
			}
			pthread_mutex_unlock(&lock);
		}
	}
}

main(int argc, char **argv){

	if(argc < 2){
		fprintf(stderr, "Syntax Error: Esperado: ./server NTHREADS\n");
		exit(1);
	}

	//Recuperar numero de Threads
	NTHREADS = atoi(argv[1]);
	pthread_t pids[NTHREADS];

	pthread_mutex_init(&lock, NULL);

	/* Abrir dispositivo */
	int unix_fd;
	unix_fd = unix_udp_bind(MY_SOCK_PATH);
	if(unix_fd < 0){
		fprintf(stderr, "Error al abrir el socket Unix");
		exit(1);
	}

	//Lanzar Threads
	int i;
	for(i=0; i < NTHREADS; i++) 
		pthread_create(&pids[i], NULL, llamadaHilo, unix_fd);

	//Esperar Threads
	for(i=0; i < NTHREADS; i++) 
		pthread_join(pids[i], NULL);

	gettimeofday(&dateFin, NULL);
	unlink(MY_SOCK_PATH);

	/* Resultados */
	segundos=(dateFin.tv_sec*1.0+dateFin.tv_usec/1000000.)-(dateInicio.tv_sec*1.0+dateInicio.tv_usec/1000000.);
	if(mostrarInfo){
		printf("Tiempo Total = %g\n", segundos);
		printf("QPS = %g\n", MAX_PACKS*1.0/segundos);
	}else{
		printf("%g, \n", segundos);
	}
	exit(0);
}