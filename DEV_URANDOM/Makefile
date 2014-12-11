all: devurandom

devurandom: dev_urandom.o
	gcc -g dev_urandom.o -o dev_urandom -lpthread 

clean:
	rm dev_urandom dev_urandom.o