/**
 * @file point.h
 *
 * @brief file di dichiarazione della struct point
 *
 * File di dichiarazione della struct point.
 * Contiene anche la dichiarazione delle funzioni per lettura e scrittura su stream.
 * VIene dichiarato anche un funtore per confrontare due punti.
 */
#ifndef POINT_H
#define POINT_H

#include <iostream>

/**
 * @brief struct point
 *
 * Rappresenta un punto con due coordinate intere.
 */
struct point
{
    int x;
    int y;
};

/**
 * @brief funtore di confronto tra due punti
 *
 * Funtore di confronto tra due punti.
 */
struct ArePointEqual
{
    /**
     * @brief operatore () di confronto tra due punti
     *
     * Ritorna se i due punti sono uguali.
     *
     * @param a primo punto
     * @param b secondo punto
     *
     * @return true se a equivale a b, false altrimenti
     */
    bool operator()(const point &a, const point &b) const;
};

/**
 * @brief operatore di stampa su stream per un punto
 *
 * Stampa su stream un punto.
 * Il formato è "(x, y)".
 *
 * @param os stream di output
 * @param p point da stampare
 *
 * @return lo stream passato per permettere concatenazione di operazioni di stampa
 */
std::ostream &operator<<(std::ostream &os, const point &p);

/**
 * @brief operatore di lettura da stream per un puto
 *
 * Legge da stream un punto.
 * Il formato aspettato è "(x,y)".
 *
 * @param is stream di input
 * @param p point in cui inserire i dati
 *
 * @return lo stream passato per permettere concatenazione di operazioni di lettura.
 */
std::istream &operator>>(std::istream &is, point &p);

#endif