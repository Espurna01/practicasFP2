#ifndef METODESPICROSS_H_INCLUDED
#define METODESPICROSS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAXROWCOL 30
#define CENTINELLA -1
#define CADMAX 50

typedef struct{
    char valor; /**< valor de la casella {0, 1}; 0 si no hi ha un cuadrat, 1 si hi ha. */
    bool revelat; /**< si l'usuari ha triat aquesta casella */
    bool flag; /**< si l'usuari ha marcat aquesta casella com a buida (com les banderes del buscamines) */
}casella_t;

/** \brief Llegei el fitxer anomenat filename i guarda les dades de la dimensió i dels errors en enters passats per
 *         referència, i guarda la taula en casella_t joc[][MAXROWCOL].
 *
 * \param filename: nom del fitxer, string.
 * \param m: on es guardaran les files, enter.
 * \param n: on es guardaran les columnes, enter.
 * \param e: on es guardaran els errors, enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \return true si s'ha pogut llegir i carregar les dades, fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool carregarDades(char *filename, int *m, int *n, int *e, casella_t joc[][MAXROWCOL]);

/** \brief Calcular els valors de cada fila i columna que corresponen a la disposició del tauler.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param maxCombinations: columnes de la matriu, correspon a (veure linea 10 del main.c), enter.
 * \param cantonades[][maxCombinations]: matriu on es guarden els valors primer les files (m) i després les columnes (n)
 *        , char[][] (s'assumeix que el valor de MAXROWCOL sempre serà menor o igual a 100, per tant, la pitjor
 *        disposició entra en un char).
 * \return void.
 * (Matias Ariel Larrosa Babio)
 */
void calcularCantonades(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

/** \brief Generar un fitxer aleatori de nom filename amb les dades passades i amb una densitat [0-100] corresponent.
 *
 * \param filename: nom del fitxer, string.
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param e: errors máxims de la partida, enter.
 * \param density: densitat de la taula [0, 100] en percentatge, enter.
 * \return true si s'ha pogut crear el fitxer, fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool generarFitxerAleatori(char *filename, int m, int n, int e, int density);

/** \brief Comprova si el joc s'ha acabat, si totes les caselles amb un cuadrat s'han revelat. No comprovà si s'ha acabat
 *         pel nombre d'errors.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \return true si s'han revelat totes les posicions correctes, fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool jocAcabat(int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief Transforma el tauler de joc en una imatge PBM (Portable BitMap) anomenada filename.
 *
 * \param filename: nom del fitxer, string.
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \return true si s'ha pogut generar el fitxer fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool boardToPBM(char *filename, int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief Printa el tauler de joc d'una forma bonica.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param maxCombinations: columnes de la matriu, correspon a (veure linea 10 del main.c), enter.
 * \param cantonades[][maxCombinations]: matriu on es guarden els valors de disposició del tauler de joc.
 * \return void.
 * (Matias Ariel Larrosa Babio)
 */
void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

/** \brief Seleccionar una posició en el tauler de joc. Revela la poscició i, j o si
 *         es vol posar un flag es posarà un flag.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param i: posició que ha triat l'usuari (files), enter.
 * \param j: posició que ha triat l'usuaari (columnes), enter.
 * \return true si la posició es correcte (dins de la taula) i no està revelada i fals en cas contrari.
 * (Pau Reverté)
 */
bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j, bool flag);

/** \brief Escriu el menu per pantalla.
 */
void printMenu();

/** \brief Afegeix la extensio al final de la cadena de text i elimina el caracter '\n' del final.
 *
 * \param *fitxer: cadena de text a modificar, string.
 * \param *extensio: cadena de text que conte l'extensio a afegir, string.
 * \return true si ha pogut afegir l'extensió i fals si no ha pogut.
 * (Pablo)
 */
bool afegirExtensio(char *fitxer, char *extensio);

/** \brief Ajunta un path relatiu i el nom d'un fitxer en pathFinal
 *
 * \param *fitxer: nom del fitxer a crear, string.
 * \param *path: path del directori relatiu, string.
 * \param *pathFinal: path relatiu del fitxer, string.
 * \return true si s'han pogut ajuntar el path i el fitxer, false si no.
 * (Matias Ariel Larrosa Babio)
 */
bool combinarPath(char *fitxer, char *path, char *pathFinal);

/** \brief Restaura tot el tauler de joc deixant totes les posicions modificades (revelades/flag).
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * (Matias Ariel Larrosa Babio)
 */
void restaurarJoc(int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief A partir d'un tauler de joc, demana a l'usuari posicions fins a que perd el joc o el guanya.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param *errorsActuals: errors actuals de l'usuari, enter.
 * \param maxErrors: errors maxims del tauler actual, enter.
 * \return true si el joc s'ha acabat (guanyat o perdut) fals si l'ususari l'ha pospos.
 * (Matias Ariel Larrosa Babio)
 */
bool jugar(int m, int n, casella_t joc[][MAXROWCOL], int *errorsActuals, int maxErrors);

/** \brief Emplena un tauler de m x n aleatoriament.
 *
 * \param m: dimensió de la taula (files), enter.
 * \param n: dimensió de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * (Matias Ariel Larrosa Babio)
 */
void taulerAleatori(int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief Neteja el buffer de l'usuari '\n' i els possibles espais i caracters brossa que hagi deixat.
 *
 * (Matias Ariel Larrosa Babio)
 */
void clearBuffer();
#endif // METODESPICROSS_H_INCLUDED
