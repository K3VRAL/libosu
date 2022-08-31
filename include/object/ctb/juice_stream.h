#ifndef OOC_JUICESTREAM_H
#define OOC_JUICESTREAM_H

#include "object/std.h"

typedef struct CatchHitObject CatchHitObject;

typedef struct JuiceStreamNested {
    CatchHitObject *object;
    SliderEventType type;
} JuiceStreamNested;

typedef struct JuiceStream {
    JuiceStreamNested *nested;
    unsigned int num_nested;
} JuiceStream;

// TODO maybe remove `HitObject` and replace with `Slider`
JuiceStream *ooc_juicestream_nestedhitobjects(HitObject *, Difficulty, TimingPoint);
void ooc_juicestream_free(JuiceStream *);

#endif