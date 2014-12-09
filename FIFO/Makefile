all: server client

server: server.o
	gcc server.o -o server -lpthread

rm_server:
	rm server server.o

client: client.o
	gcc client.o -o client -lpthread

rm_client:
	rm client client.o

clean: rm_client rm_server