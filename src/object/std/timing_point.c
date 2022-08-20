#include "object/std/timing_point.h"

void oos_timingpoint_free(TimingPoint *tps) {
    if (tps != NULL) {
        free(tps);
        tps = NULL;
    }
}

void oos_timingpoint_sort(TimingPoint *tps, unsigned int num) {
    for (int i = 0; i < num - 1; i++) {
        for (int j = i + 1; j < num; j++) {
            if ((tps + i)->time > (tps + j)->time) {
                TimingPoint temp = *(tps + i);
                *(tps + i) = *(tps + j);
                *(tps + j) = temp;
            }
        }
    }
}