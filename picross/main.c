#include "metodesPicross.h"
#include <time.h>

int main()
{
    srand(time(NULL));
    int m, n, maxErrors, opcio;
    int errorsActuals = 0;
    bool carregatCorrectament = false, partidaAleatoria = false, partidaGuardada = false, primeraTriada = false;
    char fitxercarregat[CADMAX], fitxercreat[CADMAX];
    char taulersPath[] = "taulers\\";
    char pbmPath[] = "pbm\\";
    char pathFinal[CADMAX];
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
                    printf(" \"%s\"", fitxercarregat);
                }

            }else{
                if(carregatCorrectament){
                    printf("Tens un tauler carregat (\"%s\") de %dx%d amb un maxim de errors de %d, pots jugar-lo amb op.2 o generar un aleatori op.3.", fitxercarregat, n, m, maxErrors);
                }else {
                    printf("No tens cap tauler carregat, per carregar pots carregar un amb op.1 o pots generar un aleatori amb la op.3.");
                }
            }
            printf("\n\tPots consultar les normes del joc op.4 i les estrategies del joc op.5.\n\n");
        }
        printMenu();
        printf("\n\tQuina opcio vols triar?  ");
        scanf(" %d", &opcio);
        clearBuffer();

        switch(opcio){
        case 1:
            printf("\n\t\tHas triat la opcio 1 - Carregar tauler.\n\n\tQuin es el nom del fitxer? (sense la extensio .txt)\n\t");
            fgets(fitxercarregat, CADMAX, stdin);
            if(afegirExtensio(fitxercarregat, ".txt") && combinarPath(fitxercarregat, taulersPath, pathFinal)){
                carregatCorrectament = carregarDades(pathFinal, &m, &n, &maxErrors, joc);
                if(!carregatCorrectament){
                    printf("\tNo s'ha pogut carregar correctament el fitxer, potser l'has escrit malament, no existeix al path %s o no te les dades correctes.\n", taulersPath);
                    printf("%s\n", pathFinal);
                }
                partidaAleatoria = false;
                errorsActuals = 0;
                partidaGuardada = false;
            } else {
                printf("\tNo s'ha pogut afegir l'extensio del fitxer (\".txt\") o el path correctament, el nom es mass llarg (max:%d).\n", CADMAX - 9);
            }
            break;
        case 2:
            printf("\n\t\tHas triat la opcio 2 - Jugar tauler carregat.\n\n");
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

            opcio = 2;
            break;
        case 3:
            printf("\n\t\tHas triat la opcio 3 - Jugar tauler aleatori.\n\n");
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
            {
                system("cls");
                printf("\n\n\t\tEstrategies de pi(C)ross i de nonogrames en general:\n\n");
                printf("\tAccions particulars de pi(C)ross:\n\n");
                printf("\tLa manera que tenim de seleccionar posicions en el nostre programa es triar primer\n");
                printf("\tuna columna i despres triar totes les files que vulguis seleccionar dins d'aquesta.\n");
                printf("\tD'aquesta manera es poden triar caselles mes rapid sempre i quan hi hagin menys o\n");
                printf("\tmateixes columnes que files.\n\n");
                printf("\tL'altre manera que tenim de seleccionar caselles es, tant a l'hora de seleccionar\n");
                printf("\tcolumnes o caselles, pots seleccionar un nombre especial (columnes + 1/files + 1)\n");
                printf("\tper a que automaticament seleccionar totes les caselles que no estiguin marcades\n");
                printf("\tamb banderes (flags).\n\n");
                printf("\t\tFlags\n\n");
                printf("\tUn flag o bandera es una posicio en el tauler que l'usuari pot seleccionar per\n");
                printf("\tper marcarla com a espai buit sense haber de revelar-la. Aixo, no significa que\n");
                printf("\tl'usuari no la pugi revelar sino que si utilitzen el nombre especial marcat la maquina\n");
                printf("\tautomaticament no la triara.\n\n");
                printf("\tEstrategies:\n\n");
                printf("\t\tOverlap technique\n\n");
                printf("\tEn general es pot donar quan tenim una cantonada que compleix:\n\t");
                printf("\tsuma(cantonada) + (nombre(cantonada) - 1) >= ceil(caselles/2)\n");
                printf("\tLlavors tenim un cas de un overlap i, en el cas de que es compleixi = caselles i haura\n");
                printf("\tnomes una posible combinacio de blocs en aquella cantonada. On, suma(cantonada) es la\n");
                printf("\tsuma de tots els valors que hi ha a la cantonada, nombre(cantonada) es el nombre de\n");
                printf("\tnumeros que hi ha en aquella cantonada (nombre(2 2) = 2). Per exemple, en un tauler de\n");
                printf("\t5x5 les cantonades:\n");
                printf("\t\t\t|Cantonada|       Formula\t|Overlap|\n");
                printf("\t\t\t|    1    |    1 + 1 - 1 >= 3\t|  no\t|\n");
                printf("\t\t\t|   1 1   |    2 + 2 - 1 >= 3\t|  no!\t|\n");
                printf("\t\t\t|  1 1 1  |    3 + 3 - 1 >= 3\t|  si*\t|\n");
                printf("\t\t\t|   1 2   |    3 + 2 - 1 >= 3\t|  si\t|\n");
                printf("\t\t\t|   1 3   |    4 + 2 - 1 >= 3\t|  si*\t|\n");
                printf("\t\t\t|    2    |    2 + 1 - 1 >= 3\t|  no\t|\n");
                printf("\t\t\t|   2 2   |    4 + 2 - 1 >= 3\t|  si*\t|\n");
                printf("\t\t\t|    3    |    3 + 1 - 1 >= 3\t|  si\t|\n");
                printf("\t\t\t|    4    |    4 + 1 - 1 >= 3\t|  si\t|\n");
                printf("\t\t\t|    5    |    5 + 1 - 1 >= 3\t|  si*\t|\n");
            }
            break;
        case 5:
            printf("\n\t\tHas triat la opcio 5 - Generar fitxer aleatori.\n\n");
            printf("\tQuin nom tindra l'arxiu?(sense la extensio .txt)\n\t");
            fgets(fitxercreat, CADMAX, stdin);
            if(afegirExtensio(fitxercreat, ".txt") && combinarPath(fitxercreat, taulersPath, pathFinal)){
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

                    if(generarFitxerAleatori(pathFinal, newm, newn, errors, (rand() % 4 + 1) * 20)){
                        printf("\n\tS'ha generat l'arxiu %s de %dx%d al directori %s.", fitxercreat, newn, newm, taulersPath);
                    }else {
                        printf("\n\tNO s'ha pogut generar l'arxiu %s.", fitxercreat);
                    }
                }else printf("\n\tNO s'ha pogut afegir l'extensio el nom es massa llarg.");
            break;
        case 6:
            printf("\n\t\tHas triat la opcio 6 - Tauler a imatge PBM.\n\n");
            if(carregatCorrectament || partidaAleatoria){
                printf("\tQuin nom tindra l'arxiu?(sense la extensio .pbm)\n\t");
                fgets(fitxercreat, CADMAX, stdin);
                if(afegirExtensio(fitxercreat, ".pbm") && combinarPath(fitxercreat, pbmPath, pathFinal)){
                    if(boardToPBM(pathFinal, m, n, joc)){
                        printf("\n\tS'ha guardat el tauler com a imatge a l'arxiu %s en el directori %s.", fitxercreat, pbmPath);
                    }else {
                        printf("\n\tNO s'ha pogut el tauler com a imatge a l'arxiu %s, potser el directori %s no esta creat.", fitxercreat, pbmPath);
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

    return 0;
}
