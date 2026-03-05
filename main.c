#include <stdio.h>

#include "input.h"
#include "terminal.h"

int main() {
    enableRawMode();

    printf("Welcome to VIP - Vi rIPoff!\n\nPress any key.\n");

    while (1) {
        int key = readKey();
        if (key == 'q') {
            break;
        }

        printf("Key code: %d\t(testing... press 'q' to exit)\n", key);
    }
    return 0;
}
