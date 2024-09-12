#include <stdio.h>
#include <math.h>
#include <time.h> 

double raiz(double x, double y){ 
    return sqrt(y*y - x*x);
}

double sistema_de_trapecios(double radio, int n){
    double area = 0.0;
    double a = 0.0;
    double b = radio;
    double h = (b - a) / n;

    area = raiz(a, radio) + raiz(b, radio);

    for (int i = 1; i < n; i++) { 
        double x_i = a + i * h; 
        area += 2 * raiz(x_i, radio);  
    }

    area = (h / 2) * area; 

    return area;
}

int main(){
    double radio;
    int n;
    double resultado;

    clock_t start_time, end_time;
    double cpu_time_used;

    printf("Introduce el radio que desees utilizar: ");
    scanf("%lf", &radio);
    printf("En cuantos trapecios quieres dividirlo: ");
    scanf("%d", &n);

    start_time = clock();
    resultado = sistema_de_trapecios(radio, n);
    end_time = clock();
    
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Esta es el área del cuarto de círculo: %f\n", resultado);
    printf("Esta es el área del círculo completo: %f\n", resultado * 4);
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    return 0;
}
