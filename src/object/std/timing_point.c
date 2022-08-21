#include "object/std/timing_point.h"

void oos_timingpoint_free(TimingPoint *tp) {
    if (tp != NULL) {
        free(tp);
    }
}

void oos_timingpoint_sort(TimingPoint *tp, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((tp + i)->time > (tp + j)->time) {
                TimingPoint temp = *(tp + i);
                *(tp + i) = *(tp + j);
                *(tp + j) = temp;
            }
        }
    }
}