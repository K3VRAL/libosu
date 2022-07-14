#include "file/beatmap/editor.h"

Editor ofb_editor_init() {
    Editor editor = {
        .bookmarks = NULL,
        .num_bookmark = 0,
        .distance_spacing = 1.0,
        .beat_divisor = 4.0,
        .grid_size = 4,
        .timeline_zoom = 7.5
    };
    return editor;
}

void ofb_editor_free(Editor *editor) {
    if (editor->bookmarks != NULL) {
        free(editor->bookmarks);
        editor->bookmarks = NULL;
    }
}

void ofb_editor_set(Editor *editor, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token != NULL) {
        char *key = strdup(token);
        char *value = strtok(NULL, ":");
        if (value != NULL) {
            if (*(value + 0) == ' ') {
                value++;
            }
            if (strcmp("Bookmarks", key) == 0) {
                editor->bookmarks = calloc(1, sizeof(int));
                token = strtok(value, ",");
                while (token != NULL) {
                    editor->bookmarks = realloc(editor->bookmarks, (editor->num_bookmark + 1) * sizeof(int));
                    *(editor->bookmarks + editor->num_bookmark) = (int) strtol(token, NULL, 10);
                    token = strtok(NULL, ",");
                    editor->num_bookmark++;
                }
            } else if (strcmp("DistanceSpacing", key) == 0) {
                editor->distance_spacing = strtod(value, NULL);
            } else if (strcmp("BeatDivisor", key) == 0) {
                editor->beat_divisor = strtod(value, NULL);
            } else if (strcmp("GridSize", key) == 0) {
                editor->grid_size = (int) strtol(value, NULL, 10);
            } else if (strcmp("TimelineZoom", key) == 0) {
                editor->timeline_zoom = strtod(value, NULL);
            }
        }
        free(key);
    }
}