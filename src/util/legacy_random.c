#include "util/legacy_random.h"

const double int_to_real = 1.0 / (INT_MAX + 1.0);
const unsigned int int_mask = 0x7FFFFFFF;
const unsigned int y_initial = 842502087;
const unsigned int z_initial = 3579807591;
const unsigned int w_initial = 273326509;

void ou_legacyrandom_init(LegacyRandom *lr, int seed) {
    lr = malloc(sizeof(*lr));
    lr->x = (unsigned int)seed;
    lr->y = y_initial;
    lr->z = z_initial;
    lr->w = w_initial;
    lr->bitIndex = 32;
}

unsigned int ou_legacyrandom_nextuint(LegacyRandom *lr_struct) {
    unsigned int t = lr_struct->x ^ (lr_struct->x << 11);
    lr_struct->x = lr_struct->y;
    lr_struct->y = lr_struct->z;
    lr_struct->z = lr_struct->w;
    lr_struct->w = lr_struct->w ^ (lr_struct->w >> 19) ^ t ^ (t >> 8);
    return lr_struct->w;
}

int ou_legacyrandom_next(LegacyRandom *lr_struct) {
    return (int)(int_mask & ou_legacyrandom_nextuint(lr_struct));
}

double ou_legacyrandom_nextdouble(LegacyRandom *lr_struct) {
    return int_to_real * ou_legacyrandom_next(lr_struct);
}

int ou_legacyrandom_nextupper(LegacyRandom *lr_struct, int upperBound) {
    return (int)(ou_legacyrandom_nextdouble(lr_struct) * upperBound);
}

int ou_legacyrandom_nextlowerupper(LegacyRandom *lr_struct, int lowerBound, int upperBound) {
    return (int)(lowerBound + ou_legacyrandom_nextdouble(lr_struct) * (upperBound - lowerBound));
}

int ou_legacyrandom_nextdoublelowerupper(LegacyRandom *lr_struct, double lowerBound, double upperBound) {
    return (int)(lowerBound + ou_legacyrandom_nextdouble(lr_struct) * (upperBound - lowerBound));
}

bool ou_legacyrandom_nextbool(LegacyRandom *lr_struct) {
    if (lr_struct->bitIndex == 32) {
        lr_struct->bitBuffer = ou_legacyrandom_nextuint(lr_struct);
        lr_struct->bitIndex = 1;
        return (lr_struct->bitBuffer & 1) == 1;
    }
    lr_struct->bitIndex++;
    lr_struct->bitBuffer >>= 1;
    return (lr_struct->bitBuffer & 1) == 1;
}