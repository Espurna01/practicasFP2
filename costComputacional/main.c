#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <windows.h>
#include <math.h>
#define P 50000
#define N_VEGADES 1000

bool esTriangularforcaBruta(unsigned int x){
    unsigned int nTriangular = 1;
    for(unsigned int i = 2; nTriangular < x; i++){
        nTriangular += i;
    }
    return nTriangular == x;
}

bool esTriangularOptima(unsigned int x){
//    if(x > 536870911)
//        return esTriangularforcaBruta(x);
    unsigned long long arrel = x;
    arrel = arrel*8 + 1;
    unsigned long long n =(unsigned long long) sqrt(arrel);
    return n*n == arrel;
}

bool esTriangularOptima2(unsigned int x){

    return true;
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
    unsigned int i = 2;
    taula[0] = 1;
    while(i <= max){
        taula[i - 1] = taula[i - 2] + i;
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
    float oldTimeTa = timeTa;
    printf("La taula s'ha emplenat en %.3fs. Y el valor maxim de la taula es %u, %d\n", timeTa, taula[P - 1], P);

    unsigned int n, valn = rand() + 1;

    printf("Calculant el temps d'execucio 5 vegades:\n\n");
    for(int vegades = 0; vegades < 5; vegades++){
        n = valn;
        printf("Calulant numero triangular >= %u:\n\n", n);
        gettimeofday(&start, NULL);
        while(!esTriangularforcaBruta(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia forsa bruta:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeFB += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!esTriangularOptima(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia optima:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeOp += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!esTriangularTaula(n, taula, P))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia taula:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        valn = valn * 2;
        printf("\n");
    }
    timeFB /= 5;
    timeOp /= 5;
    timeTa /= 5;
    printf("Prova acabada...\n\tTemps mitja forsa bruta: %fs\n\tTemps mitja estrategia optima: %fs\n\tTemps mitja taula: %fs", timeFB, timeOp, timeTa);
    printf("\n\n");
    char a;
    do{
        printf("Prem ENTER per fer la prova %d vegades...\n", N_VEGADES);
        scanf("%c", &a);
    }while(a != '\n');
    printf("\nCalculant el temps d'execucio %d vegades:\n", N_VEGADES);

    timeTa = oldTimeTa;
    timeFB = 0;
    timeOp = 0;

    for(int vegades = 0; vegades < N_VEGADES; vegades++){
        n = rand();
        unsigned int valn = n;
        gettimeofday(&start, NULL);
        while(!esTriangularforcaBruta(n))
            n++;
        gettimeofday(&end, NULL);

        timeFB += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!esTriangularOptima(n))
            n++;
        gettimeofday(&end, NULL);

        timeOp += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!esTriangularTaula(n, taula, P))
            n++;
        gettimeofday(&end, NULL);

        timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;
        if((vegades + 1) % 5 == 0)
            printf("\n\t%.1f%%", ((vegades + 1) * 100.)/N_VEGADES);
    }
    timeFB /= N_VEGADES;
    timeOp /= N_VEGADES;
    timeTa /= N_VEGADES;
    printf("\nProva acabada...\n\tTemps mitja forsa bruta: %fs\n\tTemps mitja estrategia optima: %fs\n\tTemps mitja taula: %fs", timeFB, timeOp, timeTa);
    printf("\n");
    return 0;
}
