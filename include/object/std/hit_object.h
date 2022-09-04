#ifndef OOS_HITOBJECTS_H
#define OOS_HITOBJECTS_H

#include <stdarg.h>
#include <stdlib.h>

#include "slider.h"

// TODO split every object up to their own files

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#hit-circles
typedef struct HOCircle {} HOCircle;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#sliders
typedef struct {
    int normal;
    int additional;
} HOSliderEdge;

typedef struct HOSliderCurve {
    int x;
    int y;
} HOSliderCurve;

typedef struct HOSlider {
    SliderType curve_type;
    HOSliderCurve *curves;
    unsigned int num_curve;
    int slides;
    double length;
    int *edge_sounds;
    unsigned int num_edge_sound;
    HOSliderEdge *edge_sets;
    unsigned int num_edge_set;
} HOSlider;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#spinners
typedef struct HOSpinner {
    int end_time;
} HOSpinner;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#hitsounds
typedef struct HOSample {
    int normal_set;
    int addition_set;
    int index;
    int volume;
    char *filename;
} HOSample;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#type
typedef union HO {
    HOCircle circle;
    HOSlider slider;
    HOSpinner spinner;
} HO;

typedef enum HOType {
    circle = 1,
    nc_circle = 5,
    slider = 2,
    nc_slider = 6,
    spinner = 8,
    nc_spinner = 12
} HOType;

typedef union HONested {
    Slider s;
} HONested;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#hit-objects
typedef struct HitObject {
    int x;
    int y;
    int time;
    HOType type;
    int hit_sound;
    HO ho;
    HOSample hit_sample;

    // For nesting
    HONested nested;
} HitObject;

extern const float oos_hitobject_BASESCORINGDISTANCE;

void oos_hitobject_free(HitObject *, unsigned int);
void oos_hitobject_sort(HitObject *, unsigned int);

#endif