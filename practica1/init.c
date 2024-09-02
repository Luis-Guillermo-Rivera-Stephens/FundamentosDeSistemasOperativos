#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>

void killinit(){
    printf("kill all the process");
    kill(0, SIGKILL);
}

int init(){

    signal(SIGTERM, killinit);

    int pid_padre = getpid();
    for (int i = 0; i<6; i++) {
        if (fork() == 0) {
            execlp("xterm", "xterm", "-e", "./getty.exe", pid_padre, NULL);
        }
    }
    while(1){
        wait(NULL);
        if (fork() == 0) {
            execlp("xterm", "xterm", "-e", "./getty.exe", pid_padre, NULL);
        }
    }
}

int main(){
    init();
}