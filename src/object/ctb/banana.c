#include "object/ctb.h"

// TODO implement this in `banana_shower.c`
CatchHitObject *ooc_banana_init(float start_time) {
    CatchHitObject *object = ooc_hitobject_init(start_time, 0, 0);
    object->type = banana;
    return object;
}