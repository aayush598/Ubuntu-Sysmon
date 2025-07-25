#include <ncurses.h>
#include "../include/theme.h"

void init_theme() {
    if (!has_colors()) return;
    start_color();
    init_pair(1, COLOR_CYAN,   COLOR_BLACK);  // Title
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Labels
    init_pair(3, COLOR_GREEN,  COLOR_BLACK);  // Values
}

void apply_color_title() {
    attron(COLOR_PAIR(1) | A_BOLD);
}
void apply_color_label() {
    attron(COLOR_PAIR(2));
}
void apply_color_value() {
    attron(COLOR_PAIR(3) | A_BOLD);
}
void reset_color() {
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
    attroff(A_BOLD);
}
