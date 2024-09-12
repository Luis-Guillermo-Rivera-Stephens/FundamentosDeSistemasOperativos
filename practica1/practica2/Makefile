# Variables
CC=gcc

all: start clean init.exe sh.exe getty.exe




nohilos.exe: nohilos.o
	$(CC) -o nohilos.exe nohilos.o -lm

hilos.exe: hilos.o
	$(CC) -o hilos.exe hilos.o -lm

nohilos.o: nohilos.c
	$(CC) -c nohilos.c

hilos.o: hilos.c
	$(CC) -c hilos.c

clean:
	rm -f *.o *.exe

start:
	echo "Iniciando el proceso de compilacion"
