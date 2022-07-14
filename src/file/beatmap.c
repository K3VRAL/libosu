#include "file/beatmap.h"

Beatmap of_beatmap_init() {
    Beatmap beatmap = {
        .structure = ofb_structure_init(),
        .general = ofb_general_init(),
        .editor = ofb_editor_init(),
        .metadata = ofb_metadata_init(),
        .difficulty = ofb_difficulty_init(),
        .events = NULL,
        .timing_points = NULL,
        .colours = NULL,
        .hit_objects = NULL
    };
    return beatmap;
}

void of_beatmap_free(Beatmap *beatmap) {
    ofb_structure_free(&beatmap->structure);
    ofb_general_free(&beatmap->general);
    ofb_editor_free(&beatmap->editor);
    ofb_metadata_free(&beatmap->metadata);
    ofb_difficulty_free(&beatmap->difficulty);
    ofb_events_free(&beatmap->events);
    ofb_timingpoints_free(&beatmap->timing_points);
    ofb_colours_free(&beatmap->colours);
    ofb_hitobjects_free(&beatmap->hit_objects, beatmap->num_ho);
}

void of_beatmap_set(Beatmap *beatmap, char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return;
    }
    char *extension = strrchr(file_path, '.');
    if (extension == NULL && strcmp(".osu", extension)) {
        return;
    }

    char *line;
    size_t len = 0;
    int read;
    enum current_item {
        structure,
        general,
        editor,
        metadata,
        difficulty,
        events,
        timing_points,
        colours,
        hit_objects
    } curr_item = structure;
    while ((read = getline(&line, &len, fp)) != -1) {
        if (*(line + read - 2) == '\r') {
            *(line + read - 2) = '\0';
        } else if (*(line + read - 1) == '\n') {
            *(line + read - 1) = '\0';
        }
        if (*(line + 0) == '[') {
            if (strcmp("[General]", line) == 0) {
                curr_item = general;
                continue;
            } else if (strcmp("[Editor]", line) == 0) {
                curr_item = editor;
                continue;
            } else if (strcmp("[Metadata]", line) == 0) {
                curr_item = metadata;
                continue;
            } else if (strcmp("[Difficulty]", line) == 0) {
                curr_item = difficulty;
                continue;
            } else if (strcmp("[Events]", line) == 0) {
                curr_item = events;
                continue;
            } else if (strcmp("[TimingPoints]", line) == 0) {
                curr_item = timing_points;
                continue;
            } else if (strcmp("[Colours]", line) == 0) {
                curr_item = colours;
                continue;
            } else if (strcmp("[HitObjects]", line) == 0) {
                curr_item = hit_objects;
                continue;
            }
        }

        switch (curr_item) {
            case structure:
                ofb_structure_set(&beatmap->structure, line);
                break;

            case general:
                ofb_general_set(&beatmap->general, line);
                break;

            case editor:
                ofb_editor_set(&beatmap->editor, line);
                break;

            case metadata:
                ofb_metadata_set(&beatmap->metadata, line);
                break;

            case difficulty:
                ofb_difficulty_set(&beatmap->difficulty, line);
                break;

            case events:
                ofb_events_set(&beatmap->events, &beatmap->num_event, line);
                break;

            case timing_points:
                ofb_timingpoints_add(&beatmap->timing_points, &beatmap->num_tp, line);
                break;

            case colours:
                ofb_colours_add(&beatmap->colours, &beatmap->num_colour, line);
                break;

            case hit_objects:
                ofb_hitobjects_add(&beatmap->hit_objects, &beatmap->num_ho, line);
                break;
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }
}