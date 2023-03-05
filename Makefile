DIR_SRC = "src"
DIR_OBJ = "obj"

NAME_CLIENT = "client"
NAME_SERVER = "server"

L_LIBS = "-lpqxx -lpq"

all: ${NAME_CLIENT} ${NAME_SERVER}

${NAME_CLIENT}: ${NAME_CLIENT}.o nettools.o
	g++ -o ${NAME_CLIENT} ${NAME_CLIENT}.o nettools.o

${NAME_SERVER}: ${NAME_SERVER}.o nettools.o
	g++ -o ${NAME_SERVER} ${NAME_SERVER}.o nettools.o ${L_LIBS}

${NAME_CLIENT}.o: 
	g++ -c ./${DIR_SRC}/client.cpp -o ./${DIR_OBJ}/${NAME_CLIENT}.o 

${NAME_SERVER}.o:
	g++ -c ./${DIR_SRC}/server.cpp -o ./${DIR_OBJ}/${NAME_SERVER}.o 

nettools.o:
	g++ -c nettools.cpp -o ./${DIR_OBJ}/nettools.o 

clean:
	rm ${NAME_CLIENT} ${NAME_SERVER} ./${DIR_OBJ}/*.o