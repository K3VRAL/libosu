#include "file/beatmap/timing_points.h"

void ofb_timingpoints_add(TimingPoint **tp, unsigned int num, TimingPoint timing_point) {
    if (*tp == NULL) {
        // If there is no space; make space
        *tp = calloc(1, sizeof(TimingPoint));
    } else {
        // If there is space; make more space
        *tp = realloc(*tp, (num + 1) * sizeof(TimingPoint));
    }
    *(*tp + num) = timing_point;
}

void ofb_timingpoints_add_string(TimingPoint **tp, unsigned int *num, char *string) {
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

void ofb_timingpoints_free(TimingPoint *tp) {
    free(tp);
    tp = NULL;
}

void ofb_timingpoints_tofile(TimingPoint *tp, unsigned int num, FILE *fp) {
    fputs("[TimingPoints]\n", fp);
    for (int i = 0; i < num; i++) {
        int time_size = ou_comparing_size((tp + i)->time);
        int size_floor = ou_comparing_size((int) (tp + i)->beat_length);
        int trailing_zeros = ou_comparing_trailing((tp + i)->beat_length, size_floor);
        int meter_size = ((tp + i)->meter == 0 ? 1 : (floor(log10(abs((tp + i)->meter))) + 1 + ((tp + i)->meter < 0 ? 1 : 0)));
        int sampleset_size = ((tp + i)->sample_set == 0 ? 1 : (floor(log10(abs((tp + i)->sample_set))) + 1 + ((tp + i)->sample_set < 0 ? 1 : 0)));
        int sampleindex_size = ((tp + i)->sample_index == 0 ? 1 : (floor(log10(abs((tp + i)->sample_index))) + 1 + ((tp + i)->sample_index < 0 ? 1 : 0)));
        int volume_size = ((tp + i)->volume == 0 ? 1 : (floor(log10(abs((tp + i)->volume))) + 1 + ((tp + i)->volume < 0 ? 1 : 0)));
        int uninherited_size = 1;
        int effects_size = ((tp + i)->effects == 0 ? 1 : (floor(log10(abs((tp + i)->effects))) + 1 + ((tp + i)->effects < 0 ? 1 : 0)));
        int len = time_size + 1 + (size_floor + (trailing_zeros != 0 ? 1 : 0) + trailing_zeros) + 1 + meter_size + 1 +sampleset_size + 1 + sampleindex_size + 1 + volume_size + 1 + uninherited_size + 1 + effects_size + 1;
        char *output = malloc((len + 1) * sizeof(char));
        snprintf(output, len, "%d,%.*f,%d,%d,%d,%d,%d,%d", (tp + i)->time, trailing_zeros, (tp + i)->beat_length, (tp + i)->meter, (tp + i)->sample_set, (tp + i)->sample_index, (tp + i)->volume, (tp + i)->uninherited, (tp + i)->effects);
        strcat(output, "\n");
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}

TimingPointData ofb_timingpointdatas_init(TimingPoint *tp) {
    TimingPointData tpd = {
        .timing_points = tp,
    };
    return tpd;
}