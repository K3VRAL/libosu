#include "file/beatmap/timing_point.h"

TimingPoint *ofb_timingpoint_addfromstring(char *string) {
    char *token = strtok(string, ",");
    TimingPoint *timing_point = NULL;
    if (token != NULL) {
        timing_point = malloc(sizeof(TimingPoint));
        timing_point->time = (int) strtol(token, NULL, 10);
        timing_point->beat_length = strtod(strtok(NULL, ","), NULL);
        timing_point->meter = (int) strtol(strtok(NULL, ","), NULL, 10);
        timing_point->sample_set = (int) strtol(strtok(NULL, ","), NULL, 10);
        timing_point->sample_index = (int) strtol(strtok(NULL, ","), NULL, 10);
        timing_point->volume = (int) strtol(strtok(NULL, ","), NULL, 10);
        timing_point->uninherited = (bool) strtol(strtok(NULL, ","), NULL, 10);
        timing_point->effects = (int) strtol(strtok(NULL, ","), NULL, 10);
    }
    return timing_point;
}

char *ofb_timingpoint_tostring(TimingPoint timing_point) {
    int time_size = ou_comparing_size(timing_point.time);
    int size_floor = ou_comparing_size((int) timing_point.beat_length);
    int trailing_zeros = ou_comparing_trailing(timing_point.beat_length, size_floor);
    int meter_size = (timing_point.meter == 0 ? 1 : (floor(log10(abs(timing_point.meter))) + 1 + (timing_point.meter < 0 ? 1 : 0)));
    int sampleset_size = (timing_point.sample_set == 0 ? 1 : (floor(log10(abs(timing_point.sample_set))) + 1 + (timing_point.sample_set < 0 ? 1 : 0)));
    int sampleindex_size = (timing_point.sample_index == 0 ? 1 : (floor(log10(abs(timing_point.sample_index))) + 1 + (timing_point.sample_index < 0 ? 1 : 0)));
    int volume_size = (timing_point.volume == 0 ? 1 : (floor(log10(abs(timing_point.volume))) + 1 + (timing_point.volume < 0 ? 1 : 0)));
    int uninherited_size = 1;
    int effects_size = (timing_point.effects == 0 ? 1 : (floor(log10(abs(timing_point.effects))) + 1 + (timing_point.effects < 0 ? 1 : 0)));
    int len = time_size + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + 1 + meter_size + 1 +sampleset_size + 1 + sampleindex_size + 1 + volume_size + 1 + uninherited_size + 1 + effects_size + (1 + 1);
    char *output = malloc(len * sizeof(char));
    snprintf(output, len, "%d,%.*f,%d,%d,%d,%d,%d,%d\n", timing_point.time, trailing_zeros, timing_point.beat_length, timing_point.meter, timing_point.sample_set, timing_point.sample_index, timing_point.volume, timing_point.uninherited, timing_point.effects);
    return output;
}