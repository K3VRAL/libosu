#include "file/beatmap/editor.h"

void ofb_editor_setfromstring(Editor *editor, char *key_value_pair) {
    char *token = strtok(key_value_pair, ":");
    if (token == NULL) {
        return;
    }

    char *key = strdup(token);
    char *value = strtok(NULL, "\0");
    if (value == NULL) {
        free(key);
        return;
    }
    
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
    free(key);
}

char *ofb_editor_tostring(Editor editor) {
    ComparingEditor data[] = {
        {
            .name = "Bookmarks",
            .info.s = {
                .p = &editor.bookmarks,
                .n = &editor.num_bookmark,
            },
            .type = e_s,
        }, {
            .name = "DistanceSpacing",
            .info.d = &editor.distance_spacing,
            .type = e_d,
        }, {
            .name = "BeatDivisor",
            .info.d = &editor.beat_divisor,
            .type = e_d,
        }, {
            .name = "GridSize",
            .info.n = &editor.grid_size,
            .type = e_n,
        }, {
            .name = "TimelineZoom",
            .info.d = &editor.timeline_zoom,
            .type = e_d,
        },
    };
    return ou_comparing_editor(data, 5);
}