#include "../include/input.h"

ViewMode handle_input(int ch) {
    switch (ch) {
        case 'c':
        case 'C': return VIEW_CPU;
        case 'm':
        case 'M': return VIEW_MEMORY;
        case 'n':
        case 'N': return VIEW_NETWORK;
        case 'p':
        case 'P': return VIEW_PROCESSES;
        case 'a':
        case 'h':
        case 'H': return VIEW_ALL;
        default: return VIEW_NO_CHANGE;
    }
}
