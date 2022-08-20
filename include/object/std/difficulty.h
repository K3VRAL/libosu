#ifndef OOS_DIFFICULTY_H
#define OOS_DIFFICULTY_H

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#difficulty
typedef struct Difficulty {
    double hp_drain_rate;
    double circle_size;
    double overall_difficulty;
    double approach_rate;
    double slider_multiplier;
    double slider_tick_rate;
} Difficulty;

Difficulty oos_difficulty_init(void);

#endif