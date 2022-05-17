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

bool carregarDades(char* filename, int *m, int *n, int *e, casella_t joc[][MAXROWCOL], int *pixelsRestants){
    FILE *fit = fopen(filename, "r");
    char buffer;
    *pixelsRestants = 0;
    if(fit == NULL){
        fclose(fit);
        return false;
    }
    fscanf(fit, "%d %d %d", m, n, e);
    buffer = 'a';
    while(buffer != '\n')
        fscanf(fit, "%c", &buffer);
    int count = 0;
    for(int i = 0; i < *m && !feof(fit); i++){
        for(int j = 0; j < *n && !feof(fit); j++){
            fscanf(fit, "%c", &buffer);
            if(buffer == '1'){
                joc[i][j].valor = buffer;
                (*pixelsRestants)++;
            }
            else joc[i][j].valor = '0';
            joc[i][j].revelat = false;
            joc[i][j].flag = false;
            count++;
        }
        fscanf(fit, "%c", &buffer);
    }
    fclose(fit);
    return count == *m * *n;
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

void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations], int marca){
    int maxBlocsM = 0;
    int maxDigitsM = 0;     /**< El nombre maxim total de digits d'una cantonada (digitsM(3 1) = 2) de les files */
    int maxBlocsN = 0;      /**< El nombre maxim de blocs en una columna */
    int maxDigitsN = 0;     /**< El nombre maxim de digits d'un nombre entre totes les cantonades de les columnes */
                            /**< Nomes es relevant si hi ha un bloc de +9 caselles consecutives */
    /**< Recorro els valors de les cantonades fent les comprovacions necesaries, */
    /**< les primeres posicions m posicions [0, m-1] corresponen a les cantonades de les files */
    /**< les següents n posicions corresponen a les columnes [m-1, m - 1 + n] */
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
    /**< Amb les dades obtingudes se com d'ample sera el tauler */
    /**< en un tauler de 5x5 per exemple on maxDigitsM sigui 3 i maxDigitsN sigui 1 */
    /**< l'amplada sera 6 (per escriure els valors de les cantonades horitzontals) + 10 (per escriure els valors verticals) = 16 */
    printf("\t\t%c", 201);
    for(int i = 0; i < maxBlocsM * 2 + (maxDigitsN - 1) * n + n * 2;i++)
        printf("%c", 205);
    printf("%c\n", 187);
    /**< Aquest primer bucle escriu les cantonades verticals de la taula */
    /**
                ╔══════════════╗
                ║    | | | |1| ║
                ║    |2| | |1| ║
                ║    |2|4|1|1|2║y
    */
    for(int i = 0; i < maxBlocsN; i++){
        printf("\t\t%c", 186);
        for(int j = 0; j < maxBlocsM * 2; j++){
            printf(" ");
        }
        /**< i: index actual de la cantonada que s'esta llegint */
        /**<    ╔══════════════╗
            i=0 ║    | | | |1| ║
            i=1 ║    |2| | |1| ║
            i=2 ║    |2|4|1|1|2║y
        */
        for(int j = 0; j < n; j++){
            printf("|");

            /**< Blocs de la cantonada que s'esta llegint actualment */
            /**
                j:    0 1 2 3 4
                ╔══════════════╗
                ║    | | | |1| ║
                ║    |2| | |1| ║
                ║    |2|4|1|1|2║y
            */
            int blocs;
            for(blocs = 0; cantonades[j + m][blocs] != CENTINELLA; blocs++);
            /**< Vull que les cantonades s'escriguin pegades al tauler */
            /**< Si l'index + el nombre de blocs de la cantonada actual es < */
            /**< que el nombre maxim de blocs del tauler escriure un espai */
            if(i + blocs < maxBlocsN){
                for(int k = 0; k < maxDigitsN; k++){
                    printf(" ");
                }
            }else{
                /**< i - maxBlocsM + blocs correspon a l'index que he d'escriure */
                /**< (2 2) -> 2 - 3 + 2 = 1 */
                printf("%d", cantonades[j + m][i - maxBlocsN + blocs]);
                /**< He de comprovar que la cantonada escrita tingui els digits corresponents */
                if(maxDigitsN != 1){
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
        }
        printf("%c", 186);
        if(i == maxBlocsN - 1)
            printf("y");
        printf("\n");
    }

    /**< Aquest segon bucle escriura les cantonades horitzontals i la taula */
    /**
                ║──────────────║
                ║ 1 2| | | | | ║1
                ║──────────────║
                ║ 2 1| | | | | ║2
                ║──────────────║
                ║   3| | | | | ║3
                ║──────────────║
                ║   2| | | | | ║4
                ║──────────────║
                ║ 2 1| | | | | ║5
                ╚══════════════╝
                    x ->
    */
    for(int i = 0; i < m; i++){
        /**< La linea de separacio entre cada fila */
        /**< ║──────────────║ */
        printf("\t\t%c", 186);
        for(int j = 0;j < maxBlocsM * 2 + (maxDigitsN - 1) * n + n * 2; j++){
            printf("%c", 196);
        }
        printf("%c\n", 186);
        printf("\t\t%c", 186);

        int blocs;
        for(blocs = 0; cantonades[i][blocs] != CENTINELLA; blocs++);
        /**< Si blocs != 0 haure de calcular si fa falta afegir espais extres */
        if(blocs != 0){
            int digits = 0;

            for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
                char cantonada = cantonades[i][j];
                while(cantonada != 0){
                    digits++;
                    cantonada /= 10;
                }
            }

            /**< maxBlocsM * 2 son els espais totals i blocs + digits - 1 son */
            /**< es el espai que aquella cantonada ocupa */
            for(int j = 0;j < maxBlocsM * 2 - (blocs + (digits - 1));j++)
                printf(" ");
        }else{
            for(int j = 0; j < maxBlocsM * 2; j++)
                printf(" ");
        }

        /**< Escriu les cantonades horitzontals del tauler */
        /**< ║ 1 2 */
        for(int j = 0; cantonades[i][j] != CENTINELLA; j++){
            if(j == 0)
                printf("%d", cantonades[i][j]);
            else printf(" %d", cantonades[i][j]);
        }
        /**< Escriu una fila del tauler cada cop */
        /**< | | | | | ║1 */
        for(int j = 0; j < n; j++){
            printf("|");
            if(joc[i][j].revelat)
                printf("%c", joc[i][j].valor == '1'?254:'X');
            else printf("%c", joc[i][j].flag?'F':' ');
            for(int k = 1; k < maxDigitsN; k++){
                printf(" ");
            }
        }
        /**< Numeros de cada fila per ajudar a l'usuari a visualitzar una posicio en el tauler */
        printf("%c%d\n", 186, i + 1);
    }

    printf("\t\t%c", 200);
    for(int i = 0; i < maxBlocsM * 2 + (maxDigitsN - 1) * n + n * 2;i++)
        printf("%c", 205);
    printf("%c\n\t\t", 188);

    for(int j = 0; j < maxBlocsM * 2; j++){
        printf(" ");
    }

    /**< Si marca != 0 (true) escriura una flecha a la columna indicada. */
    if(marca){
        printf("  ");
        for(int i = 0; i < (marca - 1) * (maxDigitsN + 1); i++){
            printf(" ");
        }
        for(int i = 0; i < maxDigitsN; i++){
            printf("%c", 30);
        }
    }else printf("x ->");

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
    printf("\t\t4. Instruccions.\n");
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

void restaurarJoc(int m, int n, casella_t joc[][MAXROWCOL], int *pixelsRestants){
    *pixelsRestants = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            joc[i][j].flag = false;
            joc[i][j].revelat = false;
            if(joc[i][j].valor == '1')
                (*pixelsRestants)++;
        }
    }
}

