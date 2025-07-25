#include <stdio.h>
#include <time.h>
#include "../include/uptime.h"

void get_uptime_formatted(char* buffer, int buffer_size) {
    FILE* file = fopen("/proc/uptime", "r");
    if (!file) {
        snprintf(buffer, buffer_size, "Uptime: Unknown");
        return;
    }

    double uptime_seconds;
    fscanf(file, "%lf", &uptime_seconds);
    fclose(file);

    int days = uptime_seconds / 86400;
    int hours = ((int)uptime_seconds % 86400) / 3600;
    int minutes = ((int)uptime_seconds % 3600) / 60;

    snprintf(buffer, buffer_size, "Uptime: %dd %02dh %02dm", days, hours, minutes);
}
