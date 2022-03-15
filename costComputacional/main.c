#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <windows.h>
#include <math.h>
#define P 50000
#define N_VEGADES 10

bool esTriangularforcaBruta(unsigned int x){
    unsigned int nTriangular = 1;
    for(unsigned int i = 2; nTriangular < x; i++){
        nTriangular += i;
    }
    return nTriangular == x;
}

bool esTriangularOptima(unsigned int x){
    unsigned int arrel = 8*x + 1;
    unsigned int n =(unsigned int) sqrt(arrel);
    return n*n == arrel;
}

bool esTriangularTaula(unsigned int x, unsigned int taula[], int max){
    if(x > taula[max - 1]) return false;
    for(int i = 0;i < max;i++){
        if(x == taula[i])
            return true;
    }
    return false;
}


void iniTaula(unsigned int taula[], int max){
    unsigned int i = 1;
    int index = 0;
    while(index < max){
        if(esTriangularOptima(i)){
            taula[index] = i;
            index++;
            //printf("%d\n", index);
        }
        i++;
    }
}

int main()
{
    struct timeval start, end;
    srand(time(NULL));
    printf("Se esta emplenant la taula amb %d elements...\n", P);

    gettimeofday(&start, NULL);
    unsigned int taula[P];
    iniTaula(taula, P);
    gettimeofday(&end, NULL);

    float timeFB = 0, timeOp = 0, timeTa = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
    printf("La taula s'ha emplenat en %.3fs.\n", timeTa);

    unsigned int n;

    printf("Calculant el temps d'execucio %d vegades:\n\n", N_VEGADES);
    for(int vegades = 0; vegades < N_VEGADES; vegades++){
        n = (rand() * rand() + 1) / 8;
        printf("Calulant numero triangular >= %u\n\n", n);
        unsigned int valn = n;
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


        gettimeofday(&start, NULL);
        while(!esTriangularTaula(n, taula, P))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia taula:\n\t\tTemps emprat = %f\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        printf("\n");
    }
    timeFB /= N_VEGADES;
    timeOp /= N_VEGADES;
    timeTa /= N_VEGADES;
    printf("\nProva acabada...\n\tTemps mitja forsa bruta: %f\n\tTemps mitja estrategia optima: %f\n\tTemps mitja taula: %f", timeFB, timeOp, timeTa);
    printf("\n");
    return 0;
}
