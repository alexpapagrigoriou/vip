#include "command.h"

#include <string.h>

#include "keys.h"

// TODO: implement command mode parser
//       for "go_to_line" use movement_line ex. (:4) -> movement_line(editor, 4);
static void execute_command_mode(Editor* editor) {
    (void)editor;
}

static void command_init(Editor* editor) {
    editor->mode = NORMAL;
    editor->command_line.line[0] = '\0';
}

static void execute_and_init(Editor* editor) {
    execute_command_mode(editor);
    command_init(editor);
}

void parse_command_mode(Editor* editor, int key) {
    switch (key) {
        case ESC:
        case CTRL_F:
            command_init(editor);
            break;
        case ENTER:
            execute_and_init(editor);
            break;
        case BACKSPACE:
            if (editor->command_line.cursor_col <= 1) {
                if (editor->command_line.length == 1) {
                    command_init(editor);
                }
                break;
            }

            memmove(&editor->command_line.line[editor->command_line.cursor_col - 1], &editor->command_line.line[editor->command_line.cursor_col], editor->command_line.length - editor->command_line.cursor_col + 1);
            editor->command_line.cursor_col--;
            editor->command_line.length--;
            break;
        case LEFT:
            if (editor->command_line.cursor_col > 1) {
                editor->command_line.cursor_col--;
            }
            break;
        case RIGHT:
            if (editor->command_line.cursor_col < editor->command_line.length) {
                editor->command_line.cursor_col++;
            }
            break;
        default:
            if (IS_PRINTABLE(key)) {
                memmove(&editor->command_line.line[editor->command_line.cursor_col + 1], &editor->command_line.line[editor->command_line.cursor_col], editor->command_line.length - editor->command_line.cursor_col + 1);
                editor->command_line.line[editor->command_line.cursor_col] = key;
                editor->command_line.length++;
                editor->command_line.cursor_col++;
            }
            break;
    }
}
