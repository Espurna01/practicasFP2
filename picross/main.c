#include "metodesPicross.h"
#include <time.h>

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

    //IA(m, n  , maxErrors, joc, maxCombinations, cantonades);
    generarFitxerAleatori("fitProva.txt", rand() % MAXROWCOL + 1, rand() % MAXROWCOL + 1, 10, 75);
    return 0;
}
