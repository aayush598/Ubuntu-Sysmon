#include <stdio.h>
#include <unistd.h>
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/uptime.h"

int main() {
    // CPU
    CPUStats prev, curr;
    read_cpu_stats(&prev);
    sleep(1);
    read_cpu_stats(&curr);
    double cpu_usage = calculate_cpu_usage(&prev, &curr);
    printf("CPU Usage: %.2f%%\n", cpu_usage);

    // Memory
    MemoryStats mem;
    read_memory_stats(&mem);
    double mem_usage_percent = get_memory_usage_percent(&mem);
    unsigned long used = get_memory_used(&mem);
    printf("Memory Usage: %.2f%% (%lu MB / %lu MB)\n",
        mem_usage_percent, used / 1024, mem.total / 1024);

    // Uptime
    char uptime_str[64];
    get_uptime_formatted(uptime_str, sizeof(uptime_str));
    printf("%s\n", uptime_str);

    return 0;
}
