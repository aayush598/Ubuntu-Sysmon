#ifndef INPUT_H
#define INPUT_H

typedef enum {
    VIEW_ALL,
    VIEW_CPU,
    VIEW_MEMORY,
    VIEW_NETWORK,
    VIEW_PROCESSES,
    VIEW_NO_CHANGE
} ViewMode;

ViewMode handle_input(int ch);

#endif
