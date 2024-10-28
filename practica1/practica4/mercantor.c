#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#define NPROCS 4
#define SERIES_MEMBER_COUNT 200000
#define PROC_MSGTYPE 1
#define MASTER_MSGTYPE 2

struct msg_buffer {
    long mtype;
};

typedef struct {
    double sums[NPROCS];
    int proc_count;
    double x_val;
    double res;
} SHARED;

SHARED *shared;
int proc_qid;
int master_qid;

double get_member(int n, double x) {
    int i;
    double numerator = 1;
    for(i=0; i<n; i++)
        numerator = numerator*x;
    if (n % 2 == 0)
        return (-numerator / n);
    else
        return numerator/n;
}

void proc(int proc_num) {
    int i;
    struct msg_buffer msg_rcv, msg_send;
    
    msg_rcv.mtype = PROC_MSGTYPE;
    msgrcv(proc_qid, &msg_rcv, 0, PROC_MSGTYPE, 0);
    
    shared->sums[proc_num] = 0;
    for(i=proc_num; i<SERIES_MEMBER_COUNT;i+=NPROCS)
        shared->sums[proc_num] += get_member(i+1, shared->x_val);
    
    msg_send.mtype = MASTER_MSGTYPE;
    msgsnd(master_qid, &msg_send, 0, IPC_NOWAIT);
    
    exit(0);
}

void master_proc() {
    int i;
    struct msg_buffer msg_send, msg_rcv;
    
    FILE *fp = fopen("entrada.txt","r");
    if(fp==NULL)
        exit(1);
    
    fscanf(fp,"%lf",&shared->x_val);
    fclose(fp);
    
    msg_send.mtype = PROC_MSGTYPE;
    
    for(i = 0; i < NPROCS; i++) {
        msgsnd(proc_qid, &msg_send, 0, IPC_NOWAIT);
    }
    
    for(i = 0; i < NPROCS; i++) {
        msgrcv(master_qid, &msg_rcv, 0, MASTER_MSGTYPE, 0);
    }
    
    shared->res = 0;
    for(i=0; i<NPROCS; i++)
        shared->res += shared->sums[i];
    exit(0);
}

int main() {
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    struct timeval ts;
    int i;
    int p;
    int shmid;
    int status;
    key_t proc_key = 1234;
    key_t master_key = 1235;
    
    proc_qid = msgget(proc_key, IPC_CREAT | 0666);
    master_qid = msgget(master_key, IPC_CREAT | 0666);
    
    shmid = shmget(0x1234,sizeof(SHARED),0666|IPC_CREAT);
    shared = shmat(shmid,NULL,0);
    
    shared->proc_count = 0;
    gettimeofday(&ts, NULL);
    
    start_ts = ts.tv_sec;
    
    for(i=0; i<NPROCS;i++) {
        p = fork();
        if(p==0)
            proc(i);
    }
    
    p = fork();
    if(p==0)
        master_proc();
    
    printf("El recuento de ln(1 + x) miembros de la serie de Mercator es %d\n",SERIES_MEMBER_COUNT);
    
    for(int i=0;i<NPROCS+1;i++) {
        wait(&status);
        if(status==0x100) {
            fprintf(stderr,"Proceso no puede abrir el archivo de entrada\n");
            break;
        }
    }
    
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;
    elapsed_time = stop_ts - start_ts;
    
    printf("Tiempo = %lld segundos\n", elapsed_time);
    printf("El resultado es %10.8f\n", shared->res);
    printf("Llamando a la función ln(1 + %f) = %10.8f\n",shared->x_val, log(1+shared->x_val));
    
    shmdt(shared);
    shmctl(shmid,IPC_RMID,NULL);
    msgctl(proc_qid, IPC_RMID, NULL);
    msgctl(master_qid, IPC_RMID, NULL);
    
    return 0;
}