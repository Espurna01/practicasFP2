#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAXROWCOL 30
#define CENTINELLA -1
#define CADMAX 50

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
        fputc('\n', fit);
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
        printf("%c", 186);
        if(i == maxBlocsN - 1)
            printf("y");
        printf("\n");
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
        printf("%c%d\n", 186, i + 1);

    }

    printf("\t\t%c", 200);
    for(int i = 0; i < maxDigitsM * 2 + (maxDigitsN - 1) * n + n * 2;i++)
        printf("%c", 205);
    printf("%c\n\t\t", 188);

    for(int j = 0; j < maxDigitsM * 2; j++){
        printf(" ");
    }
    printf("x ->");

    printf("\n\n");
}

bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j, bool flag)
{
    bool posicio_correcta;
    posicio_correcta = i < m && j < n && i >= 0 && j >= 0 && !joc[i][j].revelat;
    if(posicio_correcta){
        joc[i][j].revelat = !flag;
        joc[i][j].flag = flag;
    }
    return posicio_correcta;
}

void printMenu(){
    printf("\t\t1. Carregar tauler.\n");
    printf("\t\t2. Jugar joc carregat.\n");
    printf("\t\t3. Jugar en un tauler aleatori.\n");
    printf("\t\t4. Estrategies.\n");
    printf("\t\t5. Generar fitxer aleatori.\n");
    printf("\t\t6. Tauler a imatge pbm.\n");
    printf("\t\t0. Sortir.\n");
}

bool afegirExtensio(char* fitxer, char *extensio){

    int saltLinea = 0;
    while(fitxer[saltLinea] != '\n' && saltLinea < CADMAX){
        saltLinea++;
    }

    if(saltLinea + 5 >= CADMAX){ /**< {'.', 't', 'x', 't', '\0'}/{'.', 'p', 'b', 'm', '\0'} */
        return false;
    }
    int i;
    for(i = 0; extensio[i] != '\0'; i++){
        fitxer[i + saltLinea] = extensio[i];
    }
    fitxer[i + saltLinea] = '\0';

    return true;
}

bool combinarPath(char *fitxer, char *path, char *pathFinal){
    int indexfinal;
    for(indexfinal = 0; path[indexfinal] != '\0'; indexfinal++){
        pathFinal[indexfinal] = path[indexfinal];
    }

    int indexfit = 0;
    while(fitxer[indexfit] != '\0' && indexfit + indexfinal < CADMAX){
        pathFinal[indexfinal + indexfit] = fitxer[indexfit];
        indexfit++;
    }
    if(indexfit + indexfinal < CADMAX){
        pathFinal[indexfinal + indexfit] = '\0';
        return true;
    }
    pathFinal[0] = '\0';
    return false;
}

void restaurarJoc(int m, int n, casella_t joc[][MAXROWCOL]){
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            joc[i][j].flag = false;
            joc[i][j].revelat = false;
        }
    }
}

bool jugar(int m, int n, casella_t joc[][MAXROWCOL], int *errorsActuals, int maxErrors){
    int maxCombinations = m > n?m:n;
    maxCombinations = maxCombinations + maxCombinations % 2;
    maxCombinations /= 2;
    maxCombinations++;
    char cantonades[n + m][maxCombinations];
    calcularCantonades(m, n, joc, maxCombinations, cantonades);

    int i = -1, j = -1;
    while(*errorsActuals < maxErrors && !jocAcabat(m, n, joc)){
        system("cls");
        printf("\n\n\tUltima posicio triada (");

        if(i == -1){
            printf(", )");
        }else printf("%d, %d)", i, j);
        printf(". Errors %d/%d. Tauler %dx%d:\n\n", *errorsActuals, maxErrors, n, m);
        printTaulerJoc(m, n, joc, maxCombinations, cantonades);

        int flag;
        do{
            printf("\tSeleccionar caselles (0), posar banderes (1), sortir (2): ");
            scanf(" %d", &flag);
        }while(flag != 1 && flag != 0 && flag != 2);

        if(flag == 2)
            break;

        do{
            printf("\tEscull una columna (x = [1, %d] o tota la taula (%d)) o (0) per sortir: ", n, n + 1);
            scanf(" %d", &i);
        }while(i < 1 || i > n + 1);

        if(i == 0)
            break;

        if(i != n + 1){
            do{
                system("cls");

                printf("\n\n\tUltima posicio triada (");

                if(i == -1 || j == -1){
                    printf(", )");
                }else printf("%d, %d)", i, j);
                printf(". Errors %d/%d. Tauler %dx%d:\n\n", *errorsActuals, maxErrors, n, m);
                printTaulerJoc(m, n, joc, maxCombinations, cantonades);

                printf("\n\tEscull totes les files de la columna %d que vulguis, escriu 0 per sortir:", i);

                printf("\n\t[1, %d] o tota la columna (%d): ", m, m + 1);
                scanf(" %d", &j);
                if(seleccio(m, n, joc, j - 1, i - 1, flag)){
                    if(!flag && joc[j - 1][i - 1].valor == '0'){
                        (*errorsActuals)++;
                    }
                }
            }while(j != 0 && *errorsActuals < maxErrors && j != m + 1);

            if(j == m + 1){
                j = 0;
                while(*errorsActuals < maxErrors && j < m){
                    if(!joc[j][i - 1].flag && seleccio(m, n, joc, j, i - 1, flag)){
                        if(joc[j][i - 1].valor == '0'){
                            (*errorsActuals)++;
                        }
                    }
                    j++;
                }
            }

        }else {
            i = 0;
            while(*errorsActuals < maxErrors && i < m){
                j = 0;
                while(*errorsActuals < maxErrors && j < n){
                    if(!joc[i][j].flag && seleccio(m, n, joc, i, j, flag)){
                        if(joc[i][j].valor == '0'){
                            (*errorsActuals)++;
                        }
                    }
                    j++;
                }
                i++;
            }
        }

    }
    system("cls");

    if(*errorsActuals >= maxErrors){
        printf("\n\n\tHas arribat al limit de errors permesos, hauras de comen%car de nou.\n\n", 135);
    }else if (jocAcabat(m, n, joc)){
        printf("\n\n\tFelicitats has guanyat! Cometent %d errors.\n\n", *errorsActuals);
    } else {
        printf("\n\n\tLa partida quedara guardada. Quan vulguis continuar torna a seleccionar aquesta opcio.");
        return false;
    }

    printTaulerJoc(m, n, joc, maxCombinations, cantonades);
    *errorsActuals = 0;
    restaurarJoc(m, n, joc);
    return true;

}

void taulerAleatori(int m, int n, casella_t joc[][MAXROWCOL]){
    int density = (rand() % 4 + 1) * 20; /**< {20, 40, 60, 80} */
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            char a = (rand() % 100 + 1) <= density;
            a = a + 48; /**< ascii('0') = 48 */
            joc[i][j].valor = a;
            joc[i][j].revelat = false;
            joc[i][j].flag = false;
        }
    }
}

void clearBuffer(){
    char buffer = 0;
    while(buffer != '\n'){
        scanf("%c", &buffer);
    }
}
