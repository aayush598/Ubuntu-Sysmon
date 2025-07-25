#include <stdio.h>
#include <string.h>
#include "../include/cpu.h"

void read_cpu_stats(CPUStats* stats) {
    FILE* file = fopen("/proc/stat", "r");
    if (!file) return;

    char line[256];
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "cpu  %llu %llu %llu %llu %llu %llu %llu",
            &stats->user,
            &stats->nice,
            &stats->system,
            &stats->idle,
            &stats->iowait,
            &stats->irq,
            &stats->softirq);
    }


    fclose(file);
}

double calculate_cpu_usage(const CPUStats* prev, const CPUStats* curr) {
    unsigned long long prev_idle = prev->idle + prev->iowait;
    unsigned long long curr_idle = curr->idle + curr->iowait;

    unsigned long long prev_total = prev_idle + prev->user + prev->nice + prev->system + prev->irq + prev->softirq;
    unsigned long long curr_total = curr_idle + curr->user + curr->nice + curr->system + curr->irq + curr->softirq;

    double total_diff = curr_total - prev_total;
    double idle_diff = curr_idle - prev_idle;

    if (total_diff == 0) return 0.0;
    return (100.0 * (total_diff - idle_diff)) / total_diff;
}
