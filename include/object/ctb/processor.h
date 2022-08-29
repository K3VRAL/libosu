#ifndef OOC_PROCESSOR_H
#define OOC_PROCESSOR_H

#include <stdbool.h>

#include "object/std.h"
#include "util/legacy_random.h"

#include "hit_object.h"

void ooc_processor_applypositionoffset(CatchHitObject **, unsigned int, bool);
void ooc_processor_applypositionoffsetrng(CatchHitObject **, unsigned int, LegacyRandom *, bool);
void ooc_processor_inithyperdash(Difficulty, CatchHitObject *, unsigned int);
extern const int ooc_processor_RNGSEED;

#endif