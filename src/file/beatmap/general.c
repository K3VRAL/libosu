#include "file/beatmap.h"

void ofb_general_setfromstring(General *general, char *key_value_pair) {
	char *token = strtok(key_value_pair, ":");
	if (token == NULL) {
		return;
	}

	char *key = strdup(token);
	char *value = strtok(NULL, "\0");
	if (value == NULL) {
		return;
	}
	
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
	free(key);
}

void ofb_general_tostring(char *output, General general) {
	General original;
	oos_general_init(&original);
	ComparingGeneral data[] = {
		{
			.name = "AudioFilename",
			.info.cp = general.audio_filename,
			.original.cp = original.audio_filename,
			.type = g_cp,
		}, {
			.name = "AudioLeadIn",
			.info.n = general.audio_lead_in,
			.original.n = original.audio_lead_in,
			.type = g_n,
		}, {
			.name = "AudioHash",
			.info.cp = general.audio_hash,
			.original.cp = original.audio_hash,
			.type = g_cp,
		}, {
			.name = "PreviewTime",
			.info.n = general.preview_time,
			.original.n = original.preview_time,
			.type = g_n,
		}, {
			.name = "Countdown",
			.info.n = general.countdown,
			.original.n = original.countdown,
			.type = g_n,
		}, {
			.name = "SampleSet",
			.info.cp = general.sample_set,
			.original.cp = original.sample_set,
			.type = g_cp,
		}, {
			.name = "StackLeniency",
			.info.d = general.stack_leniency,
			.original.d = original.stack_leniency,
			.type = g_d,
		}, {
			.name = "Mode",
			.info.n = general.mode,
			.original.n = original.mode,
			.type = g_n,
		}, {
			.name = "LetterboxInBreaks",
			.info.b = general.letterbox_in_breaks,
			.original.b = original.letterbox_in_breaks,
			.type = g_b,
		}, {
			.name = "StoryFireInFront",
			.info.b = general.story_fire_in_front,
			.original.b = original.story_fire_in_front,
			.type = g_b,
		}, {
			.name = "UseSkinSprites",
			.info.b = general.use_skin_sprites,
			.original.b = original.use_skin_sprites,
			.type = g_b,
		}, {
			.name = "AlwaysShowPlayfield",
			.info.b = general.always_show_playfield,
			.original.b = original.always_show_playfield,
			.type = g_b,
		}, {
			.name = "OverlayPosition",
			.info.cp = general.overlay_position,
			.original.cp = original.overlay_position,
			.type = g_cp,
		}, {
			.name = "SkinPreference",
			.info.cp = general.skin_preference,
			.original.cp = original.skin_preference,
			.type = g_cp,
		}, {
			.name = "EpilepsyWarning",
			.info.b = general.epilepsy_warning,
			.original.b = original.epilepsy_warning,
			.type = g_b,
		}, {
			.name = "CountdownOffset",
			.info.n = general.countdown_offset,
			.original.n = original.countdown_offset,
			.type = g_n,
		}, {
			.name = "SpecialStyle",
			.info.b = general.special_style,
			.original.b = original.special_style,
			.type = g_b,
		}, {
			.name = "WidescreenStoryboard",
			.info.b = general.widescreen_storyboard,
			.original.b = original.widescreen_storyboard,
			.type = g_b,
		}, {
			.name = "SamplesMatchPlaybackRate",
			.info.b = general.samples_match_playback_rate,
			.original.b = original.samples_match_playback_rate,
			.type = g_b,
		},
	};
	output = ou_comparing_general(data, 19);
	oos_general_free(original);
}