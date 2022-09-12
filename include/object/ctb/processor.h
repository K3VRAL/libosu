#ifndef OOC_PROCESSOR_H
#define OOC_PROCESSOR_H

#include <stdbool.h>

#include "object/std.h"
#include "util/legacy_random.h"

#include "hit_object.h"

extern const int ooc_processor_RNGSEED;

/*

    argsreturn
    args
*/
void ooc_processor_applypositionoffset(CatchHitObject *, unsigned int, bool);

/*

    argsreturn
    args
*/
void ooc_processor_applypositionoffsetrng(CatchHitObject *, unsigned int, LegacyRandom *, bool);

/*

    argsreturn
    args
*/
void ooc_processor_inithyperdash(Difficulty, CatchHitObject *, unsigned int);

#endif