#include "util/legacy_random.h"

const double int_to_real = 1.0 / (INT_MAX + 1.0);
const unsigned int int_mask = 0x7FFFFFFF;
const unsigned int y_initial = 842502087;
const unsigned int z_initial = 3579807591;
const unsigned int w_initial = 273326509;

void ou_legacyrandom_init(LegacyRandom *legacy_random, int seed) {
	legacy_random->x = (unsigned int) seed;
	legacy_random->y = y_initial;
	legacy_random->z = z_initial;
	legacy_random->w = w_initial;
	legacy_random->bitIndex = 32;
}

unsigned int ou_legacyrandom_nextuint(LegacyRandom *legacy_random) {
	unsigned int t = legacy_random->x ^ (legacy_random->x << 11);
	legacy_random->x = legacy_random->y;
	legacy_random->y = legacy_random->z;
	legacy_random->z = legacy_random->w;
	legacy_random->w = legacy_random->w ^ (legacy_random->w >> 19) ^ t ^ (t >> 8);
	return legacy_random->w;
}

int ou_legacyrandom_next(LegacyRandom *legacy_random) {
	return (int)(int_mask & ou_legacyrandom_nextuint(legacy_random));
}

double ou_legacyrandom_nextdouble(LegacyRandom *legacy_random) {
	return int_to_real * ou_legacyrandom_next(legacy_random);
}

int ou_legacyrandom_nextupper(LegacyRandom *legacy_random, int upperBound) {
	return (int)(ou_legacyrandom_nextdouble(legacy_random) * upperBound);
}

int ou_legacyrandom_nextlowerupper(LegacyRandom *legacy_random, int lowerBound, int upperBound) {
	return (int)(lowerBound + ou_legacyrandom_nextdouble(legacy_random) * (upperBound - lowerBound));
}

int ou_legacyrandom_nextdoublelowerupper(LegacyRandom *legacy_random, double lowerBound, double upperBound) {
	return (int)(lowerBound + ou_legacyrandom_nextdouble(legacy_random) * (upperBound - lowerBound));
}

bool ou_legacyrandom_nextbool(LegacyRandom *legacy_random) {
	if (legacy_random->bitIndex == 32) {
		legacy_random->bitBuffer = ou_legacyrandom_nextuint(legacy_random);
		legacy_random->bitIndex = 1;
		return (legacy_random->bitBuffer & 1) == 1;
	}
	legacy_random->bitIndex++;
	legacy_random->bitBuffer >>= 1;
	return (legacy_random->bitBuffer & 1) == 1;
}