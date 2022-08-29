#ifndef OMS_TIMINGPOINTS_H
#define OMS_TIMINGPOINTS_H

#include <stdlib.h>
#include <stdbool.h>

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

void oos_timingpoint_free(TimingPoint *);
void oos_timingpoint_sort(TimingPoint *, unsigned int);
TimingPoint oos_timingpoint_attime(int, TimingPoint *, unsigned int);

#endif