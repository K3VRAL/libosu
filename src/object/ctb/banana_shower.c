#include "object/ctb.h"

CatchHitObject *ooc_bananashower_init(HitObject hit_object) {
    if (!(hit_object.type == spinner || hit_object.type == nc_spinner)) {
        return NULL;
    }
    CatchHitObject *object = ooc_hitobject_init(hit_object.time, hit_object.x, 0);
    object->type = banana_shower;
    object->cho.bs.end_time = hit_object.ho.spinner.end_time;
    object->cho.bs.duration = hit_object.ho.spinner.end_time - hit_object.time;
    object->cho.bs.bananas = NULL;
    object->cho.bs.num_banana = 0;
    return object;
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
        object->cho.bs.bananas = realloc(object->cho.bs.bananas, (object->cho.bs.num_banana + 1) * sizeof(CatchHitObject));
        (object->cho.bs.bananas + object->cho.bs.num_banana)->start_time = time;
        (object->cho.bs.bananas + object->cho.bs.num_banana)->type = banana;
        time += spacing;
        object->cho.bs.num_banana++;
    }
}

// TODO make every other object be able to free; and making `hit_object.c` allow to free all at once with one function
void ooc_bananashower_free(BananaShower *banana_shower) {
    if (banana_shower->bananas != NULL) {
        free(banana_shower->bananas);
    }
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