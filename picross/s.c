//int cantonadaOptima(int start, int end, char cantonades[][MAXCOMBINATIONS]){
//    int optim = 0;
//    int prevSum = 0;
//    int current;
//    for(int i = start; i < end; i++){
//        int sum = 0;
//        current = cantonades[i];
//        while(current != 0){
//            sum += current % 10;
//            current /= 10;
//        }
//        if(sum > prevSum){
//            optim = i;
//            prevSum = sum;
//        }
//    }
//    return optim;
//}

//void IA(int m, int n, int e, casella_t joc[][MAXROWCOL], char cantonades[][MAXCOMBINATIONS]){
//    int errorsMaquina = 0;
//    int cantonadesMaquina[m + n];
//
//    for(int i = 0; i < m + n; i++){
//        cantonadesMaquina[i] = cantonades[i];
//    }
//
//    while(!jocAcabat(m, n, joc) && errorsMaquina <= e){
//        int optimaM = cantonadaOptima(0, m, cantonadesMaquina);
//        int optimaN = cantonadaOptima(m, m + n, cantonadesMaquina) - m;
//        printf("%d, %d\n", optimaM, optimaN);
//
//        joc[optimaM][optimaN].revelat = true;
//        if(joc[optimaM][optimaN].valor == '0')
//            errorsMaquina++;
//
//
//
//        printTaulerJoc(m, n, joc, cantonades);
//        break;
//    }
//
//    if(errorsMaquina > e){
//        printf("La maquina ha guanyat ");
//    } else printf("La maquina ha perdut en ");
//    printf("%d errors.", errorsMaquina);
//}
