#ifndef OOC_FRUIT_H
#define OOC_FRUIT_H

#include "object/std.h"
#include "util/legacy_random.h"

typedef struct Fruit {
} Fruit;

// TODO maybe remove `HitObject` and replace with `Circle`
typedef struct CatchHitObject CatchHitObject; // Forward Declaration

extern const float ooc_fruit_OBJECTRADIUS;

/*

    argsreturn
    args
*/
void ooc_fruit_init(CatchHitObject *, HitObject);

/*

    argsreturn
    args
*/
void ooc_fruit_applyhardrockoffset(CatchHitObject *, float **, double *, LegacyRandom *);

/*

    argsreturn
    args
*/
void ooc_fruit_applyrandomoffset(float *, double, LegacyRandom *);

/*

    argsreturn
    args
*/
void ooc_fruit_applyoffset(float *, float);

#endif