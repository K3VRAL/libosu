#include "object/std/general.h"

General oos_general_init(void) {
    General general = {
        .audio_filename = NULL,
        .audio_lead_in = 0,
        .audio_hash = NULL,
        .preview_time = -1,
        .countdown = 1,
        .sample_set = strdup("Normal"),
        .stack_leniency = 0.7,
        .mode = 0,
        .letterbox_in_breaks = false,
        .story_fire_in_front = true,
        .use_skin_sprites = false,
        .always_show_playfield = false,
        .overlay_position = strdup("NoChange"),
        .skin_preference = NULL,
        .epilepsy_warning = false,
        .countdown_offset = 0,
        .special_style = false,
        .widescreen_storyboard = false,
        .samples_match_playback_rate = false
    };
    return general;
}

void oos_general_free(General *general) {
    if (general->audio_filename != NULL) {
        free(general->audio_filename);
    }
    if (general->audio_hash != NULL) {
        free(general->audio_hash);
    }
    if (general->sample_set != NULL) {
        free(general->sample_set);
    }
    if (general->overlay_position != NULL) {
        free(general->overlay_position);
    }
    if (general->skin_preference != NULL) {
        free(general->skin_preference);
    }
}