#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "triangular.h"
#define P 50000
#define N_VEGADES 10000

int main()
{
    struct timeval start, end;
    srand(time(NULL));
    printf("Se esta emplenant la taula amb %d elements...\n", P);

    gettimeofday(&start, NULL);
    unsigned int taula[P];
    calcula_triangulars(taula, P);
    gettimeofday(&end, NULL);

    float timeFB = 0, timeOp = 0, timeTa = 0;
    float kTa = end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
    printf("La taula s'ha emplenat en %lfs. Y el valor maxim de la taula es %u, %d\n", kTa, taula[P - 1], P);

    unsigned int n;

    printf("Calculant el temps d'execucio 10 vegades:\n\n");
    for(int vegades = 0; vegades < 10; vegades++){
        n = rand() * (rand() % 50 + 1) + 1;
        unsigned int valn = n;
        printf("Calulant numero triangular >= %u:\n\n", n);
        gettimeofday(&start, NULL);
        while(!es_triangular_fb(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia forsa bruta:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeFB += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!es_triangular_op(n))
            n++;
        gettimeofday(&end, NULL);

        printf("\tEstrategia optima:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
        timeOp += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        if(n <= taula[P - 1]){
            gettimeofday(&start, NULL);
            while(!es_triangular_tau(n, taula, P))
            n++;
            gettimeofday(&end, NULL);

            printf("\tEstrategia taula:\n\t\tTemps emprat = %fs\n\t\tNumero triangular = %d\n", end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0, n);
            timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
            n = valn;
        } else{
            printf("\tNo es pot calcular per l'estrategia de la taula el numero %d. Ja que es mes gran que la ultima posició guardada %d.\n", n, taula[P - 1]);
        }

        printf("\n");
    }
    timeFB /= 5;
    timeOp /= 5;
    timeTa /= 5;
    printf("Prova acabada...\n\tTemps mitja forsa bruta: %fs\n\tTemps mitja estrategia optima: %fs\n\tTemps mitja taula: %fs, (+ k) %fs", timeFB, timeOp, timeTa, timeTa + kTa);
    printf("\n\n");
    char a;
    do{
        printf("Prem ENTER per fer la prova %d vegades...\n", N_VEGADES);
        scanf("%c", &a);
    }while(a != '\n');
    printf("\nCalculant el temps d'execucio %d vegades:\n", N_VEGADES);

    timeTa = 0;
    timeFB = 0;
    timeOp = 0;

    for(int vegades = 0; vegades < N_VEGADES; vegades++){
        n = rand() * (rand() % 50 + 1) + 1;
        unsigned int valn = n;
        gettimeofday(&start, NULL);
        while(!es_triangular_fb(n))
            n++;
        gettimeofday(&end, NULL);

        timeFB += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;

        gettimeofday(&start, NULL);
        while(!es_triangular_op(n))
            n++;
        gettimeofday(&end, NULL);

        timeOp += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
        n = valn;
        if(n <= taula[P - 1]){
            gettimeofday(&start, NULL);
            while(!es_triangular_tau(n, taula, P))
                n++;
            gettimeofday(&end, NULL);

            timeTa += end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec)/1000000.0;
            n = valn;
        }

        if((vegades + 1) % (N_VEGADES / 10) == 0)
            printf("\n\t%d%%", ((vegades + 1) * 100)/N_VEGADES);
    }
    timeFB /= N_VEGADES;
    timeOp /= N_VEGADES;
    timeTa /= N_VEGADES;
    printf("\n\nProva %d vegades acabada...\n\tTemps mitja forsa bruta: %fs\n\tTemps mitja estrategia optima: %fs\n\tTemps mitja taula: %fs, (+ k) %fs", N_VEGADES, timeFB, timeOp, timeTa, timeTa + kTa);
    printf("\n");
    return 0;
}
