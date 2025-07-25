#include <stdio.h>
#include <unistd.h>
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/uptime.h"
#include "../include/load.h"
#include "../include/disk.h"
#include "../include/network.h"


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

    // Load Average
    LoadAvg load;
    char load_str[64];
    read_load_avg(&load);
    format_load_avg(load_str, sizeof(load_str), &load);
    printf("%s\n", load_str);

    // Disk Usage
    DiskStats disk;
    char disk_str[128];
    get_disk_usage("/", &disk);  // root filesystem
    format_disk_usage(disk_str, sizeof(disk_str), &disk);
    printf("%s\n", disk_str);

    // Network Usage
    NetStats net_old, net_new;
    double rx_rate = 0.0, tx_rate = 0.0;
    char net_str[128];

    get_network_stats(&net_old);
    sleep(1);  // wait a second to measure traffic
    get_network_stats(&net_new);

    compute_network_speed(&net_old, &net_new, &rx_rate, &tx_rate);
    format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
    printf("%s\n", net_str);

    return 0;
}
