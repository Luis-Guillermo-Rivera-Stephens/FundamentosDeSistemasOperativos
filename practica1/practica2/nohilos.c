#include <stdio.h>
#include <math.h>
#include <time.h> 

double raiz(double x, double y){  //Esta funcion va a calcular la altura del punto x que usamos, porque 
                                  //R^2 = y^2 + x^2 y si despejamos j nos da una funcion de x
    return sqrt(y*y - x*x);
}

double sistema_de_trapecios(double radio, int n){ //calculara el area de todos los trapecios
    double area = 0.0;
    double a = 0.0;
    double b = radio;
    double h = (b - a) / n;

    area = raiz(a, radio) + raiz(b, radio); //segun la regla de los trapecios se tienen que sumar la primera
                                            //y la ultima altura primero
    for (int i = 1; i < n; i++) { 
        double x_i = a + i * h; 
        area += 2 * raiz(x_i, radio);       //calcula la altura de cada x que le enviamos  y la suma al area
    }

    area = (h / 2) * area;                  //multiplica todas las alturas por la base, pues xa + xb +xc = x(a + b + c)

    return area;
}

int main(){
    double radio;
    int n;
    double resultado;

    //variables para contabilizar el tiempo 
    clock_t start_time, end_time;
    double cpu_time_used;

    printf("Introduce el radio que desees utilizar: ");
    scanf("%lf", &radio);
    printf("En cuantos trapecios quieres dividirlo: ");
    scanf("%d", &n);

    start_time = clock();
    resultado = sistema_de_trapecios(radio, n); //inicio de la funcion
    end_time = clock();
    
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    //resultados 
    printf("Esta es el área del cuarto de círculo: %f\n", resultado);
    printf("Esta es el área del círculo completo: %f\n", resultado * 4);
    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);

    return 0;
}
