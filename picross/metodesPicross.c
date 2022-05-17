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
    /**< Obrim el fitxer llegint el contingut d'aquest, per obtenir les dades necessaries i finalment tancar el fitxer. */
    FILE *fit = fopen(filename, "r");
    char buffer;
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    /**< Obtenim les files (m), columnes (n) i errors(e) permesos pel fitxer. */
    fscanf(fit, "%d %d %d", m, n, e);
    buffer = 'a';
    /**< Llegir la taula fins al final (trobar '\n') per carregar les dades de la partida guardada al fitxer. */
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
    /**< Revisar que es troba les cantonades correctes a files i columnes esquerres de la taula */
    /**< En cas de trobar un nombre diferent a 1 o 0, la possició actual es consideraria CENTINELLA */
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
    /**< Revisar que es troba les cantonades correctes a files i columnes de la dreta de la taula. */
    /**< En cas de trobar un nombre diferent a 1 o 0, la possició actual es consideraria CENTINELLA. */
    cantonadaActual = 0;
    for(int j = 0; j < n; j++){
        for(int i = 0; i < m; i++){
            if(joc[i][j].valor == '1')
                cantonadaActual++;
            else {
                if(cantonadaActual != 0){
                    cantonades[j + m][index] = cantonadaActual; /**< Cal sumar +m perque ens situem a la part dreta del tauler. */
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
<<<<<<< Updated upstream
    printf("%c\n", 188);
    printf("\n");
=======
    printf("%c\n\t\t", 188);

    for(int j = 0; j < maxDigitsM * 2; j++){
        printf(" ");
    }

    if(marca){
        for(int i = 0; i < marca; i++)
            printf("  ");
        printf("%c", 30);
    }else printf("x ->");

    printf("\n\n");
>>>>>>> Stashed changes
}

<<<<<<< Updated upstream
/** \brief Seleccionar una posició en el tauler de joc. Revela la poscició i, j.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param i: posició que ha triat l'usuari (files), enter.
 * \param j: posició que ha triat l'usuaari (columnes), enter.
 * \return true si la posició es correcte (dins de la taula) i fals en cas contrari.
 * (Pau Reverté)
 */

bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j);
=======
bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j, bool flag)
{

<<<<<<< Updated upstream
    bool posicio_correcta;
    posicio_correcta=i<m && j<n && i>=0 && j>=0;
    if(joc[i][j].revelat)
    {
        if(joc[i][j].valor == '1'?254:'X')
        {
            posicio_correcta=false;
=======
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
    /**< Passem per totes les files i columnes revisant en cas de que hi tingue una flag o estigui revelat. */
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            joc[i][j].flag = false; /**< Posar flag en false per a reiniciar i que la casella estigui neta. */
            joc[i][j].revelat = false; /**< Posar revelat en false per a reiniciar i que la casella no es trobi rebelada. */
        }
    }
}

void clearBuffer(){
    char buffer = 0;
    while(buffer != '\n'){
        scanf("%c", &buffer);
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
        char f;
        if(i == -1){
            printf(", )");
        }else printf("%d, %d)", i, j);
        printf(". Errors %d/%d. Tauler %dx%d:\n\n", *errorsActuals, maxErrors, n, m);
        printTaulerJoc(m, n, joc, maxCombinations, cantonades, 0);

        int sortir;
        do{
            printf("\tSeleccionar caselles (0), sortir (1): ");
            scanf(" %d", &sortir);
        }while(sortir != 1 && sortir != 0);

        if(sortir)
            break;

        do{
            printf("\tEscull una columna (x = [1, %d] o tota la taula (%d)) o (0) per sortir: ", n, n + 1);
            scanf(" %d", &i);
        }while(i < 0 || i > n + 1);

        do{
            scanf("%c", &f);
        }while(f == ' ');
        if(f != '\n')
            clearBuffer();

        if(i != n + 1 && i != 0){
            do{
                system("cls");

                printf("\n\n\tUltima posicio triada (");

                if(i == -1 || j == -1){
                    printf(", )");
                }else printf("%d, %d)", i, j);
                printf(". Errors %d/%d. Tauler %dx%d:\n\n", *errorsActuals, maxErrors, n, m);
                printTaulerJoc(m, n, joc, maxCombinations, cantonades, i);

                printf("\n\tEscull totes les files de la columna %d que vulguis, escriu 0 per sortir:", i);

                printf("\n\t[1, %d] o tota la columna (%d): ", m, m + 1);
                scanf(" %d", &j);
                do{
                    scanf("%c", &f);
                }while(f == ' ');

                if(f != '\n')
                    clearBuffer();

                if(seleccio(m, n, joc, j - 1, i - 1, f == 'f')){
                    if(f != 'f' && joc[j - 1][i - 1].valor == '0'){
                        (*errorsActuals)++;
                    }
                }
            }while(j != 0 && *errorsActuals < maxErrors && j != m + 1);

            if(j == m + 1){
                j = 0;
                while(*errorsActuals < maxErrors && j < m){
                    if(!joc[j][i - 1].flag && seleccio(m, n, joc, j, i - 1, f == 'f')){
                        if(f != 'f' && joc[j][i - 1].valor == '0'){
                            (*errorsActuals)++;
                        }
                    }
                    j++;
                }
            }

        }else if (i == n + 1){
            i = 0;
            while(*errorsActuals < maxErrors && i < m){
                j = 0;
                while(*errorsActuals < maxErrors && j < n){
                    if(!joc[i][j].flag && seleccio(m, n, joc, i, j, f == 'f')){
                        if(f != 'f' && joc[i][j].valor == '0'){
                            (*errorsActuals)++;
                        }
                    }
                    j++;
                }
                i++;
            }
>>>>>>> Stashed changes
        }
    }
<<<<<<< Updated upstream
    else(joc[i][j].flag)
    {
        if(joc[i][j].flag?'F':' ')
        {
            posicio_correcta=false;
=======
    system("cls");

    if(*errorsActuals >= maxErrors){
        printf("\n\n\tHas arribat al limit de errors permesos, hauras de comen%car de nou.\n\n", 135);
    }else if (jocAcabat(m, n, joc)){
        printf("\n\n\tFelicitats has guanyat! Cometent %d errors.\n\n", *errorsActuals);
    } else {
        printf("\n\n\tLa partida quedara guardada. Quan vulguis continuar torna a seleccionar aquesta opcio.");
        return false;
    }

    printTaulerJoc(m, n, joc, maxCombinations, cantonades, 0);
    *errorsActuals = 0;
    restaurarJoc(m, n, joc);
    return true;

}

void taulerAleatori(int m, int n, casella_t joc[][MAXROWCOL]){
    /**< Fer que el tauler estigui entre un 20% i un 80% ple de cuadres, és a dir, caselles correctes. */
    int density = (rand() % 4 + 1) * 20; /**< {20, 40, 60, 80} */
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            char a = (rand() % 100 + 1) <= density;
            a = a + 48; /**< ascii('0') = 48 */
            joc[i][j].valor = a; /**< Escriure a la posicio del valor un '1' o '0' depenent del valor de 'a' */
            joc[i][j].revelat = false;/**< Cal també assegurar-se que la posició no es trobara en flag ni en revelat en true. */
            joc[i][j].flag = false;
>>>>>>> Stashed changes
        }
    }
    return posicio_correcta;
}
>>>>>>> Stashed changes
