#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

#include "../include/process.h"

#define BUFFER_SIZE 1024

int get_process_list(ProcessInfo *plist, int max) {
    DIR *dir = opendir("/proc");
    if (!dir) return 0;

    FILE *fp;
    struct dirent *entry;
    int count = 0;

    long clk_tck = sysconf(_SC_CLK_TCK);
    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;

    // Get total system uptime
    double sys_uptime = 0.0;
    if ((fp = fopen("/proc/uptime", "r")) != NULL) {
        fscanf(fp, "%lf", &sys_uptime);
        fclose(fp);
    }

    // Get total memory
    unsigned long total_mem_kb = 0;
    if ((fp = fopen("/proc/meminfo", "r")) != NULL) {
        char label[64];
        while (fscanf(fp, "%63s %lu", label, &total_mem_kb) == 2) {
            if (strcmp(label, "MemTotal:") == 0) break;
        }
        fclose(fp);
    }

    while ((entry = readdir(dir)) != NULL && count < max) {
        if (!isdigit(entry->d_name[0])) continue;

        int pid = atoi(entry->d_name);
        char stat_path[256], cmd_path[256], line[BUFFER_SIZE];
        snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
        snprintf(cmd_path, sizeof(cmd_path), "/proc/%d/cmdline", pid);

        FILE *stat = fopen(stat_path, "r");
        if (!stat) continue;

        unsigned long utime, stime, starttime;
        char comm[256], state;
        int dummy;

        fscanf(stat, "%d %s %c", &dummy, comm, &state);
        for (int i = 0; i < 10; i++) fscanf(stat, "%*s"); // skip fields 4–13
        fscanf(stat, "%lu %lu", &utime, &stime);          // 14,15
        for (int i = 0; i < 6; i++) fscanf(stat, "%*s");  // skip to 22
        fscanf(stat, "%lu", &starttime);                  // 22
        fclose(stat);

        // Read memory info (RSS)
        char statm_path[256];
        snprintf(statm_path, sizeof(statm_path), "/proc/%d/statm", pid);
        FILE *statm = fopen(statm_path, "r");
        if (!statm) continue;
        unsigned long total_pages, resident_pages;
        fscanf(statm, "%lu %lu", &total_pages, &resident_pages);
        fclose(statm);

        // Get command
        FILE *cmd = fopen(cmd_path, "r");
        if (!cmd) continue;
        fgets(line, sizeof(line), cmd);
        fclose(cmd);

        if (strlen(line) == 0)
            snprintf(line, sizeof(line), "[%s]", comm);  // fallback if cmdline empty

        // CPU calculation
        unsigned long total_time = utime + stime;
        double seconds = sys_uptime - ((double)starttime / clk_tck);
        double cpu_percent = 0.0;
        if (seconds > 0)
            cpu_percent = 100.0 * ((double)total_time / clk_tck) / seconds;

        // Memory calculation
        double mem_percent = 100.0 * (resident_pages * page_size_kb) / total_mem_kb;

        // Store process info
        ProcessInfo *p = &plist[count++];
        p->pid = pid;
        strncpy(p->cmd, line, sizeof(p->cmd));
        p->cmd[sizeof(p->cmd) - 1] = '\0'; // null-terminate
        p->cpu_percent = cpu_percent;
        p->mem_percent = mem_percent;
    }

    closedir(dir);
    return count;
}

// ✅ Matches search query (PID or substring of command)
int matches_filter(ProcessInfo *proc, const char *query) {
    if (!query || strlen(query) == 0) return 1;

    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", proc->pid);

    return (strstr(proc->cmd, query) != NULL || strstr(pid_str, query) != NULL);
}
