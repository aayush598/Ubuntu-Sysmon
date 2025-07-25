#include <stdio.h>
#include <sys/statvfs.h>
#include "../include/disk.h"

void get_disk_usage(const char* path, DiskStats* stats) {
    struct statvfs vfs;

    if (statvfs(path, &vfs) != 0) {
        stats->total = stats->used = stats->free = 0;
        stats->usage_percent = 0.0;
        return;
    }

    unsigned long block_size = vfs.f_frsize;
    unsigned long total_blocks = vfs.f_blocks;
    unsigned long free_blocks = vfs.f_bavail;
    unsigned long used_blocks = total_blocks - free_blocks;

    stats->total = total_blocks * block_size;
    stats->free = free_blocks * block_size;
    stats->used = used_blocks * block_size;
    stats->usage_percent = (double)stats->used / stats->total * 100.0;
}

void format_disk_usage(char* buffer, int buffer_size, const DiskStats* stats) {
    snprintf(buffer, buffer_size,
             "Disk: Used %.2f GB / %.2f GB (%.1f%%)",
             stats->used / (1024.0 * 1024 * 1024),
             stats->total / (1024.0 * 1024 * 1024),
             stats->usage_percent);
}
