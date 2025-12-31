/**
 * @file point.cpp
 *
 * @brief file di implementazione delle funzioni per la struct point
 *
 * File di implementazione delle funzioni per la struct point.
 */
#include <iostream>
#include "point.h"

bool ArePointEqual::operator()(const point &a, const point &b) const
{
    return a.x == b.x && a.y == b.y;
}

std::ostream &operator<<(std::ostream &os, const point &p)
{
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

std::istream &operator>>(std::istream &is, point &p)
{
    char c;
    is >> c >> p.x >> c >> p.y >> c;
    return is;
}