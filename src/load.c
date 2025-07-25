#include <stdio.h>
#include "../include/load.h"

void read_load_avg(LoadAvg* load) {
    FILE* file = fopen("/proc/loadavg", "r");
    if (!file) {
        load->one_min = load->five_min = load->fifteen_min = 0.0f;
        return;
    }

    fscanf(file, "%f %f %f", &load->one_min, &load->five_min, &load->fifteen_min);
    fclose(file);
}

void format_load_avg(char* buffer, int buffer_size, const LoadAvg* load) {
    snprintf(buffer, buffer_size, "Load Avg: %.2f / %.2f / %.2f",
             load->one_min, load->five_min, load->fifteen_min);
}
