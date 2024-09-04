#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) { 
    int n = atoi(argv[1]) + 1; 

    for(int i = 0; i < n - 1; i++){
        int p = fork(); 
        if(p == 0){ 
            printf("Proceso hijo %d \n", i + 1);
            exit(0);
        }
        sleep(1);
    }
    wait(NULL);
    printf("Fin de la ejecucion \n");
}