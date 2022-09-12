#include "object/std.h"

void oos_editor_init(Editor *editor) {
    editor->bookmarks = NULL;
    editor->num_bookmark = 0;
    editor->distance_spacing = 1.0;
    editor->beat_divisor = 4.0;
    editor->grid_size = 4;
    editor->timeline_zoom = 7.5;
}

void oos_editor_free(Editor editor) {
    if (editor.bookmarks != NULL) {
        free(editor.bookmarks);
    }
}