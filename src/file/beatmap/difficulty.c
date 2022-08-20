#include "file/beatmap/difficulty.h"

void ofb_difficulty_setfromstring(Difficulty *difficulty, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, "\0");
        if (value != NULL) {
            if (*(value + 0) == ' ') {
                value++;
            }
            if (strcmp("HPDrainRate", key) == 0) {
                difficulty->hp_drain_rate = strtod(value, NULL);
            } else if (strcmp("CircleSize", key) == 0) {
                difficulty->circle_size = strtod(value, NULL);
            } else if (strcmp("OverallDifficulty", key) == 0) {
                difficulty->overall_difficulty = strtod(value, NULL);
            } else if (strcmp("ApproachRate", key) == 0) {
                difficulty->approach_rate = strtod(value, NULL);
            } else if (strcmp("SliderMultiplier", key) == 0) {
                difficulty->slider_multiplier = strtod(value, NULL);
            } else if (strcmp("SliderTickRate", key) == 0) {
                difficulty->slider_tick_rate = strtod(value, NULL);
            }
        }
        free(key);
    }
}

char *ofb_difficulty_tostring(Difficulty difficulty) {
    ComparingDifficulty data[] = {
        {
            .name = "HPDrainRate",
            .info = &difficulty.hp_drain_rate,
        }, {
            .name = "CircleSize",
            .info = &difficulty.circle_size,
        }, {
            .name = "OverallDifficulty",
            .info = &difficulty.overall_difficulty,
        }, {
            .name = "ApproachRate",
            .info = &difficulty.approach_rate,
        }, {
            .name = "SliderMultiplier",
            .info = &difficulty.slider_multiplier,
        }, {
            .name = "SliderTickRate",
            .info = &difficulty.slider_tick_rate,
        },
    };
    return ou_comparing_difficulty(data, 6);
}