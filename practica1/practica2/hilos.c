#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <time.h> 


double area = 0.0;
pthread_mutex_t mutex;  

double raiz(double x, double y) {
    return sqrt(y * y - x * x);
}

typedef struct {
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

    pthread_exit(NULL);
}

double sistema_de_trapecios(double radio, int n) {
    double a = 0.0;
    double b = radio;
    double h = (b - a) / n;

    pthread_mutex_lock(&mutex);
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

        args[i] = aux;
        pthread_create(&hilos[i], NULL, calcular_trapecio, (void*)&args[i]);
    }

    for (int i = 1; i < n; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_lock(&mutex);
    area = (h / 2) * area;
    pthread_mutex_unlock(&mutex);

    return area;
}

int main() {
    double radio;
    int n;
    double resultado;

    pthread_mutex_init(&mutex, NULL);

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
