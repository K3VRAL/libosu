// https://github.com/ppy/osu/blob/master/osu.Game/Rulesets/Objects/SliderEventGenerator.cs
#ifndef OOS_SLIDER_H
#define OOS_SLIDER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "timing_point.h"
#include "inherited_point.h"
#include "uninherited_point.h"
#include "difficulty.h"

typedef enum SliderType {
    slidertype_catmull = 'C',
    slidertype_bezier = 'B',
    slidertype_linear = 'L',
    slidertype_perfectcurve = 'P'
} SliderType;
typedef struct SliderVector2 { // TODO make free
    SliderType *type;
    float x;
    float y;
} SliderVector2;

typedef enum SliderEventType {
    sliderevent_tick,
    sliderevent_legacylasttick,
    sliderevent_head,
    sliderevent_tail,
    sliderevent_repeat
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

typedef struct HitObject HitObject;
typedef struct HOSlider HOSlider;
typedef struct SliderPath {
    double distance;
} SliderPath;
typedef struct Slider { // Some duplicated/relational variables exists here; maybe I should refactor, and for each variable called the original operation can be done
    double start_time;
    double end_time;
    double duration;
    SliderPath path;
    double *legacy_last_tick_offset;
    double span_duration;
    double tick_distance_multiplier;
    double velocity;
    double tick_distance;
    double span_count;
    SliderVector2 start_position;
    HOSlider *ho_data;

    SliderVector2 *control_point;
    unsigned int controlpoint_len;
    SliderVector2 *calculate_path;
    unsigned int calculatepath_len;
    double *cumulative_length;
    unsigned int cumulativelength_len;
    
    HitObject *nested;
    unsigned int num_nested;
} Slider;
void oos_slider_init(Slider **, Difficulty, InheritedTimingPoint *, UninheritedTimingPoint *, HitObject *);
void oos_slider_initwouninandinherited(Slider **, Difficulty, TimingPoint *, unsigned int, HitObject *);
void oos_slider_free(Slider *);
void oos_slider_catmullfindpoint(SliderVector2 **, SliderVector2 *, SliderVector2 *, SliderVector2 *, SliderVector2 *, float);
void oos_slider_catmullapproximate(SliderVector2 **, unsigned int *, SliderVector2 *, unsigned int *);
void oos_slider_approximatecirculararc(SliderVector2 **, unsigned int *, SliderVector2 *, unsigned int *);
void oos_slider_approximatebezier(SliderVector2 **, unsigned int *, SliderVector2 *, unsigned int *);
void oos_slider_calculatesubpath(SliderVector2 **, unsigned int *, SliderVector2 *, unsigned int *, SliderType);
void oos_slider_positionat(SliderVector2 *, double, Slider *);
void oos_slider_createnestedhitobjects(Slider *);

#endif