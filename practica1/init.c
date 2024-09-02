#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>



int init(){
    for (int i = 0; i<6; i++) {
        if (fork() == 0) {
            execlp("xterm", "xterm", "-e", "./getty.exe", NULL);
        }
    }
    wait(NULL);
    kill(0, SIGKILL)
}

int main(){
    init();
}