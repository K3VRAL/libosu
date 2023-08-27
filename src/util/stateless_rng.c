#include "util/stateless_rng.h"

unsigned long long int ou_statelessrng_mix(unsigned long long int x) {
	x ^= x >> 33;
	x *= 0xff51afd7ed558ccd;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53;
	x ^= x >> 33;
	return x;
}

unsigned long long int ou_statelessrng_nextulong(int seed, int series) {
	unsigned long long int combined = ((unsigned long long int) (unsigned int) series << 32) | (unsigned int) seed;
	return ou_statelessrng_mix(combined ^ 0x12345678);
}

float ou_statelessrng_nextsingle(int seed, int series) {
	return (float) (ou_statelessrng_nextulong(seed, series) & ((1 << 24) - 1)) / (1 << 24);
}
