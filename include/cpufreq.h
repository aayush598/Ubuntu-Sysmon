// include/cpufreq.h
#ifndef CPUFREQ_H
#define CPUFREQ_H

#define MAX_CORES 128

int get_cpu_freqs_khz(int freqs_khz[], int max_cores);

#endif
