#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// Desarrolla un programa que al ejecutarse cree dos procesos hijos y cada uno de los hijos creará 3 nuevos procesos hijos. 
// De esta manera habrá un proceso padre, dos procesos hijos y seis procesos nieto. 
// Los hijos y los nietos deberán tener al final la instrucción sleep(20), pero el padre después de ejecutar la instrucción sleep(5) 
// deberá terminar a todos los hijos y nietos.

int main()
{
    int p;
    p=fork();
    int p2 = p;    // Respaldar pid del hijo 1
    if (p!=0) p=fork(); // Si soy el padre, tengo mi segundo hijo
    if (p==0){  // Si soy uno de los dos hijos...
        p=fork(); // Tendre mi primer hijo
        if (p!=0) p=fork(); // Mi segundo hijo
        if (p!=0) p=fork(); // Mi tercer hijo
        sleep(20); // y todos dormiremos 20 segundos
    }
    else { // Mientras tanto, el padre
        sleep(5); // dormira 5 segundos
        kill(-p, SIGKILL); // Assesina todos los procesos del mismo grupo
    }
    printf("Fin\n"); // Solo se deberia imprimir 1 vez
}