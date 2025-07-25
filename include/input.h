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

typedef enum {
    SORT_NONE,
    SORT_CPU,
    SORT_MEM
} SortMode;

ViewMode handle_input(int ch, ViewMode current, SortMode *sort_mode);

#endif
