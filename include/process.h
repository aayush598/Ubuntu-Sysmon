#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 1024
#define MAX_CMD_LEN 256

typedef struct {
    int pid;
    char cmd[MAX_CMD_LEN];
    double cpu_percent;
    double mem_percent;
} ProcessInfo;

int get_process_list(ProcessInfo *plist, int max);

#endif
