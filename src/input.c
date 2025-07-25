#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

// Handles live search input while in search mode
int handle_search_input(int ch, char *search_query, int *is_searching) {
    if (*is_searching) {
        if (ch == 10 || ch == '\n') {  // Enter key ends search
            *is_searching = 0;
            return 1;
        } else if (ch == 127 || ch == KEY_BACKSPACE || ch == 8) {
            int len = strlen(search_query);
            if (len > 0) {
                search_query[len - 1] = '\0';
            }
        } else if (isprint(ch) && strlen(search_query) < 63) {
            int len = strlen(search_query);
            search_query[len] = ch;
            search_query[len + 1] = '\0';
        }
        return 1;
    }

    if (ch == '/') {
        *is_searching = 1;
        search_query[0] = '\0';  // Clear existing query
        return 1;
    }

    return 0;
}
