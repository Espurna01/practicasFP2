#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
    fseek(fit, 6, SEEK_SET);
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

void calcularCantonades(int m, int n, casella_t joc[][n], int cantonada[]){
    int cantonadaActual = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            if(joc[i][j].valor == '0' && cantonadaActual % 10 != 0)
                cantonadaActual *= 10;
            else if (joc[i][j].valor == '1')
                cantonadaActual++;
        }
        if(cantonadaActual % 10 == 0)
            cantonadaActual /= 10;
        cantonada[i] = cantonadaActual;
        cantonadaActual = 0;
    }

    for(int j = 0; j < n; j++){
        for(int i = 0; i < m; i++){
            if(joc[i][j].valor == '0' && cantonadaActual % 10 != 0)
                cantonadaActual *= 10;
            else if (joc[i][j].valor == '1')
                cantonadaActual++;
        }
        if(cantonadaActual % 10 == 0)
            cantonadaActual /= 10;
        cantonada[j + m] = cantonadaActual;
        cantonadaActual = 0;
    }
}

int main()
{
    int m, n, errors;
    carregarMida("joc.txt", &m, &n, &errors);
    casella_t joc[m][n];
    int cantonades[m + n];
    carregarDades("joc.txt", m, n, joc);

    printf("%d %d %d\n", m, n, errors);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf("%c", joc[i][j].valor);
        }
        printf("\n");
    }
    printf("\n");
    calcularCantonades(m, n, joc, cantonades);
    for(int i = 0; i < m + n; i++)
        printf("%d\n", cantonades[i]);
    return 0;
}
