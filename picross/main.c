#include "metodesPicross.h"
#include <time.h>

int main()
{
    srand(time(NULL));
    int m, n, maxErrors, opcio, max, maxCombinations;
    int errorsActuals = 0;
    bool carregatCorrectament = false;
    char fitxer[CADMAX], buffer;
    casella_t joc[MAXROWCOL][MAXROWCOL];

    printf("\n\n\t\t\t\t\t%c", 201);
    for(int i = 0; i < 36; i++){
        printf("%c", 205);
    }
    printf("%c", 187);

    printf("\n\t\t\t\t\t%c      Benvingut a pi(C)ross %c!      %c", 186, 169, 186);

    printf("\n\t\t\t\t\t%c", 200);
    for(int i = 0; i < 36; i++){
        printf("%c", 205);
    }
    printf("%c\n", 188);

    do{
        printMenu();
        printf("\n\tQuina opcio vols triar?  ");
        scanf(" %d", &opcio);
        scanf("%c", &buffer);
        switch(opcio){
        case 1:
            printf("\n\t\tHas triat la opcio 1.\n\n\tQuin es el nom del fitxer? (sense la extensio .txt)\n\t");
            fgets(fitxer, CADMAX, stdin);
            if(afegirExtensio(fitxer)){
                carregatCorrectament = carregarDades(fitxer, &m, &n, &maxErrors, joc);
                if(!carregatCorrectament){
                    printf("\tNo s'ha pogut carregar correctament el fitxer, potser l'has escrit malament o les dades que conte no son correctes.");
                }
            } else {
                printf("\tNo s'ha pogut afegir l'extensio del fitxer (\".txt\") correctament, el nom es mass llarg (max:%d).", CADMAX);
            }
            break;
        case 2:
            if(carregatCorrectament){
                max = m > n?m:n;
                maxCombinations = max % 2 != 0?max + 1: max;
                maxCombinations /= 2;
                maxCombinations++;
                char cantonades[n + m][maxCombinations];
                calcularCantonades(m, n, joc, maxCombinations, cantonades);
                int i = -1, j;
                while(!jocAcabat(m, n, joc) && errorsActuals < maxErrors){
                    system("cls");

                    printf("\n\n\tUltima posicio triada (");

                    if(i == -1){
                        printf(", )");
                    }else printf("%d, %d)", i, j);

                    printf(". Errors %d/%d.\n\n", errorsActuals, maxErrors);

                    printTaulerJoc(m, n, joc, maxCombinations, cantonades);

                    do{
                        printf("\tQuina casella vols triar(x y -> x = [1, %d], y = [1, %d])?\t", n, m);
                        scanf(" %d", &i);
                        scanf(" %d", &j);
                    }while(!seleccio(m, n, joc, j - 1, i - 1, false));
                    if(joc[j - 1][i - 1].valor == '0'){
                        errorsActuals++;
                    }
                }
                if(errorsActuals >= maxErrors){
                    printf("\n\n\tHas arribat al limtit de errors permesos, hauras de començar de nou.\n\n");
                    errorsActuals = 0;
                    restaurarJoc(m, n, joc);
                }else if(jocAcabat(m, n, joc)){
                    printf("\n\n\tFelicitats has guanyat! Cometent %d errors.\n\n", errorsActuals);
                }
            }
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 0:
            printf("Adeuuu!!\n");
            break;
        default:
            printf("Aquesta opcio no existeix...\n");
            break;
        }
        system("pause");
        system("cls");
    }while(opcio != 0);

//
//    printf("%d %d %d\n", m, n, maxErrors);
//
//    printTaulerJoc(m, n, joc, maxCombinations, cantonades);
//    boardToPBM("fitProva.pbm", m, n, joc);
//
//    printf("Joc acabat %d\n", jocAcabat(m, n, joc));
//
//    //IA(m, n  , maxErrors, joc, maxCombinations, cantonades);
//    generarFitxerAleatori("fitProva.txt", rand() % MAXROWCOL + 1, rand() % MAXROWCOL + 1, 10, 75);
    return 0;
}
