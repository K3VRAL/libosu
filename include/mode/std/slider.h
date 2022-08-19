// https://github.com/ppy/osu/blob/master/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
#ifndef OMS_SLIDER_H
#define OMS_SLIDER_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef enum SliderEventType {
    Tick,
    LegacyLastTick,
    Head,
    Tail,
    Repeat
} SliderEventType;

typedef struct SliderEventDescriptor {
    SliderEventType type;
    double time;
    int span_index;
    double span_start_time;
    double path_progress;
} SliderEventDescriptor;

typedef struct SliderEventDescriptors {
    SliderEventDescriptor *sed;
    unsigned int num;
} SliderEventDescriptors;

SliderEventDescriptor *oms_slider_generate(double, double, double, double, double, int, double *);
SliderEventDescriptor *oms_slider_generateticks(int, double, double, bool, double, double, double);

#endif