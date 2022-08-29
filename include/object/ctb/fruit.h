#ifndef OOC_FRUIT_H
#define OOC_FRUIT_H

#include "util/legacy_random.h"

typedef struct Fruit {
} Fruit;

typedef struct CatchHitObject CatchHitObject; // Forward Declaration
void ooc_fruit_applyhardrockoffset(CatchHitObject *, float *, double *, LegacyRandom *);
void ooc_fruit_applyrandomoffset(float *, double, LegacyRandom *);
void ooc_fruit_applyoffset(float *, float);
extern const float ooc_fruit_OBJECTRADIUS;

#endif