#include "file/beatmap.h"

Beatmap of_beatmap_init(void) {
    Beatmap beatmap = {
        .structure = oos_structure_init(),
        .general = oos_general_init(),
        .editor = oos_editor_init(),
        .metadata = oos_metadata_init(),
        .difficulty = oos_difficulty_init(),
        .events = NULL,
        .timing_points = NULL,
        .colours = NULL,
        .hit_objects = NULL
    };
    return beatmap;
}

void of_beatmap_free(Beatmap *beatmap) {
    oos_general_free(&beatmap->general);
    oos_editor_free(&beatmap->editor);
    oos_metadata_free(&beatmap->metadata);
    // oos_event_free(beatmap->events); // TODO
    oos_timingpoint_free(beatmap->timing_points);
    oos_colour_free(beatmap->colours);
    oos_hitobject_free(beatmap->hit_objects, beatmap->num_ho);
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
                ofb_structure_setfromstring(&beatmap->structure, line);
                break;

            case general:
                ofb_general_setfromstring(&beatmap->general, line);
                break;

            case editor:
                ofb_editor_setfromstring(&beatmap->editor, line);
                break;

            case metadata:
                ofb_metadata_setfromstring(&beatmap->metadata, line);
                break;

            case difficulty:
                ofb_difficulty_setfromstring(&beatmap->difficulty, line);
                break;

            case events:
                ofb_event_setfromstring(beatmap->events, line);
                break;

            case timing_points: {
                beatmap->timing_points = realloc(beatmap->timing_points, (beatmap->num_tp + 1) * sizeof(TimingPoint));
                TimingPoint *temp = ofb_timingpoint_addfromstring(line);
                if (temp != NULL) {
                    *(beatmap->timing_points + beatmap->num_tp) = *temp;
                    beatmap->num_tp++;
                    free(temp);
                }
                break;
            }

            case colours: {
                beatmap->colours = realloc(beatmap->colours, (beatmap->num_colour + 1) * sizeof(Colour));
                Colour *temp = ofb_colour_addfromstring(line);
                if (temp != NULL) {
                    *(beatmap->colours + beatmap->num_colour) = *temp;
                    beatmap->num_colour++;
                    free(temp);
                }
                break;
            }

            case hit_objects: {
                beatmap->hit_objects = realloc(beatmap->hit_objects, (beatmap->num_ho + 1) * sizeof(HitObject));
                HitObject *temp = ofb_hitobject_addfromstring(line);
                if (temp != NULL) {
                    *(beatmap->hit_objects + beatmap->num_ho) = *temp;
                    beatmap->num_ho++;
                    free(temp);
                }
                break;
            }
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }
}

void of_beatmap_tofile(Beatmap *beatmap, FILE *fp) {
    {
        char *temp = ofb_structure_tostring(beatmap->structure);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[General]\n", fp);
        char *temp = ofb_general_tostring(beatmap->general);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Editor]\n", fp);
        char *temp = ofb_editor_tostring(beatmap->editor);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {   
        fputs("[Metadata]\n", fp);
        char *temp = ofb_metadata_tostring(beatmap->metadata);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Difficulty]\n", fp);
        char *temp = ofb_difficulty_tostring(beatmap->difficulty);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Events]\n", fp);
        // for (int i = 0; i < beatmap->num_event; i++) {
        //     char *temp = ofb_event_tostring(*(beatmap->events + i));
        //     fputs(, fp);
        //     free(temp);
        // }
        fputs("\n", fp);
    }

    {
        fputs("[TimingPoints]\n", fp);
        for (int i = 0; i < beatmap->num_tp; i++) {
            char *temp = ofb_timingpoint_tostring(*(beatmap->timing_points + i));
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }

    {
        fputs("[Colours]\n", fp);
        for (int i = 0; i < beatmap->num_colour; i++) {
            char *temp = ofb_colour_tostring(*(beatmap->colours + i), i + 1);
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }

    {   
        fputs("[HitObjects]\n", fp);
        for (int i = 0; i < beatmap->num_ho; i++) {
            char *temp = ofb_hitobject_tostring(*(beatmap->hit_objects + i));
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }
}