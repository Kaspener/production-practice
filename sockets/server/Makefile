server: server.o socketFunctions.o
	gcc server.o socketFunctions.o -o server -pthread -lmysqlclient

server.o: server.c
	gcc -c server.c

socketFunctions.o: socketFunctions.c
	gcc -c socketFunctions.c

clean:
	rm -rf *.o server
