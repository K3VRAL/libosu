#ifndef OOC_JUICESTREAM_H
#define OOC_JUICESTREAM_H

#include "object/std.h"
#include "util/legacy_random.h"

typedef struct CatchHitObject CatchHitObject;

typedef struct Droplet {} Droplet;
typedef struct TinyDroplet {} TinyDroplet;

typedef struct JuiceStream {
    CatchHitObject *nested;
    unsigned int num_nested;
    Slider slider_data;
} JuiceStream;

/*

    argsreturn
    args
*/
void ooc_juicestream_init(CatchHitObject *, Difficulty, TimingPoint *, unsigned int, HitObject);

/*

    argsreturn
    args
*/
void ooc_juicestream_free(JuiceStream);

/*

    argsreturn
    args
*/
void ooc_juicestream_createnestedjuice(CatchHitObject *);

/*

    argsreturn
    args
*/
void ooc_juicestream_xoffset(CatchHitObject *, float **, double *, LegacyRandom *);

#endif