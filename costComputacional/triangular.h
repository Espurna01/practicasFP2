#ifndef TRIANGULAR_H_INCLUDED
#define TRIANGULAR_H_INCLUDED
#include <stdbool.h>

bool es_triangular_fb(unsigned int x);
bool es_triangular_op(unsigned int x);
void calcula_triangulars(unsigned int taula[], int max);
bool es_triangular_tau(unsigned int x, unsigned int taula[], int max);

#endif // TRIANGULAR_H_INCLUDED
