# Variables
CC=gcc

all: start clean init.exe sh.exe getty.exe
	./init.exe

init.exe: init.o
	$(CC) -o init.exe init.o

sh.exe: sh.o
	$(CC) -o sh.exe sh.o

getty.exe: getty.o
	$(CC) -o getty.exe getty.o

init.o: init.c
	$(CC) -c init.c

sh.o: sh.c
	$(CC) -c sh.c

getty.o: getty.c
	$(CC) -c getty.c

clean:
	rm -f *.o *.exe

start:
	echo "Iniciando el proceso de compilacion"
