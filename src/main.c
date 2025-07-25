#include <stdio.h>
#include <unistd.h>
#include "../include/cpu.h"

int main() {
    CPUStats prev, curr;

    read_cpu_stats(&prev);
    sleep(1);
    read_cpu_stats(&curr);

    double cpu_usage = calculate_cpu_usage(&prev, &curr);
    printf("CPU Usage: %.2f%%\n", cpu_usage);

    return 0;
}
