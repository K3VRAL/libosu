#include "file/timing_points.h"

TimingPoints osut_init() {
    TimingPoints timing_points = {
        .timing_points = NULL,
        .num = 0
    };
    return timing_points;
}

// TODO
void osut_free(TimingPoints *tp) {
}

void osut_add_TP(TimingPoints *tp, char *string) {
    char *token = strtok(string, ",");
    if (tp->timing_points == NULL || tp->num == 0) {
        // If there is no space; make space
        tp->timing_points = malloc(sizeof(TimingPoints));
    } else {
        // If there is space; make more space
        tp->timing_points = realloc(tp->timing_points, (tp->num + 1) * sizeof(TimingPoints));
    }
    // Set data
    (tp->timing_points + tp->num)->time = (int) strtol(token, NULL, 10);
    (tp->timing_points + tp->num)->beat_length = strtod(strtok(NULL, ","), NULL);
    (tp->timing_points + tp->num)->meter = (int) strtol(strtok(NULL, ","), NULL, 10);
    (tp->timing_points + tp->num)->sample_set = (int) strtol(strtok(NULL, ","), NULL, 10);
    (tp->timing_points + tp->num)->sample_index = (int) strtol(strtok(NULL, ","), NULL, 10);
    (tp->timing_points + tp->num)->volume = (int) strtol(strtok(NULL, ","), NULL, 10);
    (tp->timing_points + tp->num)->uninherited = (bool) strtol(strtok(NULL, ","), NULL, 10);
    (tp->timing_points + tp->num)->effects = (int) strtol(strtok(NULL, ","), NULL, 10);
    tp->num++;
}