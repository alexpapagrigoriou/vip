#include "keymap.h"

#include "input.h"
#include "keys.h"
#include "motion.h"

static void handleNormalMode(Editor* editor, int key) {
    if (editor->pending_operator == '\0') {
        switch (key) {
            case 'q':
                enterExitMode(&editor->mode);
                break;
            case 'k':
                moveUp(editor);
                break;
            case 'j':
                moveDown(editor);
                break;
            case 'l':
                moveRight(editor);
                break;
            case 'h':
                moveLeft(editor);
                break;
            case 'i':
                enterInsertMode(&editor->mode);
                break;
            case 'a':
                enterInsertMode(&editor->mode);
                moveRight(editor);
                break;
            case 'I':
                enterInsertMode(&editor->mode);
                jumpToFirstNonBlankCharOfLine(editor);
                break;
            case 'A':
                enterInsertMode(&editor->mode);
                jumpToEndOfLine(editor);
                break;
            case 'o':
                enterInsertMode(&editor->mode);
                appendLine(editor->buffer, &editor->cursor);
                break;
            case 'O':
                enterInsertMode(&editor->mode);
                prependLine(editor->buffer, &editor->cursor);
                break;
            case 'x':
                deleteChar(editor->buffer, &editor->cursor);
                break;
            case 'X':
                if (editor->cursor.col != 0) {
                    moveLeft(editor);
                    deleteChar(editor->buffer, &editor->cursor);
                }
                break;
            case 'S':
                clearLine(editor->buffer, &editor->cursor);
                enterInsertMode(&editor->mode);
                break;
            case 'p':
                moveRight(editor);
                // TODO: paste from register
                break;
            case 'P':
                // TODO: paste from register
                break;
            case 'd':
                // editor->pending_operator = 'd';
                break;
            case 'c':
                //  editor->pending_operator = 'c';
                break;
            case 'y':
                // editor->pending_operator = 'y';
                break;
        }
    }
}

static void handleInsertMode(Editor* editor, int key) {
    switch (key) {
        case ESC:
        case CTRL_F:
            moveLeft(editor);
            enterNormalMode(&editor->mode);
            break;
        case TAB:
            insertString(editor->buffer, &editor->cursor, "    ");
            break;
        case ENTER:
            appendLine(editor->buffer, &editor->cursor);
            break;
        case UP:
            moveUp(editor);
            break;
        case DOWN:
            moveDown(editor);
            break;
        case RIGHT:
            moveRight(editor);
            break;
        case LEFT:
            moveLeft(editor);
            break;
        default:
            if (isPrintable(key)) {
                insertChar(editor->buffer, &editor->cursor, key);
            }
            break;
    }
}

static void handleVisualMode(Editor* editor, int key) {
    (void)editor;
    (void)key;
}

static void handleCommandMode(Editor* editor, int key) {
    (void)editor;
    (void)key;
}

void handleKey(Editor* editor, int key) {
    switch (editor->mode) {
        case NORMAL:
            handleNormalMode(editor, key);
            break;
        case INSERT:
            handleInsertMode(editor, key);
            break;
        case VISUAL:
            handleVisualMode(editor, key);
            break;
        case COMMAND:
            handleCommandMode(editor, key);
            break;
        case EXIT:
            break;
    }
}
