// src/cpufreq.c
#include "../include/cpufreq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_cpu_freqs_khz(int freqs_khz[], int max_cores) {
    int count = 0;
    char path[128];
    FILE *fp;

    for (int i = 0; i < max_cores; i++) {
        snprintf(path, sizeof(path),
                 "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq", i);

        fp = fopen(path, "r");
        if (!fp) break;  // No more cores

        if (fscanf(fp, "%d", &freqs_khz[i]) != 1) {
            freqs_khz[i] = -1;
        }

        fclose(fp);
        count++;
    }

    return count;
}
