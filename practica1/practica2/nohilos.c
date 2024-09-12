#include <stdio.h>
#include <math.h>


double raiz(double x, double y){ //calcula la altura de ese punto, puesto que y^2 + x^2 = R^2 y si despejamos nos puede dar y en ese punto 
    return sqrt(y*y - x*x);
}

double sistema_de_trapecios(double radio, int n){
    double area=0.0;
    double a=0.0;
    double b=radio;
    double h= (b-a)/n;

    area = raiz(a,radio)+raiz(b,radio);

    for (int i = 1; i < n; i++) { 
        double x_i = a + i * h; // Calcular el punto x_i
        area += 2 * raiz(x_i, radio);  // Sumar 2 * f(x_i)
    }

    area = (h/2)*area; //aqui multiplica todas las alturas por el ancho comun para sacar el area
    

    return area;
}


int main(){

    double radio;
    int n;
    double resultado;

    printf("Introduce el radio que desees utilizar: ");
    scanf("%lf",&radio);
    printf("En cuantos trapecios quieres dividirlo: ");
    scanf("%d",&n);

    resultado=sistema_de_trapecios(radio,n);

    printf("Esta es el area del cuarto de circulo: %f",resultado);


    return 0;
}
