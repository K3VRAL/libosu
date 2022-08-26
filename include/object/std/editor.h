#ifndef OOS_EDITOR_H
#define OOS_EDITOR_H

#include <stdlib.h>

// https://osu.ppy.sh/wiki/en/Client/File_formats/Osu_%28file_format%29#editor
typedef struct Editor {
    int *bookmarks;
    unsigned int num_bookmark;
    double distance_spacing;
    double beat_divisor;
    int grid_size;
    double timeline_zoom;
} Editor;

Editor oos_editor_init(void);
void oos_editor_free(Editor);

#endif