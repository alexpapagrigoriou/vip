#include "editor.h"

void editorInit(Editor* editor, char* filename) {
    bufferInit(&editor->buffer);
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

void freeEditor(Editor* editor) {
    freeBuffer(&editor->buffer);
}
