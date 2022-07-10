#ifndef TIMING_POINTS_H
#define TIMING_POINTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#structure
typedef struct TimingPoints {
    TimingPoint *timing_points;
    unsigned int num;
} TimingPoints;

TimingPoints osut_init();
void osut_free(TimingPoints *);
void osut_add_TP(TimingPoints *, char *);

#endif