#include <stdio.h>
#include <math.h>
#include <pthread.h>


double area = 0.0;
pthread_mutex_t mutex;  

// Función para calcular la altura en ese punto, donde y siempre sera el Radio
double raiz(double x, double y) {
    return sqrt(y * y - x * x);
}

typedef struct {
    double a;
    double h;
    double radio;
    int i;
} HiloArgs;

// Función que ejecutarán los hilos
void* calcular_trapecio(void* args) {
    HiloArgs* datos = (HiloArgs*) args;
    double x_i = datos->a + datos->i * datos->h; //base, o x donde termina esa base
    double resultado = 2 * raiz(x_i, datos->radio);  // Calcular el área para el punto x_i

    pthread_mutex_lock(&mutex);
    area += resultado;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

double sistema_de_trapecios(double radio, int n) {
    double a = 0.0;
    double b = radio;
    double h = (b - a) / n;

    // Inicializar el área con los extremos
    pthread_mutex_lock(&mutex);
    area = raiz(a, radio) + raiz(b, radio);
    pthread_mutex_unlock(&mutex);

    pthread_t hilos[n];  // Arreglo de hilos
    HiloArgs args[n];  // Arreglo para almacenar los argumentos de cada hilo
    HiloArgs aux;

    // Crear los hilos para calcular los trapecios
    for (int i = 1; i < n; i++) {
        aux.a = a;
        aux.h = h;
        aux.radio = radio;
        aux.i = i;
        
        args[i] = aux;
        pthread_create(&hilos[i], NULL, calcular_trapecio, (void*)&args[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 1; i < n; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Calcular el área final
    pthread_mutex_lock(&mutex);
    area = (h / 2) * area;
    pthread_mutex_unlock(&mutex);

    return area;
}

int main() {
    double radio;
    int n;
    double resultado;

    // Inicializar el mutex
    pthread_mutex_init(&mutex, NULL);

    // Pedir datos al usuario
    printf("Introduce el radio que desees utilizar: ");
    scanf("%lf", &radio);
    printf("¿En cuántos trapecios quieres dividirlo? ");
    scanf("%d", &n);

    // Calcular el área
    resultado = sistema_de_trapecios(radio, n);

    printf("Esta es el área del cuarto de círculo: %f\n", resultado);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
