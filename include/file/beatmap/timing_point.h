#ifndef OFB_TIMINGPOINT_H
#define OFB_TIMINGPOINT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/timing_point.h"
#include "unrelated/comparing.h"

TimingPoint ofb_timingpoint_addfromstring(char *);
char *ofb_timingpoint_tostring(TimingPoint);

#endif