all: client server

client: client.o nettools.o
	g++ client.o nettools.o -o client

server: server.o nettools.o
	g++ server.o nettools.o -o server

client.o: 
	g++ -c client.cpp -o client.o

server.o:
	g++ -c server.cpp -o server.o

nettools.o:
	g++ -c nettools.cpp -o nettools.o

clean:
	rm client server *.o