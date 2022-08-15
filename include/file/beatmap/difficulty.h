#ifndef OFB_DIFFICULTY_H
#define OFB_DIFFICULTY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "unrelated/comparing.h"

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#difficulty
typedef struct Difficulty {
    double hp_drain_rate;
    double circle_size;
    double overall_difficulty;
    double approach_rate;
    double slider_multiplier;
    double slider_tick_rate;
} Difficulty;

Difficulty ofb_difficulty_init();
void ofb_difficulty_set(Difficulty *, char *);
void ofb_difficulty_free(Difficulty *);
void ofb_difficulty_tofile(Difficulty *, FILE *);

#endif