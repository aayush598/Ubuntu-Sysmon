#include <stdio.h>
#include <string.h>
#include "../include/ui.h"

#define VERSION "1.0.0"

void print_help() {
    printf("Ubuntu System Monitor - TUI-based resource monitor for Linux\n");
    printf("Usage:\n");
    printf("  sysmon           Launch the interactive terminal UI\n");
    printf("  sysmon --help    Display this help message\n");
    printf("  sysmon -h        Same as --help\n");
    printf("  sysmon --version Show version information\n");
    printf("  sysmon -v        Same as --version\n");
    printf("\nFeatures:\n");
    printf("  - Live CPU, Memory, Disk, and Network stats\n");
    printf("  - Interactive process list with filtering/sorting\n");
    printf("  - Per-core CPU frequencies, battery info, and more\n");
}

void print_version() {
    printf("Ubuntu System Monitor version %s\n", VERSION);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        if ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)) {
            print_help();
            return 0;
        } else if ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "-v") == 0)) {
            print_version();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[1]);
            fprintf(stderr, "Use --help to see available options.\n");
            return 1;
        }
    }

    start_ui();
    return 0;
}
