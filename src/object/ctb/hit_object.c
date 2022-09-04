#include "object/ctb.h"

const float ooc_hitobject_OBJECTRADIUS = 64;

void ooc_hitobject_init(CatchHitObject *object, float start_time, float x, float x_offset) {
    object = calloc(1, sizeof(*object));
    object->start_time = start_time;
    object->x = x;
    object->x_offset = x_offset;
}

void ooc_hitobject_free(CatchHitObject *object) {
    free(object);
}