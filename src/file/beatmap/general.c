#include "file/beatmap/general.h"

General ofb_general_init() {
    General general = {
        .audio_filename = NULL,
        .audio_lead_in = 0,
        .audio_hash = NULL,
        .preview_time = -1,
        .countdown = 1,
        .sample_set = strdup("Normal"),
        .stack_leniency = 0.7,
        .mode = 2,
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

void ofb_general_free(General *general) {
    if (general->audio_filename != NULL) {
        free(general->audio_filename);
        general->audio_filename = NULL;
    }
    if (general->audio_hash != NULL) {
        free(general->audio_hash);
        general->audio_hash = NULL;
    }
    if (general->sample_set != NULL) {
        free(general->sample_set);
        general->sample_set = NULL;
    }
    if (general->overlay_position != NULL) {
        free(general->overlay_position);
        general->overlay_position = NULL;
    }
    if (general->skin_preference != NULL) {
        free(general->skin_preference);
        general->skin_preference = NULL;
    }
}

void ofb_general_set(General *general, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, ":");
        if (value != NULL) {
            if (*(value + 0) == ' ') {
                value++;
            }
            if (strcmp("AudioFilename", key) == 0) {
                general->audio_filename = strdup(value);
            } else if (strcmp("AudioLeadIn", key) == 0) {
                general->audio_lead_in = (int) strtol(value, NULL, 10);
            } else if (strcmp("AudioHash", key) == 0) {
                general->audio_hash = strdup(value);
            } else if (strcmp("PreviewTime", key) == 0) {
                general->preview_time = (int) strtol(value, NULL, 10);
            } else if (strcmp("Countdown", key) == 0) {
                general->countdown = (int) strtol(value, NULL, 10);
            } else if (strcmp("SampleSet", key) == 0) {
                if (general->sample_set != NULL) {
                    free(general->sample_set);
                }
                general->sample_set = strdup(value);
            } else if (strcmp("StackLeniency", key) == 0) {
                general->stack_leniency = strtod(value, NULL);
            } else if (strcmp("Mode", key) == 0) {
                general->mode = (int) strtol(value, NULL, 10);
            } else if (strcmp("LetterboxInBreaks", key) == 0) {
                general->letterbox_in_breaks = (bool) strtol(value, NULL, 10);
            } else if (strcmp("StoryFireInFront", key) == 0) {
                general->story_fire_in_front = (bool) strtol(value, NULL, 10);
            } else if (strcmp("UseSkinSprites", key) == 0) {
                general->use_skin_sprites = (bool) strtol(value, NULL, 10);
            } else if (strcmp("AlwaysShowPlayfield", key) == 0) {
                general->always_show_playfield = (bool) strtol(value, NULL, 10);
            } else if (strcmp("OverlayPosition", key) == 0) {
                general->overlay_position = strdup(value);
            } else if (strcmp("SkinPreference", key) == 0) {
                general->skin_preference = strdup(value);
            } else if (strcmp("EpilepsyWarning", key) == 0) {
                general->epilepsy_warning = (bool) strtol(value, NULL, 10);
            } else if (strcmp("CountdownOffset", key) == 0) {
                general->countdown_offset = (int) strtol(value, NULL, 10);
            } else if (strcmp("SpecialStyle", key) == 0) {
                general->special_style = (bool) strtol(value, NULL, 10);
            } else if (strcmp("WidescreenStoryboard", key) == 0) {
                general->widescreen_storyboard = (bool) strtol(value, NULL, 10);
            } else if (strcmp("SamplesMatchPlaybackRate", key) == 0) {
                general->samples_match_playback_rate = (bool) strtol(value, NULL, 10);
            }
        }
        free(key);
    }
}