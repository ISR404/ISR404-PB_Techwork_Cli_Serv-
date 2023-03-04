all: client server

client: client.o nettools.o
	g++ -o client client.o nettools.o 

server: server.o nettools.o
	g++ -o server server.o nettools.o -lpqxx -lpq

client.o: 
	g++ -c client.cpp -o client.o 

server.o:
	g++ -c server.cpp -o server.o 

nettools.o:
	g++ -c nettools.cpp -o nettools.o 

clean:
	rm client server *.o