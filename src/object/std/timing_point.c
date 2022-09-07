#include "object/std.h"

void oos_timingpoint_free(TimingPoint *tp) {
    if (tp == NULL) {
        return;
    }
    free(tp);
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

void oos_timingpoint_attime(TimingPoint **timing_point, int time, TimingPoint *tp, unsigned int num) {
    if (tp == NULL || num == 0 || time < (tp + 0)->time) {
        const double default_beat_length = 60000.0 / 60.0;
        TimingPoint default_timing_point = { // Does this need expanding on?
            .beat_length = default_beat_length
        };
        *timing_point = malloc(sizeof(**timing_point));
        **timing_point = num > 0 ? *(tp + 0) : default_timing_point;
        return;
    }
    if (time >= (tp + num - 1)->time) {
        *timing_point = malloc(sizeof(**timing_point));
        **timing_point = *(tp + num - 1);
        return;
    }

    int l = 0;
    int r = num - 2;
    while (l <= r) {
        int pivot = l + ((r - l) >> 1);
        if ((tp + pivot)->time < time) {
            l = pivot + 1;
        } else if ((tp + pivot)->time > time) {
            r = pivot - 1;
        } else {
            *timing_point = malloc(sizeof(**timing_point));
            **timing_point = *(tp + pivot);
            return;
        }
    }
    *timing_point = malloc(sizeof(**timing_point));
    **timing_point = *(tp + l - 1);
    return;
}