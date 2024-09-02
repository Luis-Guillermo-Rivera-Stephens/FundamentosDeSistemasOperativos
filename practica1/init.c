#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>

void killsignal(){
    printf("Eliminando proceso padre");
    kill(0, SIGKILL);
}


int init(){

    int pid = getpid();  
    FILE *archivo = fopen("pid.txt", "w"); 

    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    fprintf(archivo, "%d\n", pid); 
    fclose(archivo);
    

    for (int i = 0; i<6; i++) {
        if (fork() == 0) {
            execlp("xterm", "xterm", "-e", "./getty.exe", NULL);
        }
    }
    while(1) {
        wait(NULL);
        if (fork() == 0) {
            execlp("xterm", "xterm", "-e", "./getty.exe", NULL);
        }
    }
}

int main(){
    init();
}