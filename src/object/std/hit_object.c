#include "object/std/hit_object.h"

void oos_hitobject_free(HitObject *hos) {
    if (hos != NULL) {
        if (hos->ho_type == slider) {
            if (hos->ho.slider.curves != NULL) {
                free(hos->ho.slider.curves);
                hos->ho.slider.curves = NULL;
            }
            if (hos->ho.slider.edge_sounds != NULL) {
                free(hos->ho.slider.edge_sounds);
                hos->ho.slider.edge_sounds = NULL;
            }
            if (hos->ho.slider.edge_sets != NULL) {
                free(hos->ho.slider.edge_sets);
                hos->ho.slider.edge_sets = NULL;
            }
        }
        if (hos->hit_sample.filename != NULL) {
            free(hos->hit_sample.filename);
            hos->hit_sample.filename = NULL;
        }
        free(hos);
        hos = NULL;
    }
}

void oos_hitobject_sort(HitObject *hos, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((hos + i)->time > (hos + j)->time) {
                HitObject temp = *(hos + i);
                *(hos + i) = *(hos + j);
                *(hos + j) = temp;
            }
        }
    }
}