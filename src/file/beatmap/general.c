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
                if (general->audio_filename != NULL) {
                    free(general->audio_filename);
                }
                general->audio_filename = strdup(value);
            } else if (strcmp("AudioLeadIn", key) == 0) {
                general->audio_lead_in = (int) strtol(value, NULL, 10);
            } else if (strcmp("AudioHash", key) == 0) {
                if (general->audio_hash != NULL) {
                    free(general->audio_hash);
                }
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
                if (general->overlay_position != NULL) {
                    free(general->overlay_position);
                }
                general->overlay_position = strdup(value);
            } else if (strcmp("SkinPreference", key) == 0) {
                if (general->skin_preference != NULL) {
                    free(general->skin_preference);
                }
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

void ofb_general_tofile(General general, FILE *fp) {
    fputs("[General]\n", fp);
    struct {
        char *name;
        union {
            char *cp;
            int n;
            double d;
            bool b;
        } info;
        enum {
            cp,
            n,
            d,
            b
        } type;
    } data[] = {
        {
            .name = "AudioFilename",
            .info.cp = general.audio_filename,
            .type = cp,
        }, {
            .name = "AudioLeadIn",
            .info.n = general.audio_lead_in,
            .type = n,
        }, {
            .name = "AudioHash",
            .info.cp = general.audio_hash,
            .type = cp,
        }, {
            .name = "PreviewTime",
            .info.n = general.preview_time,
            .type = n,
        }, {
            .name = "Countdown",
            .info.n = general.countdown,
            .type = n,
        }, {
            .name = "SampleSet",
            .info.cp = general.sample_set,
            .type = cp,
        }, {
            .name = "StackLeniency",
            .info.d = general.stack_leniency,
            .type = d,
        }, {
            .name = "Mode",
            .info.n = general.mode,
            .type = n,
        }, {
            .name = "LetterboxInBreaks",
            .info.b = general.letterbox_in_breaks,
            .type = b,
        }, {
            .name = "StoryFireInFront",
            .info.b = general.story_fire_in_front,
            .type = b,
        }, {
            .name = "UseSkinSprites",
            .info.b = general.use_skin_sprites,
            .type = b,
        }, {
            .name = "AlwaysShowPlayfield",
            .info.b = general.always_show_playfield,
            .type = b,
        }, {
            .name = "OverlayPosition",
            .info.cp = general.overlay_position,
            .type = cp,
        }, {
            .name = "SkinPreference",
            .info.cp = general.skin_preference,
            .type = cp,
        }, {
            .name = "EpilepsyWarning",
            .info.b = general.epilepsy_warning,
            .type = b,
        }, {
            .name = "CountdownOffset",
            .info.n = general.countdown_offset,
            .type = n,
        }, {
            .name = "SpecialStyle",
            .info.b = general.special_style,
            .type = b,
        }, {
            .name = "WidescreenStoryboard",
            .info.b = general.widescreen_storyboard,
            .type = b,
        }, {
            .name = "SamplesMatchPlaybackRate",
            .info.b = general.samples_match_playback_rate,
            .type = b,
        },
    };
    for (int i = 0; i < 19; i++) {
        char *output;
        switch ((data + i)->type) {
            case cp: {
                int len = strlen((data + i)->name) + 1 + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                sprintf(output, "%s: ", (data + i)->name);
                if ((data + i)->info.cp != NULL) {
                    printf("%d %lu\n", len, len + strlen((data + i)->info.cp));
                    output = realloc(output, (len + strlen((data + i)->info.cp) + 1) * sizeof(char)); // TODO fix "OverlayPosition"? It's possible that the heap is being smashed/corrupted by another heap
                    strcat(output, (data + i)->info.cp);
                    strcat(output, "\n");
                }
                break;
            }

            case n: 
                output = malloc((strlen((data + i)->name) + 2 + ((data + i)->info.n == 0 ? 1 : (floor(log10(abs((data + i)->info.n))) + 1 + (data + i)->info.n < 0 ? 1 : 0)) + 2) * sizeof(char));
                sprintf(output, "%s: %d\n", (data + i)->name, (data + i)->info.n);
                break;

            case d: {
                int size_floor = ((int) (data + i)->info.d == 0 ? 1 : (floor(log10(abs((int) (data + i)->info.d))) + 1 + ((int) (data + i)->info.d < 0 ? 1 : 0)));
                int trailing_zeros = 0;
                char analysing_trail[size_floor + 12];
                sprintf(analysing_trail, "%.12f", (data + i)->info.d);
                for (int i = 0; i < strlen(analysing_trail); i++) {
                    if (analysing_trail[i] == '0') {
                        trailing_zeros--;
                    } else {
                        trailing_zeros = 12;
                    }
                }
                
                output = malloc((strlen((data + i)->name) + 1 + 1 + (size_floor + trailing_zeros) + 1 + 1) * sizeof(char));
                sprintf(output, "%s: %*f\n", (data + i)->name, trailing_zeros, (data + i)->info.d);
                break;
            }

            case b:
                output = malloc((strlen((data + i)->name) + 2 + 1 + 2) * sizeof(char));
                sprintf(output, "%s: %d\n", (data + i)->name, (data + i)->info.b);
                break;
        }
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}