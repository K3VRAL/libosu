#include "object/std.h"

const float oos_hitobject_BASESCORINGDISTANCE = 100;

void oos_hitobject_free(HitObject *hit_object) {
    if (hit_object == NULL) {
        return;
    }
    if (hit_object->type == slider || hit_object->type == nc_slider) {
        if (hit_object->ho.slider.curves != NULL) {
            free(hit_object->ho.slider.curves);
        }
        if (hit_object->ho.slider.edge_sounds != NULL) {
            free(hit_object->ho.slider.edge_sounds);
        }
        if (hit_object->ho.slider.edge_sets != NULL) {
            free(hit_object->ho.slider.edge_sets);
        }
    }
    if (hit_object->hit_sample.filename != NULL) {
        free(hit_object->hit_sample.filename);
    }
    free(hit_object);
}

void oos_hitobject_sort(HitObject *hit_object, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((hit_object + i)->time > (hit_object + j)->time) {
                HitObject temp = *(hit_object + i);
                *(hit_object + i) = *(hit_object + j);
                *(hit_object + j) = temp;
            }
        }
    }
}