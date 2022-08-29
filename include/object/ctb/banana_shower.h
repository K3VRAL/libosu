#ifndef OOC_BANANASHOWER_H
#define OOC_BANANASHOWER_H

#include "object/std.h"
#include "util/legacy_random.h"

#include "banana.h"

typedef struct CatchHitObject CatchHitObject; // Forward Declaration
typedef struct BananaShower {
    int end_time;
    int duration;
    CatchHitObject *bananas;
    unsigned int num_banana;
} BananaShower;

// TODO maybe remove `HitObject` and replace with `Spinner`
CatchHitObject *ooc_bananashower_init(HitObject);
void ooc_bananashower_createnestedbananas(CatchHitObject *);
void ooc_bananashower_free(BananaShower *);
void ooc_bananashower_xoffset(CatchHitObject *, LegacyRandom *);
void ooc_bananashower_xoffsetuntilindex(CatchHitObject *, LegacyRandom *, int);

#endif