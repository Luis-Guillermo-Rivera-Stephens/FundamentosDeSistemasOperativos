#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {

    FILE *archivo = fopen("pid.txt", "r"); 
    if (archivo == NULL) {
        printf("Error al abrir el archivo");
        return -1;
    }

    int pid;
    if (fscanf(archivo, "%d", &pid) != 1) {  
        printf("Error al leer el PID del archivo");
        fclose(archivo);
        return 0;
    }

    fclose(archivo);
    
    char command[100];
    char *args[20];

    while (1) {
        printf("Programa a ejecutar: ");
        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            printf("Saliendo...\n");
            break;
        }
        if (strcmp(command, "shutdown") == 0) {
            printf("Apagando...\n");
            kill(pid, SIGTERM);
        }

        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL && i < 9) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL;  

        int p = fork();
        if (p == 0) {
            execvp(args[0], args);  

        } else {
            wait(NULL);
        } 
    }
    return 0;
}