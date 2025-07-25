#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 1024
#define MAX_CMD_LEN 256

typedef struct {
    int pid;
    char cmd[256];
    double cpu_percent;
    double mem_percent;
    unsigned long last_total_time;   // For CPU %
    unsigned long last_uptime;       // For CPU %
} ProcessInfo;

int get_process_list(ProcessInfo *plist, int max);

#endif
