#ifndef DISK_H
#define DISK_H

typedef struct {
    unsigned long total;
    unsigned long used;
    unsigned long free;
    double usage_percent;
} DiskStats;

typedef struct {
    unsigned long read_sectors;
    unsigned long write_sectors;
    double read_rate_kb;
    double write_rate_kb;
} DiskIOStats;

void get_disk_usage(const char* path, DiskStats* stats);
void format_disk_usage(char* buffer, int buffer_size, const DiskStats* stats);

void get_disk_io_stats(DiskIOStats *stats);
void compute_disk_io_rate(const DiskIOStats *old, const DiskIOStats *new, DiskIOStats *rate, double elapsed_sec);

void get_disk_io_rates(double *read_rate, double *write_rate);  // <-- Add this

#endif
