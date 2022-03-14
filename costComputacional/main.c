#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <windows.h>
#define P 50000
#define N_VEGADES 5


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
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int max = calculaMax();
    unsigned int taula[max];
    iniTaula(taula, max);
    gettimeofday(&end, NULL);
    printf("La taula s'ha emplenat en %.3fs.\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0);

    unsigned int n;
    float timeFB = 0, timeOp = 0, timeTa = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
    do{
        printf("Esciru un nombre > 0: ");
        scanf(" %u", &n);
    }while(n <= 0);
    unsigned int valn = n;
    printf("\n");
    for(int vegades = 0; vegades < N_VEGADES; vegades++){

        gettimeofday(&start, NULL);
        while(!esTriangularforcaBruta(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia forsa bruta:\n\t\tTemps emprat = %f\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeFB += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!esTriangularOptima(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia optima:\n\t\tTemps emprat = %f\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeOp += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        if(n > taula[max - 1]){
            printf("\tL'estrategia de la taula no pot calcular un numero triangular més gran que %d.\n", taula[max - 1]);
        }else{
            gettimeofday(&start, NULL);
            while(!esTriangularTaula(n, taula, max))
                n++;
            printf("\tEstrategia taula:\n\t\tTemps emprat = %f\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
            gettimeofday(&end, NULL);
            gettimeofday(&end, NULL);
            gettimeofday(&end, NULL);

            timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
            n = valn;
        }

    }
    timeFB /= N_VEGADES;
    timeOp /= N_VEGADES;
    timeTa /= N_VEGADES;
    printf("\nProva acabada...\n\tTemps mitja forsa bruta: %f\n\tTemps mitja estrategia optima: %f\n\tTemps mitja taula: %f", timeFB, timeOp, timeTa);
    printf("\n");
    return 0;
}
