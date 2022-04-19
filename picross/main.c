#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAX 10
#define MAXROWCOL 100
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
            int rand10 = rand() % 100 + 1;
            char a = rand() % rand10 <= density;
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

        for(int k = 0; (digits * 2) + k < maxDigitsM * 2 + 1;k++)
            printf(" ");

        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            char a = cantonades[i][j] / 10;
            while(a != 0){
                printf(" ");
                a /= 10;
            }
        }

        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            if(j == 0)
                printf("%d", cantonades[i][j]);
            else printf(" %d", cantonades[i][j]);
        }

        for(int j = 0; j < n; j++){
            printf("|");
            if(joc[i][j].revelat)
                printf("%c", joc[i][j].valor == '1'?254:'X');
            else printf("%c", joc[i][j].flag?'F':'?');
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

int cantonadaOptima(int start, int end, int maxCombinations,char cantonades[][maxCombinations]){
    int optim = 0;
    int prevSum = 0;
    int current;

    return optim;
}

bool overlapTech(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]){
    bool change = true;
    for(int i = 0; i < m + n; i++){
        int blocs;
        int sum = 0;
        for(blocs = 0; cantonades[i][blocs] != CENTINELLA; blocs++){
            sum += cantonades[i][blocs];
        }

        int fc = i < m?n:m;
        if(sum + blocs/2 > fc/2){
            char arr1[fc];
            char arr2[fc];
            if(fc == n){
                for(int j = 0; j < n; j++){
                    if(joc[i][j].revelat){
                        arr1[j] = joc[i][j].valor;
                        arr1[j] = joc[i][j].valor;
                    }else if(joc[i][j].flag){
                        arr1[j] = '0';
                        arr2[j] = '0';
                    }
                    else{
                        arr1[j] = 0;
                        arr2[j] = 0;
                    }

                }
            }else {
                for(int j = 0; j < m; j++){
                    if(joc[j][i].revelat){
                        arr1[j] = joc[i][j].valor;
                        arr1[j] = joc[i][j].valor;
                    }else if(joc[j][i].flag){
                        arr1[j] = '0';
                        arr2[j] = '0';
                    }
                    else{
                        arr1[j] = 0;
                        arr2[j] = 0;
                    }
                }
            }
            int i = 0;
            int j;
            for(j = 0; cantonades[i][j] != CENTINELLA; j++){
                for(;i <= cantonades[i][j]; i++){
                    if(arr1[i] == 0)
                        arr1[i] = '1';
                }
                if(i < fc)
                    arr1[i] = 0;
            }
            for(j = j - 1; j >= 0; j--){
                if(joc[i])
            }

            printf("[");
            for(int j = 0; j < fc; j++){
                if(j == 0)
                    printf("%d", arr1[j]);
                else printf(", %d", arr1[j]);
            }
            printf("]\n");

            printf("[");
            for(int j = 0; j < fc; j++){
                if(j == 0)
                    printf("%d", arr2[j]);
                else printf(", %d", arr2[j]);
            }
            printf("]\n");
            break;
        }else if(sum == 0){

        }


    }

    return change;
}

void IA(int m, int n, int e, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]){
    int errorsMaquina = 0;
    char cantonadesMaquina[m + n][maxCombinations];

    for(int i = 0; i < m + n; i++){
        int j;
        for(j = 0; cantonades[i][j] != CENTINELLA; j++){
            cantonadesMaquina[i][j] = cantonades[i][j];
        }
        cantonadesMaquina[i][j] = CENTINELLA;
    }

    while(!jocAcabat(m, n, joc) && errorsMaquina <= e){
        while(!(overlapTech(m, n, joc, maxCombinations, cantonades))){
            printTaulerJoc(m, n, joc, maxCombinations, cantonades);
        }

        break;
    }

    if(errorsMaquina > e){
        printf("La maquina ha guanyat ");
    } else printf("La maquina ha perdut en ");
    printf("%d errors.", errorsMaquina);
}

int main()
{
    srand(time(NULL));
    int m, n, maxErrors;
    casella_t joc[MAXROWCOL][MAXROWCOL];
    carregarDades("fitProva.txt", &m, &n, &maxErrors, joc);

    int maxCombinations = 0;
    if(m > n){
        if(m % 2 != 0)
            maxCombinations++;
        maxCombinations += m;
    }else {
        if(n % 2 != 0)
            maxCombinations++;
        maxCombinations += n;
    }
    maxCombinations /= 2;
    maxCombinations++;
    char cantonades[n + m][maxCombinations];
    calcularCantonades(m, n, joc, maxCombinations, cantonades);

    printf("%d %d %d\n", m, n, maxErrors);

    printTaulerJoc(m, n, joc, maxCombinations, cantonades);
    boardToPBM("fitProva.pbm", m, n, joc);

    printf("Joc acabat %d\n", jocAcabat(m, n, joc));

    IA(m, n  , maxErrors, joc, maxCombinations, cantonades);
    //generarFitxerAleatori("fitProva.txt", 5, 5, 10, 25);
    return 0;
}
