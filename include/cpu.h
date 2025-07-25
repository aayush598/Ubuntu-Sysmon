#ifndef CPU_H
#define CPU_H

typedef struct {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
} CPUStats;

double calculate_cpu_usage(const CPUStats* prev, const CPUStats* curr);
void read_cpu_stats(CPUStats* stats);

#endif
