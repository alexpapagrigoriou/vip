#include "insert.h"

#include "keys.h"
#include "movement.h"

void parseInsertMode(Editor* editor, int key) {
    switch (key) {
        case ESC:
        case CTRL_F:
            insertArrowLeft(editor);
            editor->mode = NORMAL;
            break;
        case TAB:
            insertString(&editor->buffer, &editor->cursor, "    ");
            break;
        case ENTER:
            // TODO: new buffer function that moves text after cursor to the new line
            appendLine(&editor->buffer, &editor->cursor);
            break;
        case BACKSPACE:
            // TODO: delete char and move text on this row in end of row - 1 if col == 0
            break;
        case LEFT:
            insertArrowLeft(editor);
            break;
        case UP:
            insertArrowUp(editor);
            break;
        case DOWN:
            insertArrowDown(editor);
            break;
        case RIGHT:
            insertArrowRight(editor);
            break;
        default:
            if (IS_PRINTABLE(key)) {
                insertChar(&editor->buffer, &editor->cursor, key);
            }
            break;
    }
}
