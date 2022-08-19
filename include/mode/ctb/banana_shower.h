#ifndef OMC_BANANASHOWER_H
#define OMC_BANANASHOWER_H

#include "banana.h"
#include "playfield.h"
#include "util/legacy_random.h"
#include "file/beatmap/hit_objects.h"

typedef struct BananaShower {
    int start;
    int end;
    int duration;
    Banana *bananas;
    unsigned int num_banana;
} BananaShower;


BananaShower omc_bananashower_init(HitObject *);
void omc_bananashower_free(BananaShower *);
LegacyRandom omc_bananashower_xoffset(BananaShower *, LegacyRandom);
LegacyRandom omc_bananashower_xoffsetuntilindex(BananaShower *, LegacyRandom, int);

#endif