#include "object/std.h"

const float oos_hitobject_BASESCORINGDISTANCE = 100;

void oos_hitobject_free(HitObject hit_object) {
    if (hit_object.type == slider || hit_object.type == nc_slider) {
        oos_hitobject_freeslider(hit_object.ho.slider);
    }
    if (hit_object.hit_sample.filename != NULL) {
        free(hit_object.hit_sample.filename);
    }
}

void oos_hitobject_freebulk(HitObject *hit_object, unsigned int num) {
    if (hit_object == NULL) {
        return;
    }
    for (int i = 0; i < num; i++) {
        oos_hitobject_free(*(hit_object + i));
    }
    free(hit_object);
}

void oos_hitobject_freeslider(HOSlider slider) {
    if (slider.curves != NULL) {
        free(slider.curves);
    }
    if (slider.edge_sounds != NULL) {
        free(slider.edge_sounds);
    }
    if (slider.edge_sets != NULL) {
        free(slider.edge_sets);
    }
}

void oos_hitobject_sort(HitObject *hit_object, unsigned int num) {
    if (hit_object == NULL || num == 0) {
        return;
    }
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num - i - 1; j++) {
            if ((hit_object + i)->time > (hit_object + j)->time) {
                HitObject temp = *(hit_object + i);
                *(hit_object + i) = *(hit_object + j);
                *(hit_object + j) = temp;
            }
        }
    }
}