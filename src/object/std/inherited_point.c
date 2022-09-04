#include "object/std.h"

void oos_inheritedpoint_init(InheritedTimingPoint *intp, TimingPoint *timing_point, unsigned int num) {
    if (timing_point == NULL || num == 0) {
        return;
    }
    intp = malloc(sizeof(*intp));
    intp->tp = NULL;
    intp->num = 0;
    for (int i = 0; i < num; i++) {
        if (!(timing_point + i)->uninherited) {
            intp->tp = realloc(intp->tp, (intp->num + 1) * sizeof(*intp->tp));
            *(intp->tp + intp->num) = *(timing_point + i);
            intp->num++;
        }
    }
}

void oos_inheritedpoint_free(InheritedTimingPoint *timing_point) {
    if (timing_point == NULL) {
        return;
    }
    if (timing_point->tp != NULL) {
        free(timing_point->tp);
    }
    free(timing_point);
}