#include "object/ctb.h"

void ooc_bananashower_init(CatchHitObject **object, HitObject *hit_object) {
    if (!(hit_object->type == spinner || hit_object->type == nc_spinner)) {
        return;
    }

    ooc_hitobject_init(object, hit_object->time, hit_object->x, 0);
    (*object)->type = banana_shower;
    (*object)->cho.bs.end_time = hit_object->ho.spinner.end_time;
    (*object)->cho.bs.duration = hit_object->ho.spinner.end_time - hit_object->time;
    (*object)->cho.bs.bananas = NULL;
    (*object)->cho.bs.num_banana = 0;
}

void ooc_bananashower_createnestedbananas(CatchHitObject *object) {
    double spacing = object->cho.bs.duration;
    while (spacing > 100) {
        spacing /= 2;
    }
    if (spacing <= 0) {
        return;
    }
    double time = object->start_time;
    while (time <= object->cho.bs.end_time) {
        object->cho.bs.bananas = realloc(object->cho.bs.bananas, (object->cho.bs.num_banana + 1) * sizeof(*object->cho.bs.bananas));
        (object->cho.bs.bananas + object->cho.bs.num_banana)->start_time = time;
        (object->cho.bs.bananas + object->cho.bs.num_banana)->type = banana;
        time += spacing;
        object->cho.bs.num_banana++;
    }
}

void ooc_bananashower_free(BananaShower *banana_shower) {
    if (banana_shower->bananas != NULL) {
        free(banana_shower->bananas);
    }
    free(banana_shower);
}

void ooc_bananashower_xoffset(CatchHitObject *object, LegacyRandom *lr) {
    ooc_bananashower_xoffsetuntilindex(object, lr, object->cho.bs.num_banana);
}

void ooc_bananashower_xoffsetuntilindex(CatchHitObject *object, LegacyRandom *lr, int index) {
    for (int i = 0; i < index; i++) {
        (object->cho.bs.bananas + i)->x_offset = (float) (ou_legacyrandom_nextdouble(lr) * ooc_playfield_WIDTH);
        ou_legacyrandom_next(lr);
        ou_legacyrandom_next(lr);
        ou_legacyrandom_next(lr);
    }
}