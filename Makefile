CPP_CMP = g++

DIR_SRC = src
DIR_OBJ = obj

NAME_CLIENT = client
NAME_SERVER = server

L_LIBS = -lpqxx -lpq


all: ${NAME_CLIENT} ${NAME_SERVER}

${NAME_CLIENT}: ${NAME_CLIENT}.o nettools.o
	${CPP_CMP} -o ${NAME_CLIENT} ./${DIR_OBJ}/${NAME_CLIENT}.o ./${DIR_OBJ}/nettools.o

${NAME_SERVER}: ${NAME_SERVER}.o nettools.o srvcmd.o
	${CPP_CMP} -o ${NAME_SERVER} ./${DIR_OBJ}/${NAME_SERVER}.o ./${DIR_OBJ}/nettools.o ./${DIR_OBJ}/srvcmd.o

${NAME_CLIENT}.o: nettools.o
	${CPP_CMP} -Iheaders -c ./${DIR_SRC}/client.cpp -o ./${DIR_OBJ}/${NAME_CLIENT}.o 

${NAME_SERVER}.o: nettools.o srvcmd.o
	${CPP_CMP} -Iheaders -c ./${DIR_SRC}/server.cpp -o ./${DIR_OBJ}/${NAME_SERVER}.o 

nettools.o: srvcmd.o
	${CPP_CMP} -Iheaders -c ./${DIR_SRC}/nettools.cpp ./${DIR_OBJ}/srvcmd.o -o ./${DIR_OBJ}/nettools.o 

srvcmd.o:
	${CPP_CMP} -Iheaders -c ./${DIR_SRC}/srvcmd.cpp -o ./${DIR_OBJ}/srvcmd.o ${L_LIBS}

clean:
	rm ${NAME_CLIENT} ${NAME_SERVER} ./${DIR_OBJ}/*.o