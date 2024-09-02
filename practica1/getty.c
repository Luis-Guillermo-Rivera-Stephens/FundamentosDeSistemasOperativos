#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

int validate_username(char *username, char *password){
    FILE *f=fopen("passwd.txt","r");
    char line[256];
    
    if(!f)
    {
        printf("Error al abrir el archivo passwd");
        return 0;
    }

    while (fgets(line, sizeof(line),f)!= NULL)
    {
        char *user =strtok(line,":");
        char *pass =strtok(NULL,"\n");

        if (strcmp(user,username)==0 && strcmp(pass,password)==0)
        {   
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void getty(int pid_padre){
    char username[256];
    char password[256];

    while(1){
        printf("Username: ");
        fgets(username,sizeof(username),stdin);
        username[strcspn(username,"\n")]='\0';

        printf("Password: ");
        fgets(password,sizeof(username),stdin);
        password[strcspn(password,"\n")]='\0';

        if (validate_username(username,password))
        {
            int pid=fork();

            if (pid==0){
                execlp("./sh.exe","sh.exe", pid_padre ,NULL);
            }
            else{
                wait(NULL);
                printf("Volviendo a iniciar cuenta\n");
            }
        }
        else{
            printf("Contraseña o usuario incorrecto, intente de nuevo");
        }
        

    }
}

int main(int argc){
    getty(argc);
}