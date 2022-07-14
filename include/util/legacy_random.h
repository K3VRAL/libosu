#ifndef OU_LEGACY_RANDOM_H
#define OU_LEGACY_RANDOM_H

#include <stdbool.h>
#include <limits.h>

// https://github.com/ppy/osu/blob/200f0d9f5540e0542a29d6bb0dc4da77d6c16f52/osu.Game/Utils/LegacyRandom.cs
typedef struct LegacyRandom {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
    unsigned int bitBuffer;
    int bitIndex;
} LegacyRandom;

LegacyRandom legacyrandom_init(int);
unsigned int legacyrandom_nextuint(LegacyRandom *);
int legacyrandom_next(LegacyRandom *);
double legacyrandom_nextdouble(LegacyRandom *);
int legacyrandom_nextupper(LegacyRandom *, int);
int legacyrandom_nextlowerupper(LegacyRandom *, int, int);
int legacyrandom_nextdoublelowerupper(LegacyRandom *, double, double);
bool legacyrandom_nextbool(LegacyRandom *);

#endif