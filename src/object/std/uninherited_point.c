#include "object/std.h"

void oos_uninheritedpoint_init(UninheritedTimingPoint *unintp, TimingPoint *timing_point, unsigned int num) {
	if (timing_point == NULL || num == 0) {
		return;
	}
	unintp->tp = NULL;
	unintp->num = 0;
	for (int i = 0; i < num; i++) {
		if ((timing_point + i)->uninherited) {
			unintp->tp = realloc(unintp->tp, (unintp->num + 1) * sizeof(*unintp->tp));
			*(unintp->tp + unintp->num) = *(timing_point + i);
			unintp->num++;
		}
	}
}

void oos_uninheritedpoint_free(UninheritedTimingPoint timing_point) {
	if (timing_point.tp != NULL) {
		oos_timingpoint_free(timing_point.tp);
	}
}