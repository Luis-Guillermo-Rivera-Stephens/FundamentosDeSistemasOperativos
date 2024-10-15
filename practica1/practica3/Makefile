CC=gcc

all: start clean mercator.exe exec

exec:
	.\mercator.exe

mercator.exe: mercator.o
        $(CC)-o mercator.exe mercator.o -lm

mercator.o: mercator.c
        $(CC) -c mercator.c

clean:
        rm -f *.o *.exe

start:
        echo "Iniciando el proceso de compilacion"