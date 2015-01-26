#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//Definiciones
#define BUF_SIZE 10
#define FIRST_PORT 1820

//Variables
int first_pack = 0;
struct timeval dateInicio, dateFin;
pthread_mutex_t lock;
int mostrarInfo = 0;
int MAX_PACKS = 1;
int NTHREADS = 1;
double segundos;

llamadaHilo(int dev_fd){
	char buf[BUF_SIZE];
	int lectura;

	int paquetesParaAtender = MAX_PACKS/NTHREADS;
	int i;
	for(i = 0; i < paquetesParaAtender; i++) {
		//lectura = recv(dev_fd, buf, BUF_SIZE, 0);
		lectura = read(dev_fd, buf, BUF_SIZE);
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

	if(argc < 3){
		fprintf(stderr, "Syntax Error: Esperado: ./dev_null MAX_PACKS NTHREADS\n");
		exit(1);
	}

	//Recuperar total de paquetes a enviar
	MAX_PACKS = atoi(argv[1]);

	//Recuperar numero de Threads
	NTHREADS = atoi(argv[2]);
	pthread_t pids[NTHREADS];

	pthread_mutex_init(&lock, NULL);

	/* Abrir dispositivo */
	int dev_fd;
	dev_fd = open("/dev/null", 0);
	if(dev_fd < 0){
		fprintf(stderr, "Error al abrir el dispositivo");
		exit(1);
	}

	//Lanzar Threads
	int i;
	for(i=0; i < NTHREADS; i++) 
		pthread_create(&pids[i], NULL, llamadaHilo, dev_fd);

	//Esperar Threads
	for(i=0; i < NTHREADS; i++) 
		pthread_join(pids[i], NULL);

	gettimeofday(&dateFin, NULL);

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