void clearBuffer(){
    char buffer = 0;
    while(buffer != '\n'){
        scanf("%c", &buffer);
    }
}

bool jugar(int m, int n, casella_t joc[][MAXROWCOL], int *errorsActuals, int maxErrors, int *pixelsRestants){
    int maxCombinations = m > n?m:n;
    maxCombinations = maxCombinations + maxCombinations % 2;
    maxCombinations /= 2;
    maxCombinations++;
    char cantonades[n + m][maxCombinations];
    calcularCantonades(m, n, joc, maxCombinations, cantonades);

    int i = -1, j = -1;
    while(*errorsActuals < maxErrors && *pixelsRestants != 0){
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
            clearBuffer();
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
                    }else if(f != 'f')
                        (*pixelsRestants)--;
                }
            }while(j != 0 && *errorsActuals < maxErrors && j != m + 1 && *pixelsRestants != 0);

            if(j == m + 1){
                j = 0;
                while(*errorsActuals < maxErrors && j < m){
                    if(!joc[j][i - 1].flag && seleccio(m, n, joc, j, i - 1, f == 'f')){
                        if(f != 'f' && joc[j][i - 1].valor == '0'){
                            (*errorsActuals)++;
                        }else if(f != 'f') (*pixelsRestants)--;
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
                        }else if(f != 'f') (*pixelsRestants)--;
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
    }else if (*pixelsRestants == 0){
        printf("\n\n\tFelicitats has guanyat! Cometent %d errors.\n\n", *errorsActuals);
    } else {
        printf("\n\n\tLa partida quedara guardada. Quan vulguis continuar torna a seleccionar aquesta opcio.");
        return false;
    }

    printTaulerJoc(m, n, joc, maxCombinations, cantonades, 0);
    *errorsActuals = 0;
    restaurarJoc(m, n, joc, pixelsRestants);
    return true;

}

void taulerAleatori(int m, int n, casella_t joc[][MAXROWCOL], int *pixelsRestants){
    int density = (rand() % 4 + 1) * 20; /**< {20, 40, 60, 80} */
    *pixelsRestants = 0;
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            char a = (rand() % 100 + 1) <= density;
            if(a) (*pixelsRestants)++;
            a = a + '0';
            joc[i][j].valor = a;
            joc[i][j].revelat = false;
            joc[i][j].flag = false;
        }
    }
}
