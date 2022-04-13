FLAGS += -Ofast -march=native
OMP_FLAGS += -fopenmp

all: main

main: main.o aes.o service.o
	g++ main.o aes.o service.o -o main $(FLAGS)

aes.o: aes.cpp aes.h
	g++ -c aes.cpp -o aes.o $(FLAGS)

service.o: service.cpp aes.h service.h
	g++ -c service.cpp -o service.o $(FLAGS)

main.o: main.cpp aes.h service.h
	g++ -c main.cpp -o main.o $(FLAGS)

gen: gen.cpp
	g++ gen.cpp -o gen $(FLAGS)

sbox: sbox.cpp
	g++ sbox.cpp -o sbox $(FLAGS) $(OMP_FLAGS)

clean:
	rm *.o main gen sbox
