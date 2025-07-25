#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    unsigned long total;
    unsigned long free;
    unsigned long buffers;
    unsigned long cached;
} MemoryStats;

void read_memory_stats(MemoryStats* mem);
unsigned long get_memory_used(const MemoryStats* mem);
double get_memory_usage_percent(const MemoryStats* mem);

#endif
