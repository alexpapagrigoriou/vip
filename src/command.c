#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "keys.h"
#include "movement.h"

typedef enum {
    CMD_WRITE,
    CMD_QUIT,
    CMD_WRITE_AND_QUIT,
    CMD_QUIT_FORCE,
    CMD_SET,
    CMD_UNKNOWN
} CommandType;

static CommandType parse_command(char* command) {
    if (strcmp(command, "w") == 0 || strcmp(command, "write") == 0) {
        return CMD_WRITE;
    }

    if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0) {
        return CMD_QUIT;
    }

    if (strcmp(command, "wq") == 0 || strcmp(command, "x") == 0 || strcmp(command, "xit") == 0) {
        return CMD_WRITE_AND_QUIT;
    }

    if (strcmp(command, "q!") == 0) {
        return CMD_QUIT_FORCE;
    }

    if (strcmp(command, "set") == 0) {
        return CMD_SET;
    }

    return CMD_UNKNOWN;
}

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

static size_t string_to_digit(char* str) {
    size_t digit = (size_t)strtoull(str, NULL, 10);
    if (digit == 0) {
        digit = 1;
    }
    return digit;
}

static void execute_command(Editor* editor) {
    char command[COMMAND_LINE_SIZE];
    strncpy(command, editor->command_line.line + 1, sizeof(command));
    command[sizeof(command) - 1] = '\0';

    strip(command);

    if (is_all_digits(command)) {
        size_t row = string_to_digit(command);
        movement_line(editor, row);
        return;
    }

    char* word = strtok(command, " ");

    CommandType command_type = parse_command(word);

    if (command_type == CMD_UNKNOWN) {
        snprintf(editor->command_line.line, sizeof(editor->command_line.line), BRED "Not an editor command: %s" RESET, word);
        return;
    }

    word = strtok(NULL, " ");
    if (word != NULL) {
        // TODO: handle arguments
        // write or write_and_quit filename
        // set number or set nu
        // set nonumber or set nonu
        // set relativenumber or set rnu
        // set norelativenumber or set nornu
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
    if (editor->command_line.line[0] == ':' || editor->command_line.line[0] == '/') {
        editor->command_line.line[0] = '\0';
    }
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
