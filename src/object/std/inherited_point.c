#include "object/std.h"

void oos_inheritedpoint_init(InheritedTimingPoint *intp, TimingPoint *timing_point, unsigned int num) {
	if (timing_point == NULL || num == 0) {
		return;
	}
	intp->tp = NULL;
	intp->num = 0;
	for (int i = 0; i < num; i++) {
		if (!(timing_point + i)->uninherited) {
			intp->tp = realloc(intp->tp, (intp->num + 1) * sizeof(*intp->tp));
			*(intp->tp + intp->num) = *(timing_point + i);
			intp->num++;
		}
	}
	if (intp->tp == NULL || intp->num == 0) {
		intp->tp = realloc(intp->tp, (intp->num + 1) * sizeof(*intp->tp));
		(intp->tp + 0)->time = 0;
		(intp->tp + 0)->beat_length = -100;
		(intp->tp + 0)->meter = 4;
		(intp->tp + 0)->sample_set = 1;
		(intp->tp + 0)->sample_index = 0;
		(intp->tp + 0)->volume = 100;
		(intp->tp + 0)->uninherited = 0;
		(intp->tp + 0)->effects = 0;
		intp->num++;
	}
}

void oos_inheritedpoint_free(InheritedTimingPoint intp) {
	if (intp.tp != NULL) {
		oos_timingpoint_free(intp.tp);
	}
}