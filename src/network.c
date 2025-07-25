#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/network.h"

void get_network_stats(NetStats* stats) {
    FILE* fp = fopen("/proc/net/dev", "r");
    if (!fp) return;

    char line[256];
    int count = 0;

    // Skip first two lines
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) && count < MAX_INTERFACES) {
        char name[32];
        unsigned long rx, tx;
        sscanf(line, " %[^:]: %lu %*s %*s %*s %*s %*s %*s %*s %lu",
               name, &rx, &tx);

        strncpy(stats->interfaces[count].name, name, 32);
        stats->interfaces[count].rx_bytes = rx;
        stats->interfaces[count].tx_bytes = tx;
        count++;
    }

    stats->count = count;
    fclose(fp);
}

void compute_network_speed(const NetStats* old_stats, const NetStats* new_stats, double* rx_rate, double* tx_rate) {
    unsigned long rx_total = 0, tx_total = 0;

    for (int i = 0; i < new_stats->count; i++) {
        for (int j = 0; j < old_stats->count; j++) {
            if (strcmp(new_stats->interfaces[i].name, old_stats->interfaces[j].name) == 0) {
                rx_total += (new_stats->interfaces[i].rx_bytes - old_stats->interfaces[j].rx_bytes);
                tx_total += (new_stats->interfaces[i].tx_bytes - old_stats->interfaces[j].tx_bytes);
                break;
            }
        }
    }

    *rx_rate = rx_total / 1024.0;  // KB/s
    *tx_rate = tx_total / 1024.0;  // KB/s
}

void format_network_usage(char* buffer, int buffer_size, double rx_rate, double tx_rate) {
    snprintf(buffer, buffer_size,
             "Net: TX %.2f KB/s   RX %.2f KB/s", tx_rate, rx_rate);
}
