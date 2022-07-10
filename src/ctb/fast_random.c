#include "../../include/ctb/fast_random.h"

const double int_to_real = 1.0 / (INT_MAX + 1.0);
const unsigned int int_mask = 0x7FFFFFFF;
const unsigned int y_initial = 842502087;
const unsigned int z_initial = 3579807591;
const unsigned int w_initial = 273326509;

FastRandom fastrandom_FastRandom(int seed) {
    FastRandom frStruct;
    frStruct.x = (unsigned int)seed;
    frStruct.y = y_initial;
    frStruct.z = z_initial;
    frStruct.w = w_initial;
    frStruct.bitIndex = 32;
    return frStruct;
}

unsigned int fastrandom_NextUInt(FastRandom *frStruct) {
    unsigned int t = frStruct->x ^ (frStruct->x << 11);
    frStruct->x = frStruct->y;
    frStruct->y = frStruct->z;
    frStruct->z = frStruct->w;
    frStruct->w = frStruct->w ^ (frStruct->w >> 19) ^ t ^ (t >> 8);
    return frStruct->w;
}

int fastrandom_Next(FastRandom *frStruct) {
    return (int)(int_mask & fastrandom_NextUInt(frStruct));
}

double fastrandom_NextDouble(FastRandom *frStruct) {
    return int_to_real * fastrandom_Next(frStruct);
}

int fastrandom_NextUpper(FastRandom *frStruct, int upperBound) {
    return (int)(fastrandom_NextDouble(frStruct) * upperBound);
}

int fastrandom_NextLowerUpper(FastRandom *frStruct, int lowerBound, int upperBound) {
    return (int)(lowerBound + fastrandom_NextDouble(frStruct) * (upperBound - lowerBound));
}

int fastrandom_NextDoubleLowerUpper(FastRandom *frStruct, double lowerBound, double upperBound) {
    return (int)(lowerBound + fastrandom_NextDouble(frStruct) * (upperBound - lowerBound));
}

bool fastrandom_NextBool(FastRandom *frStruct) {
    if (frStruct->bitIndex == 32) {
        frStruct->bitBuffer = fastrandom_NextUInt(frStruct);
        frStruct->bitIndex = 1;
        return (frStruct->bitBuffer & 1) == 1;
    }
    frStruct->bitIndex++;
    frStruct->bitBuffer >>= 1;
    return (frStruct->bitBuffer & 1) == 1;
}