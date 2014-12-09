all: null urandom

null: dev_null.o
	gcc -g dev_null.o -o dev_null -lpthread

clean_null:
	rm dev_null dev_null.o

urandom: dev_urandom.o
	gcc -g dev_urandom.o -o dev_urandom -lpthread 

clean_urandom:
	rm dev_urandom dev_urandom.o

clean: clean_urandom clean_null