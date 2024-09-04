#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){

    while(1){
        char command[10];
        printf("Comando a ejecutar: ");
        scanf("%s", command); 

        if(strcmp(command, "exit") == 0){
            printf("Saliendo del shell \n");
            break;
        }
        system(command);
        printf("\n");
    }
    printf("Fin de la ejecucion \n");
}