#include "../include/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str++)) return 0;
    }
    return 1;
}

int get_process_list(ProcessInfo *plist, int max) {
    DIR *proc = opendir("/proc");
    if (!proc) return 0;

    struct dirent *entry;
    int count = 0;

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    long total_mem_kb = sysconf(_SC_PHYS_PAGES) * page_size_kb;

    while ((entry = readdir(proc)) != NULL && count < max) {
        if (!is_number(entry->d_name)) continue;

        int pid = atoi(entry->d_name);
        char stat_path[64], cmd_path[64];
        snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
        snprintf(cmd_path, sizeof(cmd_path), "/proc/%d/cmdline", pid);

        FILE *stat_file = fopen(stat_path, "r");
        if (!stat_file) continue;

        unsigned long utime, stime;
        long rss;
        int scanned = fscanf(stat_file,
            "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u "
            "%lu %lu %*d %*d %*d %*d %*d %*u %*u %ld",
            &utime, &stime, &rss);
        fclose(stat_file);
        if (scanned != 3) continue;

        double cpu_percent = (utime + stime) / (double)sysconf(_SC_CLK_TCK);

        FILE *cmd_file = fopen(cmd_path, "r");
        if (!cmd_file) continue;

        char cmdline[MAX_CMD_LEN];
        fgets(cmdline, sizeof(cmdline), cmd_file);
        fclose(cmd_file);

        if (strlen(cmdline) == 0) {
            snprintf(cmdline, sizeof(cmdline), "[%d]", pid);
        }

        strncpy(plist[count].cmd, cmdline, MAX_CMD_LEN);
        plist[count].pid = pid;
        plist[count].cpu_percent = cpu_percent;
        plist[count].mem_percent = (rss * page_size_kb) * 100.0 / total_mem_kb;

        count++;
    }

    closedir(proc);
    return count;
}
