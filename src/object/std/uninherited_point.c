#include "object/std.h"

UninheritedTimingPoint *oos_uninheritedpoint_init(TimingPoint *timing_point, unsigned int num) {
    if (timing_point == NULL || num == 0) {
        return NULL;
    }
    UninheritedTimingPoint *unintp = malloc(sizeof(InheritedTimingPoint));
    unintp->tp = NULL;
    unintp->num = 0;
    for (int i = 0; i < num; i++) {
        if ((timing_point + i)->uninherited) {
            unintp->tp = realloc(unintp->tp, (unintp->num + 1) * sizeof(TimingPoint));
            *(unintp->tp + unintp->num) = *(timing_point + i);
            unintp->num++;
        }
    }
    return unintp;
}

void oos_uninheritedpoint_free(UninheritedTimingPoint *timing_point) {
    if (timing_point != NULL) {
        if (timing_point->tp != NULL) {
            free(timing_point->tp);
        }
        free(timing_point);
    }
}