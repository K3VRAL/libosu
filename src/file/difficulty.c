#include "file/difficulty.h"

Difficulty osud_init() {
    Difficulty difficulty = {
        .hp_drain_rate = 0.0,
        .circle_size = 0.0,
        .overall_difficulty = 0.0,
        .approach_rate = 0.0,
        .slider_multiplier = 1.0,
        .slider_tick_rate = 1.0
    };
    return difficulty;
}

void osud_set(Difficulty *difficulty, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strdup(strtok(NULL, ":"));
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
        free(key);
        free(value);
    }
}