/*
Nuria García García           n.ggarcia@udc.es
Miguel Magdalena Sánchez      m.magdalenas@udc.es
Isabel Villar García          i.villar@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

//Algoritmo 1 O(n^2)
int sumaSubMax1 (int v[], int n){
    int sumaMax = 0, estaSuma;
    int i, j;

    for ( i=0; i<n; i++){
        estaSuma = 0;
        for(j=i; j<n; j++){
            estaSuma += v[j];
            if (estaSuma > sumaMax) sumaMax = estaSuma;
        }
    }

    return sumaMax;
}

//Algoritmo 2 O(n)
int sumaSubMax2 (int v[], int n){
    int estaSuma = 0;
    int sumaMax = 0;
    int j;

    for (j=0; j<n; j++){
        estaSuma += v[j];
        if (estaSuma > sumaMax) sumaMax = estaSuma;
        else if (estaSuma < 0) estaSuma = 0;
    }

    return sumaMax;
}

//Se establece la semilla de una nueva serie de enteros pseudo-aleatorios 
void inicializar_semilla() {
    srand(time(NULL));
}

//Se generan números pseudoaleatorio entre -n y +n 
void aleatorio(int v[], int n) {
    int i, m=2*n+1;

    for (i=0; i < n; i++) v[i] = (rand() % m) - n;
}

//Obtiene la hora del sistema en microsegundos 
double microsegundos() {
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

//Imprime con formato un vector de n elementos
void imprimirVector (int v[], int n){
    int i;

    printf("[ ");
    for(i=0; i<n; i++) printf("%3d ", v[i]);
    printf("%2c", ']');
}

void test1(){
    int i;
    int v[6][5] = {
        {-9, 2, -5, -4, 6},
        {4, 0, 9, 2, 5},
        {-2, -1, -9, -7, -1},
        {9, -2, 1, -7, -8},
        {15, -2, -5, -4, 16},
        {7, -5, 6, 7, -7}
    };

    printf("Test 1\n");
    printf("\t\t\t\tsumaSubMax1\tsumaSubMax2\n");

    for(i = 0; i < 6; i++){
        imprimirVector(v[i],5);
        printf("%19d %15d\n", sumaSubMax1(v[i], 5), sumaSubMax2(v[i], 5));
    }
}

void test2(){
    int fil = 10;
    int col = 9;   
    int v[fil][col];
    int i;

    for (i = 0; i < fil; i++) aleatorio(v[i], col);

    printf("\nTest 2\n");
    printf("\t\t\t\t\t\tsumaSubMax1\tsumaSubMax2\n");

    for(i = 0; i < fil; i++){
        imprimirVector(v[i], col);
        printf("%19d %15d\n", sumaSubMax1(v[i], col), sumaSubMax2(v[i], col));
    }
}


double medir_tiempo_vector(int n, int (*func)(int [], int)){
    int i;
    double ta, tb, t, t1, t2;
    int k = 1000;
    int v[n];

    aleatorio(v, n);
    ta = microsegundos();
    func(v, n); //Ejecutamos el algoritmo correspondiente
    tb = microsegundos();
    t = tb - ta;

    if (t<500){ //Si no llega al umbral de confianza
        printf("(*)");
        ta = microsegundos();
        for (i=0;i<k;i++){
            aleatorio(v,n);
            func(v,n);
        }
        tb = microsegundos();
        t1 = tb - ta;
        ta = microsegundos();
        for (i=0;i<k;i++){
            aleatorio(v,n);
        }
        tb = microsegundos();
        t2 = tb - ta;
        t = (t1 - t2)/k;
    }

    return t;

}

void medicion(int (*func)(int[], int), int mode){  
    //mode indica si es el primer o el segundo algoritmo
    int n = 500, i;
    double t;
    float tinf, taj, tsup;
    int limite;

    printf("\n");
    if (!mode)
        printf("%9s%16s%22s%17s%20s\n", "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");
    else
        printf("%11s%16s%20s%20s%20s\n", "n", "t(n)", "t(n)/n^0.8", "t(n)/n", "t(n)/(nlog(n))");
    
    if(!mode) limite = 7;
    else limite = 10;

    for(i = 0; i<limite; i++){
        t = medir_tiempo_vector(n, func);
        if(!mode){
            tinf = t/pow(n,1.8);
            taj = t/pow(n,2);
            tsup = t/pow(n,2.2);
        }
        else{
            tinf = t/pow(n,0.8);
            taj = t/n;
            tsup = t/(n*log(n));
        }
        
        printf("%10d%16.3f%18.6f%20.6f%18.6f\n", n, t, tinf, taj, tsup);

        n*=2; //Progresión geométrica de razón 2
    }
}


int main(){
    int a = 5;

    inicializar_semilla();
    test1();
    test2();

    while(a--){
        printf("\nSumaSubMax1----------------------\n");
        medicion(sumaSubMax1, 0);
        printf("\nSumaSubMax2----------------------\n");
        medicion(sumaSubMax2, 1);
        printf("\n\n");
    }
}
