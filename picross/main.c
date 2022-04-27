#include "metodesPicross.h"
#include <time.h>

int main()
{
    srand(time(NULL));
    int m, n, maxErrors, opcio;
    int errorsActuals = 0;
    bool carregatCorrectament = false, partidaAleatoria = false, partidaGuardada = false, primeraTriada = false;
    char fitxer[CADMAX], pbm[CADMAX];
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
        if(primeraTriada){
            printf("\n\n\t");
            if(partidaGuardada){
                printf("Tens una partida (");
                if(partidaAleatoria)
                    printf("aleatoria");
                else printf("carregada");
                printf(") guardada de %dx%d amb %d/%d errors.", n, m, errorsActuals, maxErrors);
                if(!partidaAleatoria){
                    printf(" \"%s\"", fitxer);
                }

            }else{
                if(carregatCorrectament){
                    printf("Tens un tauler carregat (\"%s\") de %dx%d amb un maxim de errors de %d, pots jugar-lo amb op.2 o generar un aleatori op.3.", fitxer, n, m, maxErrors);
                }else {
                    printf("No tens cap tauler carregat, per carregar pots carregar un amb op.1 o pots generar un aleatori amb la op.3.");
                }

            }
            printf("\n\tPots consultar les normes del joc op.4, les estrategies del joc op.5 i els records personals op.6.\n\n");
        }
        printMenu();
        printf("\n\tQuina opcio vols triar?  ");
        scanf(" %d", &opcio);
        clearBuffer();

        switch(opcio){
        case 1:
            printf("\n\t\tHas triat la opcio 1.\n\n\tQuin es el nom del fitxer? (sense la extensio .txt)\n\t");
            fgets(fitxer, CADMAX, stdin);
            if(afegirExtensio(fitxer, ".txt")){
                carregatCorrectament = carregarDades(fitxer, &m, &n, &maxErrors, joc);
                if(!carregatCorrectament){
                    printf("\tNo s'ha pogut carregar correctament el fitxer, potser l'has escrit malament o les dades que conte no son correctes.");
                }
                partidaAleatoria = false;
            } else {
                printf("\tNo s'ha pogut afegir l'extensio del fitxer (\".txt\") correctament, el nom es mass llarg (max:%d).", CADMAX);
            }
            break;
        case 2:
            printf("\n\t\tHas triat la opcio 2.\n\n");
            if(carregatCorrectament){
                if(partidaGuardada){
                    printf("\tTens una partida d'un tauler CARREGAT ja comen%cada amb %d/%d errors.", 135, errorsActuals, maxErrors);
                    do{
                        printf("\n\tVols continuar la partida(1/0)? ");
                        scanf(" %d", &opcio);
                    }while(opcio != 1 && opcio != 0);

                    if(!opcio){
                        restaurarJoc(m, n, joc);
                    }
                }
                partidaGuardada = !jugar(m, n, joc, &errorsActuals, maxErrors);
                partidaAleatoria = false;
            }else {
                if(partidaAleatoria){
                    printf("\tHi ha generada una partida ALEATORIA si vols jugar un tauler aleatori selecciona (op.3) sino, carrega un tauler(op.1)...\n");
                }else
                    printf("\tNo hi ha cap tauler carregat hauras de carregar un...(opcio 1)\n");
            }
            break;
        case 3:
            printf("\n\t\tHas triat la opcio 3.\n\n");
            if(partidaGuardada){
                printf("\tTens una partida d'un tauler");
                if(partidaAleatoria)
                    printf(" ALEATORI ");
                else printf(" CARREGAT ");
                printf("ja comen%cat amb %d/%d errors.", 135, errorsActuals, maxErrors);

                do{
                    printf("\n\tVols sobreescriure-la(1/0)? ");
                    scanf(" %d", &opcio);
                }while(opcio != 1 && opcio != 0);
            } else opcio = 1;

            if(opcio){
                m = (rand() % 3 + 1) * 5;   /**< {5, 10, 15} */
                n = (rand() % 3 + 1) * 5;   /**< {5, 10, 15} */
                maxErrors = (m * n)/(m + n);
                taulerAleatori(m, n, joc);
                partidaGuardada = !jugar(m, n, joc, &errorsActuals, maxErrors);
                partidaAleatoria = true;
                carregatCorrectament = false;
            } else if(partidaAleatoria){
                partidaGuardada = !jugar(m, n, joc, &errorsActuals, maxErrors);
                partidaAleatoria = true;
                carregatCorrectament = false;
            }

            opcio = 3;

            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            printf("\n\t\tHas triat la opcio 7.\n\n");
            printf("\tQuin nom tindra l'arxiu?(sense la extensio .txt)\n\t");
            fgets(pbm, CADMAX, stdin);
            if(afegirExtensio(pbm, ".txt")){
                    int newm, newn, errors;
                    do{
                        printf("\n\tQuantes files vols que tingui([1, %d])? ", MAXROWCOL);
                        scanf(" %d", &newm);
                    }while(newm < 1 || newm > MAXROWCOL);

                    do{
                        printf("\n\tQuantes columnes vols que tingui([1, %d])? ", MAXROWCOL);
                        scanf(" %d", &newn);
                    }while(newn < 1 || newn > MAXROWCOL);

                    do{
                        printf("\n\tQuants errors maxims vols que tingui un tauler de %dx%d? ", newm, newn);
                        scanf(" %d", &errors);
                    }while(errors <= 0);

                    if(generarFitxerAleatori(pbm, newm, newn, errors, (rand() % 4 + 1) * 20)){
                        printf("\n\tS'ha generat l'arxiu %s de %dx%d.", pbm, newn, newm);
                    }else {
                        printf("\n\tNO s'ha pogut generar l'arxiu %s.", pbm);
                    }
                }else printf("\n\tNO s'ha pogut afegir l'extensio el nom es massa llarg.");
            break;
        case 8:
            printf("\n\t\tHas triat la opcio 8.\n\n");
            if(carregatCorrectament || partidaAleatoria){
                printf("\tQuin nom tindra l'arxiu?(sense la extensio .pbm)\n\t");
                fgets(pbm, CADMAX, stdin);
                if(afegirExtensio(pbm, ".pbm")){
                    if(boardToPBM(pbm, m, n, joc)){
                        printf("\n\tS'ha guardat el tauler com a imatge a l'arxiu %s.", pbm);
                    }else {
                        printf("\n\tNO s'ha pogut el tauler com a imatge a l'arxiu %s.", pbm);
                    }
                }else printf("\n\tNO s'ha pogut afegir l'extensio el nom es massa llarg.");
            }
            break;
        case 0:
            printf("\n\tAdeuuu!!\n");
            break;
        default:
            printf("Aquesta opcio no existeix...\n");
            break;
        }
        printf("\n\t");
        system("pause");
        system("cls");
        primeraTriada = true;
    }while(opcio != 0);

    printf("He salido opcio %d", opcio);
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
