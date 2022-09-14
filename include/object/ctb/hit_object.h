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
    Droplet d;
    TinyDroplet td;
} CHO;

typedef enum CHOType {
    catchhitobject_fruit,
    catchhitobject_juicestream,
    catchhitobject_bananashower,
    catchhitobject_banana,
    catchhitobject_droplet,
    catchhitobject_tinydroplet
} CHOType;

typedef struct CatchHitObject {
    float start_time;
    float x;
    float x_offset;
    CHOType type;
    CHO cho;
} CatchHitObject;

extern const float ooc_hitobject_OBJECTRADIUS;

/*

    argsreturn
    args
*/
void ooc_hitobject_init(CatchHitObject *, float, float, float);

/*

    argsreturn
    args
*/
void ooc_hitobject_free(CatchHitObject);

/*

    argsreturn
    args
*/
void ooc_hitobject_freebulk(CatchHitObject *object, unsigned int num);

/*

    argsreturn
    args
*/
void ooc_hitobject_sort(CatchHitObject *object, unsigned int num);

#endif