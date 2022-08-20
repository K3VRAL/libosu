#ifndef OOC_BANANASHOWER_H
#define OOC_BANANASHOWER_H

#include "banana.h"
#include "playfield.h"
#include "util/legacy_random.h"
#include "file/beatmap/hit_object.h"

typedef struct BananaShower {
    int start;
    int end;
    int duration;
    Banana *bananas;
    unsigned int num_banana;
} BananaShower;


BananaShower ooc_bananashower_add(HitObject *);
void ooc_bananashower_free(BananaShower *);
LegacyRandom ooc_bananashower_xoffset(BananaShower *, LegacyRandom);
LegacyRandom ooc_bananashower_xoffsetuntilindex(BananaShower *, LegacyRandom, int);

#endif