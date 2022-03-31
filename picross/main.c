#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAX 10
#define MAXROWCOL 19

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

void calcularCantonades(int m, int n, casella_t joc[][MAXROWCOL], int cantonades[]){
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

void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int cantonades[]){
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
    printf("\t\t%c", 201);
    for(int i = 0; i < n*2 + digitsMaxCantonadesM;i++)
        printf("%c", 205);
    printf("%c\n", 187);
    for(int i = 0; i < digitsMaxCantonadesN; i++){
        printf("\t\t%c", 186);
        for(int j = 0; j < digitsMaxCantonadesM; j++){
            printf(" ");
        }
        for(int j = 0; j < n; j++){
            printf("|");
            int valor = cantonades[m + j];
            int reverse = 0;
            while (valor > 0){
                reverse = reverse * 10 + valor % 10;
                valor /= 10;
            }
            for(int k = 0; k < i; k++){
                reverse = reverse/10;
            }
            if(reverse % 10 != 0)
                printf("%d", reverse % 10);
            else printf(" ");
        }
        printf("%c\n", 186);
    }

    for(int i = 0; i < m; i++){
        printf("\t\t%c", 186);
        for(int j = 0; j < (digitsMaxCantonadesM + n * 2); j++){
            printf("%c", 196);
        }
        printf("%c\n", 186);
        int valor = cantonades[i];
        int digits = 0;
        while(valor != 0){
            valor /= 10;
            digits++;
        }
        printf("\t\t%c", 186);
        if(cantonades[i] != 0)
            printf("%d", cantonades[i]);
        for(int j = 0; digits + j < digitsMaxCantonadesM;j++){
            printf(" ");
        }
        for(int j = 0; j < n; j++){
            printf("|");
            if(!joc[i][j].revelat)
                printf("%c", joc[i][j].valor == '1'?254:'X');
            else printf("%c", joc[i][j].flag?'F':'?');
        }
        printf("%c\n", 186);
    }
    printf("\t\t%c", 200);
    for(int i = 0; i < n*2 + digitsMaxCantonadesM;i++)
        printf("%c", 205);
    printf("%c\n", 188);
}

bool generarFitxerAleatori(char *filename, int m, int n, int e){
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
            int rand10 = rand() % 10 + 1;
            char a = rand() % rand10 > rand10/2;
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

int cantonadaOptima(int start, int end, int cantonades[]){
    int optim = 0;
    int prevSum = 0;
    int current;
    for(int i = start; i < end; i++){
        int sum = 0;
        current = cantonades[i];
        while(current != 0){
            sum += current % 10;
            current /= 10;
        }
        if(sum > prevSum){
            optim = i;
            prevSum = sum;
        }
    }
    return optim;
}

void IA(int m, int n, int e, casella_t joc[][MAXROWCOL], int cantonades[]){
    int errorsMaquina = 0;
    int cantonadesMaquina[m + n];

    for(int i = 0; i < m + n; i++){
        cantonadesMaquina[i] = cantonades[i];
    }

    while(!jocAcabat(m, n, joc) && errorsMaquina <= e){
        int optimaM = cantonadaOptima(0, m, cantonadesMaquina);
        int optimaN = cantonadaOptima(m, m + n, cantonadesMaquina) - m;
        printf("%d, %d\n", optimaM, optimaN);

        joc[optimaM][optimaN].revelat = true;
        if(joc[optimaM][optimaN].valor == '0')
            errorsMaquina++;



        printTaulerJoc(m, n, joc, cantonades);
        break;
    }

    if(errorsMaquina > e){
        printf("La maquina ha guanyat ");
    } else printf("La maquina ha perdut en ");
    printf("%d errors.", errorsMaquina);
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
            fwrite(&(joc[i][j].valor), 1, 1, fit);
        }
    }
    fclose(fit);
    return true;
}

int main()
{
    srand(time(NULL));
    int m, n, maxErrors;
    casella_t joc[MAXROWCOL][MAXROWCOL];
    carregarDades("fitProva.txt", &m, &n, &maxErrors, joc);

    int cantonades[n + m];
    calcularCantonades(m, n, joc, cantonades);

    printf("%d %d %d\n", m, n, maxErrors);

    printTaulerJoc(m, n, joc, cantonades);
    boardToPBM("fitProva.pbm", m, n, joc);

    printf("Joc acabat %d\n", jocAcabat(m, n, joc));

    //IA(m, n  , maxErrors, joc, cantonades);
    //generarFitxerAleatori("fitProva.txt", rand() % MAXROWCOL + 1, rand() % MAXROWCOL + 1, 10);
    return 0;
}
