#include "file/structure.h"

Structure *osus_init(char *file_path) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return NULL;
    }
    char *extension = strrchr(file_path, '.');
    if (extension == NULL && strcmp(".osu", extension)) {
        return NULL;
    }
    Structure *structure = malloc(sizeof(Structure));
    structure->general = osug_init();
    structure->editor = osue_init();
    structure->metadata = osum_init();
    structure->difficulty = osud_init();
    structure->events = osuv_init();
    structure->timing_points = osut_init();
    structure->colours = osuc_init();
    structure->hit_objects = osuh_init();

    char *line;
    size_t len = 0;
    int read;
    enum current_item {
        version,
        general,
        editor,
        metadata,
        difficulty,
        events,
        timing_points,
        colours,
        hit_objects
    } curr_item = version;
    while ((read = getline(&line, &len, fp)) != -1) {
        *(line + read - 2) = '\0'; // Remove \n
        if (strcmp("[General]", line) == 0) {
            curr_item = general;
        } else if (strcmp("[Editor]", line) == 0) {
            curr_item = editor;
        } else if (strcmp("[Metadata]", line) == 0) {
            curr_item = metadata;
        } else if (strcmp("[Difficulty]", line) == 0) {
            curr_item = difficulty;
        } else if (strcmp("[Events]", line) == 0) {
            curr_item = events;
        } else if (strcmp("[TimingPoints]", line) == 0) {
            curr_item = timing_points;
        } else if (strcmp("[Colours]", line) == 0) {
            curr_item = colours;
        } else if (strcmp("[HitObjects]", line) == 0) {
            curr_item = hit_objects;
        }

        switch (curr_item) {
            case version: {
                char *version = strrchr(line, 'v');
                if (version != NULL) {
                    structure->version = (int) strtol(version, NULL, 10);
                }
            } break;

            case general:
                osug_set(&structure->general, line);
                break;

            case editor:
                osue_set(&structure->editor, line);
                break;

            case metadata:
                osum_set(&structure->metadata, line);
                break;

            case difficulty:
                osud_set(&structure->difficulty, line);
                break;

            case events:
            //     osuv_set(&structure->events, line);
                break;

            case timing_points:
                osut_add_TP(&structure->timing_points, line);
                break;

            case colours:
                osuc_add_C(&structure->colours, line);
                break;

            case hit_objects:
                osuh_add_HO(&structure->hit_objects, line);
                break;
        }
    }
    fclose(fp);
    if (line) {
        free(line);
    }
    return structure;
}

void osus_free(Structure *structure) {
    if (structure != NULL) {
        osug_free(&structure->general);
        osue_free(&structure->editor);
        osum_free(&structure->metadata);
        osuv_free(&structure->events);
        osut_free(&structure->timing_points);
        osuc_free(&structure->colours);
        osuh_free(&structure->hit_objects);
    }
}