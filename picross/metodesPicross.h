#ifndef METODESPICROSS_H_INCLUDED
#define METODESPICROSS_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAXROWCOL 50
#define CENTINELLA -1

typedef struct{
    char valor; /**< valor de la casella {0, 1}; 0 si no hi ha un cuadrat, 1 si hi ha. */
    bool revelat; /**< si l'usuari ha triat aquesta casella */
    bool flag; /**< si l'usuari ha marcat aquesta casella com a buida (com les banderes del buscamines) */
}casella_t;

/** \brief Llegei el fitxer anomenat filename i guarda les dades de la dimensi� i dels errors en enters passats per
 *         refer�ncia, i guarda la taula en casella_t joc[][MAXROWCOL].
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

/** \brief Calcular els valors de cada fila i columna que corresponen a la disposici� del tauler.
 *
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param maxCombinations: columnes de la matriu, correspon a (veure linea 10 del main.c), enter.
 * \param cantonades[][maxCombinations]: matriu on es guarden els valors primer les files (m) i despr�s les columnes (n)
 *        , char[][] (s'assumeix que el valor de MAXROWCOL sempre ser� menor o igual a 100, per tant, la pitjor
 *        disposici� entra en un char).
 * \return void.
 * (Matias Ariel Larrosa Babio)
 */
void calcularCantonades(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

/** \brief Generar un fitxer aleatori de nom filename amb les dades passades i amb una densitat [0-100] corresponent.
 *
 * \param filename: nom del fitxer, string.
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param e: errors m�xims de la partida, enter.
 * \param density: densitat de la taula [0, 100] en percentatge, enter.
 * \return true si s'ha pogut crear el fitxer, fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool generarFitxerAleatori(char *filename, int m, int n, int e, int density);

/** \brief Comprova si el joc s'ha acabat, si totes les caselles amb un cuadrat s'han revelat. No comprov� si s'ha acabat
 *         pel nombre d'errors.
 *
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \return true si s'han revelat totes les posicions correctes, fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool jocAcabat(int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief Transforma el tauler de joc en una imatge PBM (Portable BitMap) anomenada filename.
 *
 * \param filename: nom del fitxer, string.
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \return true si s'ha pogut generar el fitxer fals en cas contrari.
 * (Matias Ariel Larrosa Babio)
 */
bool boardToPBM(char *filename, int m, int n, casella_t joc[][MAXROWCOL]);

/** \brief Printa el tauler de joc d'una forma bonica.
 *
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param maxCombinations: columnes de la matriu, correspon a (veure linea 10 del main.c), enter.
 * \param cantonades[][maxCombinations]: matriu on es guarden els valors de disposici� del tauler de joc.
 * \return void.
 * (Matias Ariel Larrosa Babio)
 */
void printTaulerJoc(int m, int n, casella_t joc[][MAXROWCOL], int maxCombinations, char cantonades[][maxCombinations]);

/** \brief Seleccionar una posici� en el tauler de joc. Revela la poscici� i, j o si
 *         es vol posar un flag es posar� un flag.
 *
 * \param m: dimensi� de la taula (files), enter.
 * \param n: dimensi� de la taula (columnes), enter.
 * \param joc[][MAXROWCOL]: tauler de joc, casella_t.
 * \param i: posici� que ha triat l'usuari (files), enter.
 * \param j: posici� que ha triat l'usuaari (columnes), enter.
 * \return true si la posici� es correcte (dins de la taula) i no est� revelada i fals en cas contrari.
 * (Pau Revert�)
 */
bool seleccio(int m, int n, casella_t joc[][MAXROWCOL], int i, int j, bool flag);


#endif // METODESPICROSS_H_INCLUDED