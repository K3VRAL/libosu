#ifndef OMS_INHERITEDPOINT_H
#define OMS_INHERITEDPOINT_H

#include <stdlib.h>

#include "timing_point.h"

typedef struct InheritedTimingPoint {
    TimingPoint *tp;
    unsigned int num;
} InheritedTimingPoint;

void oos_inheritedpoint_init(InheritedTimingPoint *, TimingPoint *, unsigned int);
void oos_inheritedpoint_free(InheritedTimingPoint *);

#endif