#ifndef OFB_EDITOR_H
#define OFB_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#editor
typedef struct Editor {
    int *bookmarks;
    unsigned int num_bookmark;
    double distance_spacing;
    double beat_divisor;
    int grid_size;
    double timeline_zoom;
} Editor;

Editor ofb_editor_init();
void ofb_editor_free(Editor *);
void ofb_editor_set(Editor *, char *);

#endif