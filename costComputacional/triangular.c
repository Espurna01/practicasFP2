#include <stdbool.h>

bool es_triangular_fb(unsigned int x){
    unsigned int nTriangular = 1;
    for(unsigned int i = 2; nTriangular < x; i++){
        nTriangular += i;
    }
    return nTriangular == x;
}

bool es_triangular_op(unsigned int x){
    unsigned long long arrel = x;
    arrel = arrel*8 + 1;
    unsigned long long n =(unsigned long long) sqrt(arrel);
    return n*n == arrel;
}

bool es_triangular_tau(unsigned int x, unsigned int taula[], int max){
    for(int i = 0;i < max;i++){
        if(x == taula[i])
            return true;
    }
    return false;
}

void calcula_triangulars(unsigned int taula[], int max){
    unsigned int i = 2;
    taula[0] = 1;
    while(i <= max){
        taula[i - 1] = taula[i - 2] + i;
        i++;
    }
}
