#include "object/std.h"

Difficulty oos_difficulty_init(void) {
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

void oos_difficulty_free(Difficulty difficulty) {
    // Why not
}