#include "object/std.h"

void oos_difficulty_init(Difficulty *difficulty) {
    difficulty->hp_drain_rate = 0.0;
    difficulty->circle_size = 0.0;
    difficulty->overall_difficulty = 0.0;
    difficulty->approach_rate = 0.0;
    difficulty->slider_multiplier = 1.0;
    difficulty->slider_tick_rate = 1.0;
}