.PHONY: all clean
all : client pollserver

client: client.o
	g++ -o client client.o 

pollserver: pollserver.o
	g++ -o pollserver pollserver.o -pthread

client.o: client.cpp
	g++ -c client.cpp  

pollserver.o: pollserver.cpp
	g++ -c pollserver.cpp -pthread 

clean:
	rm -f *.o pollserver client
