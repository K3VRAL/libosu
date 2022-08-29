// https://github.com/ppy/osu/blob/master/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
#ifndef OOS_SLIDER_H
#define OOS_SLIDER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "difficulty.h"
#include "timing_point.h"
#include "hit_object.h"

typedef enum SliderEventType {
    tick,
    legacy_last_tick,
    head,
    tail,
    repeat
} SliderEventType;
typedef struct SliderEventDescriptor {
    SliderEventType type;
    double time;
    int span_index;
    double span_start_time;
    double path_progress;
} SliderEventDescriptor;
SliderEventDescriptor *oos_slider_generate(double, double, double, double, double, int, double *);
SliderEventDescriptor *oos_slider_generateticks(int, double, double, bool, double, double, double);

typedef struct SliderPath {
    double distance;
} SliderPath;

typedef struct Slider {
    double start_time;
    double end_time;
    double duration;
    SliderPath path;
    double distance;
    double *legacy_last_tick_offset;
    double span_duration;
    double tick_distance_multiplier;
    double velocity;
    double tick_distance;
    double span_count; // TODO maybe remove?
} Slider;
Slider oos_slider_init(Difficulty, TimingPoint *, unsigned int, HitObject);
void oos_slider_createnestedhitobjects(Slider);

#endif