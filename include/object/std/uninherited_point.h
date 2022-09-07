#ifndef OMS_UNINHERITEDPOINT_H
#define OMS_UNINHERITEDPOINT_H

#include <stdlib.h>

#include "timing_point.h"

typedef struct UninheritedTimingPoint {
    TimingPoint *tp;
    unsigned int num;
} UninheritedTimingPoint;

void oos_uninheritedpoint_init(UninheritedTimingPoint **, TimingPoint *, unsigned int);
void oos_uninheritedpoint_free(UninheritedTimingPoint *);

#endif