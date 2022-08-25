#include "object/std/hit_object.h"

void oos_hitobject_free(HitObject *ho, unsigned int num) {
    if (ho != NULL) {
        for (int i = 0; i < num; i++) {
            if ((ho + i)->type == slider) {
                if ((ho + i)->ho.slider.curves != NULL) {
                    free((ho + i)->ho.slider.curves);
                }
                if ((ho + i)->ho.slider.edge_sounds != NULL) {
                    free((ho + i)->ho.slider.edge_sounds);
                }
                if ((ho + i)->ho.slider.edge_sets != NULL) {
                    free((ho + i)->ho.slider.edge_sets);
                }
            }
            if ((ho + i)->hit_sample.filename != NULL) {
                free((ho + i)->hit_sample.filename);
            }
        }
        free(ho);
    }
}

void oos_hitobject_sort(HitObject *ho, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((ho + i)->time > (ho + j)->time) {
                HitObject temp = *(ho + i);
                *(ho + i) = *(ho + j);
                *(ho + j) = temp;
            }
        }
    }
}