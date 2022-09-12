#include "object/std.h"

void oos_general_init(General *general) {
    general->audio_filename = NULL;
    general->audio_lead_in = 0;
    general->audio_hash = NULL;
    general->preview_time = -1;
    general->countdown = 1;
    general->sample_set = strdup("Normal");
    general->stack_leniency = 0.7;
    general->mode = 0;
    general->letterbox_in_breaks = false;
    general->story_fire_in_front = true;
    general->use_skin_sprites = false;
    general->always_show_playfield = false;
    general->overlay_position = strdup("NoChange");
    general->skin_preference = NULL;
    general->epilepsy_warning = false;
    general->countdown_offset = 0;
    general->special_style = false;
    general->widescreen_storyboard = false;
    general->samples_match_playback_rate = false;
}

void oos_general_free(General general) {
    if (general.audio_filename != NULL) {
        free(general.audio_filename);
    }
    if (general.audio_hash != NULL) {
        free(general.audio_hash);
    }
    if (general.sample_set != NULL) {
        free(general.sample_set);
    }
    if (general.overlay_position != NULL) {
        free(general.overlay_position);
    }
    if (general.skin_preference != NULL) {
        free(general.skin_preference);
    }
}