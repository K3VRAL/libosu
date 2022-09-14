#include "object/ctb.h"

const float ooc_hitobject_OBJECTRADIUS = 64;

void ooc_hitobject_init(CatchHitObject *object, float start_time, float x, float x_offset) {
    object->start_time = start_time;
    object->x = x;
    object->x_offset = x_offset;
}

void ooc_hitobject_free(CatchHitObject object) {
    switch (object.type) {
        case catchhitobject_fruit:
            break;

        case catchhitobject_juicestream:
            ooc_juicestream_free(object.cho.js);
            break;

        case catchhitobject_bananashower:
            break;

        case catchhitobject_banana:
            break;

        case catchhitobject_droplet:
            break;

        case catchhitobject_tinydroplet:
            break;
    }
}

void ooc_hitobject_freebulk(CatchHitObject *object, unsigned int num) {
    for (int i = 0; i < num; i++) {
        ooc_hitobject_free(*(object + i));
    }
    free(object);
}

void ooc_hitobject_sort(CatchHitObject *object, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((object + i)->start_time > (object + j)->start_time) {
                CatchHitObject temp = *(object + i);
                *(object + i) = *(object + j);
                *(object + j) = temp;
            }
        }
    }
}