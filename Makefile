FLAGS += -Ofast -march=native

all: main

main: main.o aes.o service.o
	g++ main.o aes.o service.o -o main $(FLAGS)

aes.o: aes.cpp aes.h
	g++ -c aes.cpp -o aes.o $(FLAGS)

service.o: service.cpp aes.h service.h
	g++ -c service.cpp -o service.o $(FLAGS)

main.o: main.cpp aes.h service.h
	g++ -c main.cpp -o main.o $(FLAGS)

clean:
	rm *.o main
