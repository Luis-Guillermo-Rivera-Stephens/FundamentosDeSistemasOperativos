#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h> 


double area = 0.0;
pthread_mutex_t mutex;  //mutex para que los hilos no escriban a la vez en la variable de area

double raiz(double x, double y) {   //Esta funcion va a calcular la altura del punto x que usamos, porque 
                                    //R^2 = y^2 + x^2 y si despejamos j nos da una funcion de x
    return sqrt(y * y - x * x);
}

typedef struct {        //Los argumentos que va a usar cada hilo para que pueda hacer el calculo desde afuera
    double a;
    double h;
    double radio;
    int i;
} HiloArgs;

void* calcular_trapecio(void* args) {
    HiloArgs* datos = (HiloArgs*) args;
    double x_i = datos->a + datos->i * datos->h; 
    double resultado = 2 * raiz(x_i, datos->radio);  

    pthread_mutex_lock(&mutex);
    area += resultado;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);     //Hace la operacion con cada n dada, donde le va a sumar la altura al area para una vez 
                            //terminada la ejecucion, lo multiplique por la base
}

double sistema_de_trapecios(double radio, int n) {
    double a = 0.0;
    double b = radio;
    double h = (b - a) / n;

    pthread_mutex_lock(&mutex); //MUTEX PARA LOCKEAR LA VARIABLE GLOBAL AREA 
    //segun la regla de los trapecios se tienen que sumar la primera
    //y la ultima altura primero
    area = raiz(a, radio) + raiz(b, radio);
    pthread_mutex_unlock(&mutex);

    
    pthread_t hilos[n];  
    HiloArgs args[n];  
    HiloArgs aux;

    for (int i = 1; i < n; i++) {
        aux.a = a;
        aux.h = h;
        aux.radio = radio;
        aux.i = i;

        args[i] = aux; //creacion de los argumentos para los hilos, 
        pthread_create(&hilos[i], NULL, calcular_trapecio, (void*)&args[i]);  //Creacion de hilos por cada iteracion
            // Se supone que solo son 4 cores pero esto es mas escalable en caso de querer correrlo en mas cores
    }

    for (int i = 1; i < n; i++) {
        pthread_join(hilos[i], NULL); //espera a que todos los hilos terminen para poder multiplicar todo por la base
    }

    pthread_mutex_lock(&mutex);
    area = (h / 2) * area; //se multiplica toda la sumatoria por la base
    pthread_mutex_unlock(&mutex);

    return area;
}

int main() {
    double radio;
    int n;
    double resultado;

    pthread_mutex_init(&mutex, NULL); //se inicializa el mutex

    printf("Introduce el radio que desees utilizar: ");
    scanf("%lf", &radio);
    printf("¿En cuántos trapecios quieres dividirlo? ");
    scanf("%d", &n);

    clock_t start_time, end_time;
    double cpu_time_used;


    start_time = clock();
    resultado = sistema_de_trapecios(radio, n);
    end_time = clock();

    printf("Esta es el área del cuarto de círculo: %f\n", resultado);
    printf("Esta es el área del círculo completo: %f\n", resultado * 4);
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    pthread_mutex_destroy(&mutex);

    return 0;
}
