#ifndef METODESPICROSS_H_INCLUDED
#define METODESPICROSS_H_INCLUDED
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

/** \brief Llegei el fitxer anomenat filename i guarda les dades de la dimensi� i dels errors en enters passats per
 *         par�metre i
 *
 * \param
 * \param
 * \return
 *
 */
bool carregarDades(char* filename, int *m, int *n, int *e, casella_t joc[][MAXROWCOL]);

void calcularCantonades(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

bool generarFitxerAleatori(char *filename, int m, int n, int e, int density);

bool jocAcabat(int m, int n, casella_t joc[][MAXROWCOL]);

bool boardToPBM(char *filename, int m, int n, casella_t joc[][MAXROWCOL]);

void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

/** \brief Seleccionar una posici� en el tauler de joc. Revela la poscici� i, j.
 *
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param i: posici� que ha triat l'usuari (files), enter.
 * \param j: posici� que ha triat l'usuaari (columnes), enter.
 * \return true si la posici� es correcte (dins de la taula) i fals en cas contrari.
 * (Pau Revert�)
 */

bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j);



#endif // METODESPICROSS_H_INCLUDED
