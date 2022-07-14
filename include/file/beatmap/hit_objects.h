#ifndef OFB_HITOBJECTS_H
#define OFB_HITOBJECTS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char curve_type;
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
    circle,
    slider,
    spinner
} HOType;

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#hit-objects
typedef struct HitObject {
    int x;
    int y;
    int time;
    int type;
    int hit_sound;
    HOType ho_type;
    HO ho;
    HOSample hit_sample;
} HitObject;

void ofb_hitobjects_free(HitObject **, unsigned int);
void ofb_hitobjects_add(HitObject **, unsigned int *, char *);
void ofb_hitobjects_sort(HitObject **, unsigned int);

#endif