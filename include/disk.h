#ifndef DISK_H
#define DISK_H

typedef struct {
    unsigned long total;
    unsigned long used;
    unsigned long free;
    double usage_percent;
} DiskStats;

void get_disk_usage(const char* path, DiskStats* stats);
void format_disk_usage(char* buffer, int buffer_size, const DiskStats* stats);

#endif
