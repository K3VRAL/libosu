#ifndef OOC_BANANASHOWER_H
#define OOC_BANANASHOWER_H

#include "object/std.h"
#include "util/legacy_random.h"

#include "banana.h"

typedef struct BananaShower {
    int start;
    int end;
    int duration;
    Banana *bananas;
    unsigned int num_banana;
} BananaShower;

BananaShower ooc_bananashower_init(HitObject);
// TODO replace all args with `CatchHitObject`
void ooc_bananashower_getnestedbananas(BananaShower *);
void ooc_bananashower_free(BananaShower *);
LegacyRandom ooc_bananashower_xoffset(BananaShower *, LegacyRandom);
LegacyRandom ooc_bananashower_xoffsetuntilindex(BananaShower *, LegacyRandom, int);

#endif