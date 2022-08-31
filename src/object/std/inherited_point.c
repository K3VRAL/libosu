#include "object/std.h"

InheritedTimingPoint *oos_inheritedpoint_init(TimingPoint *timing_point, unsigned int num) {
    if (timing_point == NULL || num == 0) {
        return NULL;
    }
    InheritedTimingPoint *intp = malloc(sizeof(InheritedTimingPoint));
    intp->tp = NULL;
    intp->num = 0;
    for (int i = 0; i < num; i++) {
        if (!(timing_point + i)->uninherited) {
            intp->tp = realloc(intp->tp, (intp->num + 1) * sizeof(TimingPoint));
            *(intp->tp + intp->num) = *(timing_point + i);
            intp->num++;
        }
    }
    return intp;
}

void oos_inheritedpoint_free(InheritedTimingPoint *timing_point) {
    if (timing_point != NULL) {
        if (timing_point->tp != NULL) {
            free(timing_point->tp);
        }
        free(timing_point);
    }
}