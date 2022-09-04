#include "osu.h"

void of_beatmap_init(Beatmap *beatmap) {
    beatmap = malloc(sizeof(*beatmap));

    beatmap->structure = NULL;
    oos_structure_init(beatmap->structure);
    
    beatmap->general = NULL;
    oos_general_init(beatmap->general);

    beatmap->editor = NULL;
    oos_editor_init(beatmap->editor);
    
    beatmap->metadata = NULL;
    oos_metadata_init(beatmap->metadata);
    
    beatmap->difficulty = NULL;
    oos_difficulty_init(beatmap->difficulty);
    
    beatmap->events = NULL;
    beatmap->timing_points = NULL;
    beatmap->colours = NULL;
    beatmap->hit_objects = NULL;
}

void of_beatmap_free(Beatmap *beatmap) {
    oos_general_free(beatmap->general);
    oos_editor_free(beatmap->editor);
    oos_metadata_free(beatmap->metadata);
    oos_event_free(beatmap->events, beatmap->num_event);
    oos_timingpoint_free(beatmap->timing_points);
    oos_colour_free(beatmap->colours);
    oos_hitobject_free(beatmap->hit_objects, beatmap->num_ho);
}

void of_beatmap_set(Beatmap *beatmap, char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return;
    }

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
    char *line;
    while ((line = ou_readingline_line(fp)) != NULL) {
        if (*(line + 0) == '[' && *(line + strlen(line) - 1) == ']') {
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
                ofb_structure_setfromstring(beatmap->structure, line);
                break;

            case general:
                ofb_general_setfromstring(beatmap->general, line);
                break;

            case editor:
                ofb_editor_setfromstring(beatmap->editor, line);
                break;

            case metadata:
                ofb_metadata_setfromstring(beatmap->metadata, line);
                break;

            case difficulty:
                ofb_difficulty_setfromstring(beatmap->difficulty, line);
                break;

            case events: {
                Event *temp = NULL;
                ofb_event_addfromstring(temp, line);
                if (temp == NULL) {
                    break;
                }
                beatmap->events = realloc(beatmap->events, (beatmap->num_event + 1) * sizeof(*beatmap->events));
                *(beatmap->events + beatmap->num_event) = *temp;
                beatmap->num_event++;
                free(temp);
                break;
            }

            case timing_points: {
                TimingPoint *temp = NULL;
                ofb_timingpoint_addfromstring(temp, line);
                if (temp == NULL) {
                    break;
                }
                beatmap->timing_points = realloc(beatmap->timing_points, (beatmap->num_tp + 1) * sizeof(*beatmap->timing_points));
                *(beatmap->timing_points + beatmap->num_tp) = *temp;
                beatmap->num_tp++;
                free(temp);
                break;
            }

            case colours: {
                Colour *temp = NULL;
                ofb_colour_addfromstring(temp, line);
                if (temp == NULL) {
                    break;
                }
                beatmap->colours = realloc(beatmap->colours, (beatmap->num_colour + 1) * sizeof(*beatmap->colours));
                *(beatmap->colours + beatmap->num_colour) = *temp;
                beatmap->num_colour++;
                free(temp);
                break;
            }

            case hit_objects: {
                HitObject *temp = NULL;
                ofb_hitobject_addfromstring(temp, line);
                if (temp == NULL) {
                    break;
                }
                beatmap->hit_objects = realloc(beatmap->hit_objects, (beatmap->num_ho + 1) * sizeof(*beatmap->hit_objects));
                *(beatmap->hit_objects + beatmap->num_ho) = *temp;
                beatmap->num_ho++;
                free(temp);
                break;
            }
        }
    }
    fclose(fp);
}

void of_beatmap_tofile(Beatmap *beatmap, FILE *fp) {
    {
        char *temp = NULL;
        ofb_structure_tostring(temp, beatmap->structure);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[General]\n", fp);
        char *temp = NULL;
        ofb_general_tostring(temp, beatmap->general);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Editor]\n", fp);
        char *temp = NULL;
        ofb_editor_tostring(temp, beatmap->editor);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {   
        fputs("[Metadata]\n", fp);
        char *temp = NULL;
        ofb_metadata_tostring(temp, beatmap->metadata);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Difficulty]\n", fp);
        char *temp = NULL;
        ofb_difficulty_tostring(temp, beatmap->difficulty);
        fputs(temp, fp);
        free(temp);
        fputs("\n", fp);
    }

    {
        fputs("[Events]\n", fp);
        for (int i = 0; i < beatmap->num_event; i++) {
            char *temp = NULL;
            ofb_event_tostring(temp, (beatmap->events + i));
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }

    {
        fputs("[TimingPoints]\n", fp);
        for (int i = 0; i < beatmap->num_tp; i++) {
            char *temp = NULL;
            ofb_timingpoint_tostring(temp, (beatmap->timing_points + i));
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }

    {
        fputs("[Colours]\n", fp);
        for (int i = 0; i < beatmap->num_colour; i++) {
            char *temp = NULL;
            ofb_colour_tostring(temp, (beatmap->colours + i), i + 1);
            fputs(temp, fp);
            free(temp);
        }
        fputs("\n", fp);
    }

    {   
        fputs("[HitObjects]\n", fp);
        for (int i = 0; i < beatmap->num_ho; i++) {
            char *temp = NULL;
            ofb_hitobject_tostring(temp, (beatmap->hit_objects + i));
            fputs(temp, fp);
            free(temp);
        }
    }
}