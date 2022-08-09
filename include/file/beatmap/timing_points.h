#ifndef OFB_TIMINGPOINTS_H
#define OFB_TIMINGPOINTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#timing-points
typedef struct TimingPoint {
    int time;
    double beat_length;
    int meter;
    int sample_set;
    int sample_index;
    int volume;
    bool uninherited;
    int effects;
} TimingPoint;

void ofb_timingpoints_add(TimingPoint **, unsigned int *, TimingPoint);
void ofb_timingpoints_add_string(TimingPoint **, unsigned int *, char *);
void ofb_timingpoints_free(TimingPoint **);
void ofb_timingpoints_tofile(TimingPoint *, unsigned int, FILE *);

#endif