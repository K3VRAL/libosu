#ifndef OU_LEGACY_RANDOM_H
#define OU_LEGACY_RANDOM_H

#include <stdlib.h>
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

/*

    argsreturn
    args
*/
void ou_legacyrandom_init(LegacyRandom *, int);

/*

    argsreturn
    args
*/
unsigned int ou_legacyrandom_nextuint(LegacyRandom *);

/*

    argsreturn
    args
*/
int ou_legacyrandom_next(LegacyRandom *);

/*

    argsreturn
    args
*/
double ou_legacyrandom_nextdouble(LegacyRandom *);

/*

    argsreturn
    args
*/
int ou_legacyrandom_nextupper(LegacyRandom *, int);

/*

    argsreturn
    args
*/
int ou_legacyrandom_nextlowerupper(LegacyRandom *, int, int);

/*

    argsreturn
    args
*/
int ou_legacyrandom_nextdoublelowerupper(LegacyRandom *, double, double);

/*

    argsreturn
    args
*/
bool ou_legacyrandom_nextbool(LegacyRandom *);

#endif