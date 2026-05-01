#include "insert.h"

#include "keys.h"
#include "movement.h"

void parse_insert_mode(Editor* editor, int key) {
    switch (key) {
        case ESC:
        case CTRL_F:
            insert_arrow_left(editor);
            editor->mode = NORMAL;
            break;
        case TAB:
            insert_string(&editor->buffer, &editor->cursor, "    ");
            break;
        case ENTER:
            // TODO: new buffer function that moves text after cursor to the new line
            append_line(&editor->buffer, &editor->cursor);
            break;
        case BACKSPACE:
            // TODO: delete char and move text on this row in end of row - 1 if col == 0
            break;
        case LEFT:
            insert_arrow_left(editor);
            break;
        case UP:
            insert_arrow_up(editor);
            break;
        case DOWN:
            insert_arrow_down(editor);
            break;
        case RIGHT:
            insert_arrow_right(editor);
            break;
        default:
            if (IS_PRINTABLE(key)) {
                insert_char(&editor->buffer, &editor->cursor, key);
            }
            break;
    }
}
