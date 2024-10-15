#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000

typedef struct {
    double sums[NPROCS];
    double x_val;
    double res;
} SHARED;

SHARED *shared;


sem_t *sem_master, *sem_prod;

double get_member(int n, double x) {
    int i;
    double numerator = 1;
    for (i = 0; i < n; i++)
        numerator = numerator * x;
    if (n % 2 == 0)
        return (-numerator / n);
    else
        return (numerator / n);
}

void proc(int proc_num) {
    printf("Starting production process #%d", proc_num);
    int i;
    
    // Espera a la señal del maestro para empezar
    sem_wait(sem_master);
    printf("Signal recieved");
    
    // Cada proceso realiza su cálculo
    shared->sums[proc_num] = 0;
    for (i = proc_num; i < SERIES_MEMBER_COUNT; i += NPROCS) {
        shared->sums[proc_num] += get_member(i + 1, shared->x_val);
    }
    
    // Señala al maestro que ha terminado
    printf("Sending a signal to the master process");
    sem_post(sem_prod);
    printf("Exit process %d", proc_num);
    exit(0);
}

void master_proc() {
    int i;
    printf("Starting master process");
    // Leer valor de x desde el archivo
    FILE *fp = fopen("entrada.txt", "r");
    if (fp == NULL)
        exit(1);
    
    fscanf(fp, "%lf", &shared->x_val);
    fclose(fp);
    
    // Señala a los hijos para que comiencen
    for (i = 0; i < NPROCS; i++) {
        printf("Allowing 1+");
        sem_post(sem_master);  // Desbloquea a todos los hijos
    }
    
    // Espera a que todos los hijos terminen
    for (i = 0; i < NPROCS; i++) {
        printf("waiting production %d....", i);
        sem_wait(sem_prod);
    }
    
    // Suma los resultados finales
    shared->res = 0;
    for (i = 0; i < NPROCS; i++) {
        shared->res += shared->sums[i];
    }
    exit(0);
}

int main() {
    int i, p, shmid;
    int status;
    
    // Crear memoria compartida
    shmid = shmget(0x1234, sizeof(SHARED), 0666 | IPC_CREAT);
    shared = shmat(shmid, NULL, 0);
    

    sem_master = sem_open("/sem_master", O_CREAT, 0666, 0); 
    sem_prod = sem_open("/sem_prod", O_CREAT, 0666, 0);

    for (i = 0; i < NPROCS; i++) {
        p = fork();
        if (p == 0)
            proc(i);
    }
    
    
    p = fork();
    if (p == 0)
        master_proc();
    
    
    for (int i = 0; i < NPROCS + 1; i++) {
        wait(&status);
    }
    
    
    printf("El resultado es %10.8f\n", shared->res);
    printf("ln(1 + %f) = %10.8f\n", shared->x_val, log(1 + shared->x_val));
    
    
    sem_close(sem_master);
    sem_close(sem_prod);
    sem_unlink("/sem_master");
    sem_unlink("/sem_prod");
    
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);
    
    return 0;
}