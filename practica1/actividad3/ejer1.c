#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int p = fork(); 

    for(int i = 0; i < 10; i++){
        if(p == 0) 
        {
            printf("Soy el hijo \n"); 
            sleep(1);
        }
        else 
        {
            printf("Soy el padre \n");
            sleep(1);
        }
    }

    if (p!= 0) {
        p = wait(NULL); 
        printf("Mi proceso hijo ya ha terminado \n");
    }
}