#ifndef FAST_RANDOM_H
#define FAST_RANDOM_H

#include <stdbool.h>
#include <limits.h>

// https://github.com/ppy/osu/blob/200f0d9f5540e0542a29d6bb0dc4da77d6c16f52/osu.Game/Utils/LegacyRandom.cs
typedef struct FastRandom {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
    unsigned int bitBuffer;
    int bitIndex;
} FastRandom;

FastRandom fastrandom_FastRandom(int);
unsigned int fastrandom_NextUInt(FastRandom *);
int fastrandom_Next(FastRandom *);
double fastrandom_NextDouble(FastRandom *);
int fastrandom_NextUpper(FastRandom *, int);
int fastrandom_NextLowerUpper(FastRandom *, int, int);
int fastrandom_NextDoubleLowerUpper(FastRandom *, double, double);
bool fastrandom_NextBool(FastRandom *);

#endif