#ifndef OOC_HITOBJECT_H
#define OOC_HITOBJECT_H

#include <stdlib.h>

#include "banana_shower.h"
#include "fruit.h"
#include "juice_stream.h"

typedef union CHO {
    Fruit f;
    JuiceStream js;
    BananaShower bs;
    Banana b;
} CHO;

typedef enum CHOType {
    fruit,
    juice_stream,
    banana_shower,
    banana
} CHOType;

typedef struct CatchHitObject {
    float start_time;
    float x;
    float x_offset;
    CHOType type;
    CHO cho;
} CatchHitObject;

CatchHitObject *ooc_hitobject_init(float, float, float);
extern const float ooc_hitobject_OBJECTRADIUS;

#endif