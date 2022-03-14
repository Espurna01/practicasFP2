#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define P 50000
#define N_VEGADES 100

bool esTriangularforcaBruta(unsigned int x){
    unsigned int nTriangular = 1;
    for(int i = 2; nTriangular < x; i++){
        nTriangular += i;
    }
    return nTriangular == x;
}

bool esTriangularOptima(unsigned int x){
    unsigned int arrel = 8*x + 1;
    unsigned int i = 1;
    while(i*i < arrel){
        i++;
    }
    return i*i == arrel;
}

bool esTriangularTaula(unsigned int x, unsigned int taula[], int max){
    if(x > taula[max - 1]) return false;
    int i = 0;
    for(;i < max;i++){
        if(x == taula[i])
            return true;
    }
    return false;
}

int calculaMax(){
    int i = P;
    while(!esTriangularOptima(i)){
        i--;
    }
    int arrel = 8*i + 1;
    i = 1;
    while(i * i != arrel)
        i++;
    return (i - 1)/2;
}

void iniTaula(unsigned int taula[], int max){
    unsigned int i = 1;
    int index = 0;
    while(i < P && index < max){
        if(esTriangularOptima(i)){
            taula[index] = i;
            index++;
        }
        i++;
    }
}

int main()
{
    clock_t start, end;
    int max = calculaMax();
    unsigned int taula[max];
    iniTaula(taula, max);

    unsigned int n;
    double timeFB = 0, timeOp = 0, timeTa = 0;
    do{
        printf("Esciru un nombre > 0: ");
        scanf(" %u", &n);
    }while(n <= 0);
    unsigned int valn = n;
    printf("\n");
    for(int vegades = 0; vegades < N_VEGADES; vegades++){
        start = clock();
        while(!esTriangularforcaBruta(n))
            n++;
        end = clock();
        printf("\tForsa bruta:\n\t\tTemps emprat = %lf\n\t\tNumero triangular = %d\n", ((double)(end - start)) / CLOCKS_PER_SEC, n);
        timeFB += (double)(end - start) / CLOCKS_PER_SEC;
        n = valn;

        start = clock();
        while(!esTriangularOptima(n))
            n++;
        end = clock();
        printf("\tEstrategia optima:\n\t\tTemps emprat = %lf\n\t\tNumero triangular = %d\n", ((double)(end - start)) / CLOCKS_PER_SEC, n);
        timeOp += (double)(end - start) / CLOCKS_PER_SEC;
        n = valn;

        start = clock();
        while(!esTriangularTaula(n, taula, max) && n < P)
            n++;
        end = clock();
        printf("\tEstrategia taula:\n\t\tTemps emprat = %lf\n\t\tNumero triangular = %d\n", ((double)(end - start)) / CLOCKS_PER_SEC, n);
        timeTa += (double)(end - start) / CLOCKS_PER_SEC;
        n = valn;
    }
    timeFB /= N_VEGADES;
    timeOp /= N_VEGADES;
    timeTa /= N_VEGADES;
    printf("\nProva acabada...\n\tTemps mitja forsa bruta: %.3lf\n\tTemps mitja estrategia optima: %.3lf\n\tTemps mitja taula: %.3lf", timeFB, timeOp, timeTa);
    printf("\n");
    return 0;
}
