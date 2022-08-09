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

void ofb_editor_tofile(Editor editor, FILE *fp) {
    fputs("[Editor]\n", fp);
    struct {
        char *name;
        union {
            int n;
            double d;
            struct {
                int *p;
                unsigned int n;
            } s;
        } info;
        enum {
            n,
            d,
            s
        } type;
    } data[] = {
        {
            .name = "Bookmarks",
            .info.s = {
                .p = editor.bookmarks,
                .n = editor.num_bookmark,
            },
            .type = s,
        }, {
            .name = "DistanceSpacing",
            .info.d = editor.distance_spacing,
            .type = d,
        }, {
            .name = "BeatDivisor",
            .info.d = editor.beat_divisor,
            .type = d,
        }, {
            .name = "GridSize",
            .info.n = editor.grid_size,
            .type = n,
        }, {
            .name = "TimelineZoom",
            .info.d = editor.timeline_zoom,
            .type = d,
        },
    };
    for (int i = 0; i < 5; i++) {
        char *output;
        switch ((data + i)->type) {
            case n:
                output = malloc((strlen((data + i)->name) + ((data + i)->info.n == 0 ? 1 : (floor(log10(abs((data + i)->info.n))) + 1 + ((data + i)->info.n < 0 ? 1 : 0))) + 2) * sizeof(char));
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

            case s: {
                int len = strlen((data + i)->name) + 1 + 1 + 1;
                output = malloc((len + 1) * sizeof(char));
                sprintf(output, "%s: ", (data + i)->name);
                if ((data + i)->info.s.p != NULL || (data + i)->info.s.n != 0) {
                    for (int j = 0; j < (data + i)->info.s.n; j++) {
                        int len_buffer = 1 + (*((data + i)->info.s.p + j) == 0 ? 1 : (floor(log10(abs(*((data + i)->info.s.p + j)))) + 1 + (*((data + i)->info.s.p + j) < 0 ? 1 : 0)));
                        len += len_buffer + (j == 0 ? 0 : 1);
                        char *buffer = malloc((len_buffer + 1) * sizeof(char));
                        sprintf(buffer, (j == 0 ? "%d" : ",%d"), *((data + i)->info.s.p + j));
                        output = realloc(output, (len + 1) * sizeof(char));
                        strcat(output, buffer);
                        free(buffer);
                    }
                }
                strcat(output, "\n");
                break;
            }
        }
        fputs(output, fp);
        free(output);
    }
    fputs("\n", fp);
}