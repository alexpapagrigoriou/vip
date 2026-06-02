#include "command.h"

#include <stdlib.h>
#include <string.h>

#include "keys.h"
#include "movement.h"

static void strip(char* str) {
    char* start = str;

    while (*start && (char)*start == SPACE) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    char* end = str + strlen(str);

    while (end > str && (char)*(end - 1) == SPACE) {
        end--;
    }
    *end = '\0';
}

static bool is_all_digits(char* str) {
    while (*str) {
        if (!(IS_DIGIT((char)*str))) {
            return false;
        }
        str++;
    }

    return true;
}

static void execute_command(Editor* editor) {
    char* command = editor->command_line.line + 1;

    strip(command);

    if (is_all_digits(command)) {
        size_t row = (size_t)strtoull(command, NULL, 10);
        movement_line(editor, row);
        return;
    }
}

static void execute_search(Editor* editor) {
    (void)editor;
}

static void execute_command_mode(Editor* editor) {
    if (editor->command_line.length == 1) {
        return;
    }

    if (editor->command_line.line[0] == ':') {
        execute_command(editor);
    } else if (editor->command_line.line[0] == '/') {
        execute_search(editor);
    }
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
