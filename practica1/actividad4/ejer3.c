#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Desarrolla un programa donde un proceso padre crea un hijo, el padre tendrá una duración de 1 segundo y el hijo una duración de 20 segundos.

int main()
{
    int p;
    p=fork();
    if (p==0) sleep(20);
    else sleep(1);
}