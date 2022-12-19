#ifndef OOC_PROCESSOR_H
#define OOC_PROCESSOR_H

#include <stdbool.h>

#include "object/std.h"
#include "util/legacy_random.h"

#include "hit_object.h"

extern const int ooc_processor_RNGSEED;

void ooc_processor_applypositionoffset(CatchHitObject *, unsigned int, bool);
void ooc_processor_applypositionoffsetrng(CatchHitObject *, unsigned int, LegacyRandom *, bool, float **, double *);
void ooc_processor_applypositionoffsetrngstarttime(CatchHitObject *, unsigned int, unsigned int, LegacyRandom *, bool, float **, double *);
void ooc_processor_inithyperdash(Difficulty, CatchHitObject *, unsigned int);

#endif