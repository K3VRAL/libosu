#ifndef OU_STATELESS_RNG_H
#define OU_STATELESS_RNG_H

unsigned long long int ou_statelessrng_mix(unsigned long long int);
unsigned long long int ou_statelessrng_nextulong(int, int);
float ou_statelessrng_nextsingle(int, int);

#endif
