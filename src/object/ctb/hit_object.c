#include "object/ctb.h"

const float ooc_hitobject_OBJECTRADIUS = 64;

CatchHitObject *ooc_hitobject_init(float start_time, float x, float x_offset) {
    CatchHitObject *object = calloc(1, sizeof(CatchHitObject));
    object->start_time = start_time;
    object->x = x;
    object->x_offset = x_offset;
    return object;
}