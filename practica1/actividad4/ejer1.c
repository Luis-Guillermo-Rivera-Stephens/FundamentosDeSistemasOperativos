#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Desarrolla un programa donde un proceso padre crea un proceso hijo, al iniciar este proceso hijo, deberá matar o terminar a su padre.

int main()
{
    int p;

    p=fork();
    if(p==0) 
    {
        kill(getppid(), SIGKILL);
        exit(0);
    }
    else {
        printf("Mi hijo esta a punto de ejecutarme\n");
    }
    wait(NULL);
    printf("Esta linea no deberia de imprimirse\n");
}