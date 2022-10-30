#ifndef OOC_BANANA_H
#define OOC_BANANA_H

#include <stdlib.h>

#include "object/std.h"

typedef struct Banana {} Banana;

typedef struct CatchHitObject CatchHitObject;

void ooc_banana_init(CatchHitObject *, float);

#endif