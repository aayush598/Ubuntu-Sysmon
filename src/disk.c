#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/statvfs.h>
#include "../include/disk.h"

#define SECTOR_SIZE 512  // most Linux systems use 512-byte sectors

static DiskIOStats prev_stats = {0};
static struct timespec prev_time = {0};

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

void get_disk_io_stats(DiskIOStats *stats) {
    FILE *fp = fopen("/proc/diskstats", "r");
    if (!fp) return;

    char line[256];
    unsigned long read_sec = 0, write_sec = 0;

    while (fgets(line, sizeof(line), fp)) {
        char dev[32];
        unsigned long rd_ios, rd_merges, rd_sec, rd_ticks;
        unsigned long wr_ios, wr_merges, wr_sec, wr_ticks;
        int major, minor;

        int parsed = sscanf(line, "%d %d %s %lu %lu %lu %lu %lu %lu %lu %lu",
            &major, &minor, dev,
            &rd_ios, &rd_merges, &rd_sec, &rd_ticks,
            &wr_ios, &wr_merges, &wr_sec, &wr_ticks);

        // Filter physical devices only (e.g., "sda", "nvme0n1")
        if (parsed >= 11 && (strncmp(dev, "sd", 2) == 0 || strncmp(dev, "nvme", 4) == 0)) {
            read_sec += rd_sec;
            write_sec += wr_sec;
        }
    }

    fclose(fp);
    stats->read_sectors = read_sec;
    stats->write_sectors = write_sec;
}

void compute_disk_io_rate(const DiskIOStats *old, const DiskIOStats *new, DiskIOStats *rate, double elapsed_sec) {
    unsigned long delta_read = new->read_sectors - old->read_sectors;
    unsigned long delta_write = new->write_sectors - old->write_sectors;

    rate->read_rate_kb = (delta_read * SECTOR_SIZE) / 1024.0 / elapsed_sec;
    rate->write_rate_kb = (delta_write * SECTOR_SIZE) / 1024.0 / elapsed_sec;
}


void get_disk_io_rates(double *read_rate, double *write_rate) {
    DiskIOStats current_stats, rate_stats;
    struct timespec current_time;

    clock_gettime(CLOCK_MONOTONIC, &current_time);
    get_disk_io_stats(&current_stats);

    double elapsed = (current_time.tv_sec - prev_time.tv_sec) +
                     (current_time.tv_nsec - prev_time.tv_nsec) / 1e9;

    if (elapsed > 0.0) {
        compute_disk_io_rate(&prev_stats, &current_stats, &rate_stats, elapsed);
        *read_rate = rate_stats.read_rate_kb;
        *write_rate = rate_stats.write_rate_kb;
    } else {
        *read_rate = *write_rate = 0.0;
    }

    prev_stats = current_stats;
    prev_time = current_time;
}
