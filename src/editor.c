#include "editor.h"

void editor_init(Editor* editor, char* filename) {
    buffer_init(&editor->buffer);
    editor->cursor = (Position){0, 0};
    editor->save_curosr_col = true;
    editor->prev_cursor_col = 0;
    editor->text = (Position){0, 0};
    editor->status_line[0] = '\0';
    editor->status_cursor_col = 0;
    editor->in_start = filename == NULL;
    editor->mode = NORMAL;
    editor->filename = filename;
}

void free_editor(Editor* editor) {
    free_buffer(&editor->buffer);
}
