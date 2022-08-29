#ifndef OOC_BANANA_H
#define OOC_BANANA_H

#include <stdlib.h>

#include "object/std.h"

typedef struct Banana {
} Banana;

typedef struct CatchHitObject CatchHitObject; // Forward Declaration
CatchHitObject *ooc_banana_init(float);

#endif