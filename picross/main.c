#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAX 10

typedef struct{
    char valor;
    bool revelat;
}casella_t;

bool carregarMida(char *filename, int *m, int *n, int *maxErrors){
    FILE *fit = fopen(filename, "r");
    if(fit == NULL){
        return false;
        fclose(fit);
    }
    fscanf(fit, "%d %d %d", m, n, maxErrors);
    fclose(fit);
    return true;
}

bool carregarDades(char* filename, int m, int n, casella_t joc[][n]){
    FILE *fit = fopen(filename, "r");
    char buffer;
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    buffer = 'a';
    while(buffer != '\n')
        fscanf(fit, "%c", &buffer);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            fscanf(fit, "%c", &(joc[i][j].valor));
            joc[i][j].revelat = false;
        }
        fscanf(fit, "%c", &buffer);
    }
    fclose(fit);
    return true;
}

void calcularCantonades(int m, int n, casella_t joc[][n], int cantonades[]){
    int cantonadaActual = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(joc[i][j].valor == '1')
                cantonadaActual++;
            else if (cantonadaActual % 10 != 0)
                cantonadaActual *= 10;
        }
        if(cantonadaActual % 10 == 0)
            cantonadaActual /= 10;
        cantonades[i] = cantonadaActual;
        cantonadaActual = 0;
    }

    for(int j = 0; j < n; j++){
        for(int i = 0; i < m; i++){
            if(joc[i][j].valor == '1')
                cantonadaActual++;
            else if (cantonadaActual % 10 != 0)
                cantonadaActual *= 10;
        }
        if(cantonadaActual % 10 == 0)
            cantonadaActual /= 10;
        cantonades[j + m] = cantonadaActual;
        cantonadaActual = 0;
    }
}

void printTaulerJoc(int m, int n, casella_t joc[][n], int cantonades[]){
    int digitsMaxCantonadesM = 0;
    int digitsMaxCantonadesN = 0;
    for(int i = 0; i < m + n; i++){
        int digits = 0;
        int cantonada = cantonades[i];
        while(cantonada != 0){
            digits++;
            cantonada /= 10;
        }
        if(i < m){
            if(digits > digitsMaxCantonadesM)
                digitsMaxCantonadesM = digits;
        }else if(digits > digitsMaxCantonadesN)
            digitsMaxCantonadesN = digits;

    }
    for(int i = 0; i < digitsMaxCantonadesN; i++){
        for(int j = 0; j < digitsMaxCantonadesM; j++){
            printf(" ");
        }
        for(int j = 0; j < n; j++){
            printf("|");
            int valor = cantonades[m + j];

            for(int k = 0; k < i; k++){
                valor = valor/10;
            }
            if(valor % 10 != 0)
                printf("%d", valor % 10);
            else printf(" ");
        }
        printf("\n");
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < (digitsMaxCantonadesM + n * 2); j++){
            printf("%c", 196);
        }
        printf("\n");
        int valor = cantonades[i];
        int digits = 0;
        while(valor != 0){
            valor /= 10;
            digits++;
        }
        printf("%d", cantonades[i]);
        for(int j = 0; digits + j < digitsMaxCantonadesM;j++){
            printf(" ");
        }
        for(int j = 0; j < n; j++){
            printf("|");
            if(!joc[i][j].revelat)
                printf("%c", joc[i][j].valor == '1'?254:'X');
            else printf("_");
        }
        printf("\n");


    }
}

bool generarFitxerAleatori(char *filename, int m, int n, int e){
    FILE *fit = fopen(filename, "w");
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    fprintf(fit, "%d %d %d\n", m, n, e);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            int rand10 = rand() % 50 + 1;
            char a = rand() % rand10 > rand10/2;
            fprintf(fit, "%d", a);
        }
        fprintf(fit, "\n");
    }
    fclose(fit);
    return true;
}

int main()
{
    srand(time(NULL));
    int m, n, errors;
    carregarMida("fitProva.txt", &m, &n, &errors);
    casella_t joc[m][n];
    int cantonades[m + n];
    carregarDades("fitProva.txt", m, n, joc);
    calcularCantonades(m, n, joc, cantonades);


    printf("%d %d %d\n", m, n, errors);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf("%c", joc[i][j].valor);
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; i < m + n; i++)
        printf("%d\n", cantonades[i]);

    printTaulerJoc(m, n, joc, cantonades);
    //printf("%d\n", rand() % 10 > 5);
    //generarFitxerAleatori("fitProva.txt", 10, 10, 10);
    return 0;
}
