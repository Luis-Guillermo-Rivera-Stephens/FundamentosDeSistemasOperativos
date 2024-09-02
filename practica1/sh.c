#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
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
            kill(0, SIGKILL);
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