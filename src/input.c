#include <stdlib.h>
#include <ncurses.h>
#include "../include/input.h"

ViewMode handle_input(int ch, ViewMode current, SortMode *sort_mode) {
    switch (ch) {
        case 'q':
        case 'Q':
            endwin();
            exit(0);

        case 'a':
        case 'A':
            return VIEW_ALL;

        case 'c':
        case 'C':
            return VIEW_CPU;

        case 'm':
        case 'M':
            return VIEW_MEMORY;

        case 'n':
        case 'N':
            return VIEW_NETWORK;

        case 'p':
        case 'P':
            return VIEW_PROCESSES;

        case 's':
        case 'S':
            if (*sort_mode == SORT_CPU) {
                *sort_mode = SORT_MEM;
            } else if (*sort_mode == SORT_MEM) {
                *sort_mode = SORT_NONE;
            } else {
                *sort_mode = SORT_CPU;
            }
            break;

        default:
            break;
    }

    return current;
}
