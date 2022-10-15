#ifndef OFB_TIMINGPOINT_H
#define OFB_TIMINGPOINT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "object/std/timing_point.h"
#include "unrelated/comparing.h"

void ofb_timingpoint_addfromstring(TimingPoint *, char *);
void ofb_timingpoint_tostring(char *, TimingPoint);

#endif