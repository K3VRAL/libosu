#ifndef OOS_GENERAL_H
#define OOS_GENERAL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#general
typedef struct General {
    char *audio_filename;
    int audio_lead_in;
    char *audio_hash;
    int preview_time;
    int countdown;
    char *sample_set;
    double stack_leniency;
    int mode;
    bool letterbox_in_breaks;
    bool story_fire_in_front;
    bool use_skin_sprites;
    bool always_show_playfield;
    char *overlay_position;
    char *skin_preference;
    bool epilepsy_warning;
    int countdown_offset;
    bool special_style;
    bool widescreen_storyboard;
    bool samples_match_playback_rate;
} General;

/*

    argsreturn
    args
*/
void oos_general_init(General *);

/*

    argsreturn
    args
*/
void oos_general_free(General);

#endif