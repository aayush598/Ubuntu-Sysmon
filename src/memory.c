#include <stdio.h>
#include <string.h>
#include "../include/memory.h"

void read_memory_stats(MemoryStats* mem) {
    FILE* file = fopen("/proc/meminfo", "r");
    if (!file) return;

    char label[64];
    unsigned long value;
    char unit[8];

    while (fscanf(file, "%63s %lu %7s\n", label, &value, unit) != EOF) {
        if (strcmp(label, "MemTotal:") == 0)
            mem->total = value;
        else if (strcmp(label, "MemFree:") == 0)
            mem->free = value;
        else if (strcmp(label, "Buffers:") == 0)
            mem->buffers = value;
        else if (strcmp(label, "Cached:") == 0)
            mem->cached = value;
    }

    fclose(file);
}

unsigned long get_memory_used(const MemoryStats* mem) {
    return mem->total - mem->free - mem->buffers - mem->cached;
}

double get_memory_usage_percent(const MemoryStats* mem) {
    if (mem->total == 0) return 0.0;
    return 100.0 * get_memory_used(mem) / mem->total;
}
