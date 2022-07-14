#include "file/beatmap/timing_points.h"

void ofb_timingpoints_add(TimingPoint **tp, unsigned int *num, char *string) {
    char *token = strtok(string, ",");
    if (token != NULL) {
        if (*tp == NULL) {
            // If there is no space; make space
            *tp = calloc(1, sizeof(TimingPoint));
        } else {
            // If there is space; make more space
            *tp = realloc(*tp, (*num + 1) * sizeof(TimingPoint));
        }
        // Set data
        (*tp + *num)->time = (int) strtol(token, NULL, 10);
        (*tp + *num)->beat_length = strtod(strtok(NULL, ","), NULL);
        (*tp + *num)->meter = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*tp + *num)->sample_set = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*tp + *num)->sample_index = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*tp + *num)->volume = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*tp + *num)->uninherited = (bool) strtol(strtok(NULL, ","), NULL, 10);
        (*tp + *num)->effects = (int) strtol(strtok(NULL, ","), NULL, 10);
        (*num)++;
    }
}

void ofb_timingpoints_free(TimingPoint **tp) {
    free(*tp);
    *tp = NULL;
}