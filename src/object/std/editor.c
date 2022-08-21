#include "object/std/editor.h"

Editor oos_editor_init() {
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

void oos_editor_free(Editor *editor) {
    if (editor->bookmarks != NULL) {
        free(editor->bookmarks);
    }
}