#ifndef POINT_H
#define POINT_H

#include <iostream>

struct point
{
    int x;
    int y;
};

struct ArePointEqual
{
    bool operator()(const point &a, const point &b) const;
};

std::ostream &operator<<(std::ostream &os, const point &p);

std::istream &operator>>(std::istream &is, point &p);

#endif