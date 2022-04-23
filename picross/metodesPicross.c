#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAXROWCOL 50
#define CENTINELLA -1

typedef struct{
    char valor;
    bool revelat;
    bool flag;
}casella_t;

bool carregarDades(char* filename, int *m, int *n, int *e, casella_t joc[][MAXROWCOL]){
    FILE *fit = fopen(filename, "r");
    char buffer;
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    fscanf(fit, "%d %d %d", m, n, e);
    buffer = 'a';
    while(buffer != '\n')
        fscanf(fit, "%c", &buffer);
    for(int i = 0; i < *m; i++){
        for(int j = 0; j < *n; j++){
            fscanf(fit, "%c", &(joc[i][j].valor));
            joc[i][j].revelat = false;
            joc[i][j].flag = false;
        }
        fscanf(fit, "%c", &buffer);
    }
    fclose(fit);
    return true;
}

void calcularCantonades(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]){
    char cantonadaActual = 0;
    int index = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(joc[i][j].valor == '1')
                cantonadaActual++;
            else {
                if(cantonadaActual != 0){
                    cantonades[i][index] = cantonadaActual;
                    index++;
                }
                cantonades[i][index] = CENTINELLA;
                cantonadaActual = 0;
            }
        }
        if(cantonadaActual != 0){
            cantonades[i][index] = cantonadaActual;
            index++;
            cantonades[i][index] = CENTINELLA;
            cantonadaActual = 0;
        }
        index = 0;
    }
    cantonadaActual = 0;
    for(int j = 0; j < n; j++){
        for(int i = 0; i < m; i++){
            if(joc[i][j].valor == '1')
                cantonadaActual++;
            else {
                if(cantonadaActual != 0){
                    cantonades[j + m][index] = cantonadaActual;
                    index++;
                }
                cantonades[j + m][index] = CENTINELLA;
                cantonadaActual = 0;
            }
        }
        if(cantonadaActual != 0){
            cantonades[j + m][index] = cantonadaActual;
            index++;
            cantonades[j + m][index] = CENTINELLA;
            cantonadaActual = 0;
        }
        index = 0;
    }
}

bool generarFitxerAleatori(char *filename, int m, int n, int e, int density){
    FILE *fit = fopen(filename, "w");

    if(fit == NULL){
        fclose(fit);
        return false;
    }

    if(m > MAXROWCOL)
        m = MAXROWCOL;
    if(n > MAXROWCOL)
        n = MAXROWCOL;

    fprintf(fit, "%d %d %d\n", m, n, e);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            char a = (rand() % 100 + 1) <= density;
            fprintf(fit, "%d", a);
        }
        fprintf(fit, "\n");
    }
    fclose(fit);

    return true;
}

bool jocAcabat(int m, int n, casella_t joc[][MAXROWCOL]){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(!joc[i][j].revelat && joc[i][j].valor == '1')
                return false;
        }
    }
    return true;
}

bool boardToPBM(char *filename, int m, int n, casella_t joc[][MAXROWCOL]){
    FILE *fit = fopen(filename, "wb");
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    fprintf(fit, "P1 %d %d\n", n, m);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            fputc(joc[i][j].valor, fit);
        }
    }
    fclose(fit);
    return true;
}

void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]){
    int maxBlocsM = 0;
    int maxDigitsM = 0;
    int maxBlocsN = 0;
    int maxDigitsN = 0;
    for(int i = 0; i < m + n; i++){
        int digits = 0;
        int blocs = 0;
        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            char cantonada = cantonades[i][j];
            while(cantonada != 0){
                digits++;
                cantonada /= 10;
            }
            blocs++;
            if(i >= m){
                if(digits > maxDigitsN)
                    maxDigitsN = digits;
                digits = 0;
            }
        }
        if(i < m){
            if(digits > maxDigitsM)
                maxDigitsM = digits;
            if(blocs > maxBlocsM)
                maxBlocsM = blocs;
        }else {
            if(blocs > maxBlocsN)
                maxBlocsN = blocs;
        }
    }

    printf("\t\t%c", 201);
    for(int i = 0; i < maxDigitsM * 2 + (maxDigitsN - 1) * n + n * 2;i++)
        printf("%c", 205);
    printf("%c\n", 187);

    for(int i = 0; i < maxBlocsN; i++){
        printf("\t\t%c", 186);
        for(int j = 0; j < maxDigitsM * 2; j++){
            printf(" ");
        }
        for(int j = 0; j < n; j++){
            printf("|");

            int blocs;
            for(blocs = 0; cantonades[j + m][blocs] != CENTINELLA; blocs++);

            if(i + blocs < maxBlocsN){
                for(int k = 0; k < maxDigitsN; k++){
                    printf(" ");
                }
            }else{
                printf("%d", cantonades[j + m][i - maxBlocsN + blocs]);
                int digits = 0;
                char cantonada = cantonades[j + m][i - maxBlocsN + blocs];
                while(cantonada != 0){
                    digits++;
                    cantonada /= 10;
                }
                for(int k = 0;digits + k < maxDigitsN;k++){
                    printf(" ");
                }
            }
        }
        printf("%c\n", 186);
    }

    for(int i = 0; i < m; i++){
        printf("\t\t%c", 186);
        for(int j = 0;j < maxDigitsM * 2 + (maxDigitsN - 1) * n + n * 2; j++){
            printf("%c", 196);
        }
        printf("%c\n", 186);
        printf("\t\t%c", 186);
        int digits = 0;
        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            char cantonada = cantonades[i][j];
            while(cantonada != 0){
                digits++;
                cantonada /= 10;
            }
        }

        if(digits == 0)
            digits = 1;

        for(int k = 0; (digits * 2) + k < maxDigitsM * 2 + 1;k++)
            printf(" ");

        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            char a = cantonades[i][j] / 10;
            while(a != 0){
                printf(" ");
                a /= 10;
            }
        }

        if(cantonades[i][0] == CENTINELLA)
            printf(" ");

        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            if(j == 0)
                printf("%d", cantonades[i][j]);
            else printf(" %d", cantonades[i][j]);
        }

        for(int j = 0; j < n; j++){
            printf("|");
            if(joc[i][j].revelat)
                printf("%c", joc[i][j].valor == '1'?254:'X');
            else printf("%c", joc[i][j].flag?'F':' ');
            for(int k = 1; k < maxDigitsN; k++){
                printf(" ");
            }
        }
        printf("%c\n", 186);
    }

    printf("\t\t%c", 200);
    for(int i = 0; i < maxDigitsM * 2 + (maxDigitsN - 1) * n + n * 2;i++)
        printf("%c", 205);
    printf("%c\n", 188);
    printf("\n");
}

bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j, bool flag)
{

    bool posicio_correcta;
    posicio_correcta=i<m && j<n && i>=0 && j>=0;
    if(joc[i][j].revelat)
    {
        if(joc[i][j].valor == '1'?254:'X')
        {
            posicio_correcta=false;
        }
    }
    else(joc[i][j].flag)
    {
        if(joc[i][j].flag?'F':' ')
        {
            posicio_correcta=false;
        }
    }
    return posicio_correcta;
}
