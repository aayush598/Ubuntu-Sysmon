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

void hex_to_ip_port(const char *hex, char *ip, unsigned short *port) {
    unsigned int ip_part[4], port_part;
    sscanf(hex, "%8X:%4X", (unsigned int *)&ip_part[3], &port_part);

    ip_part[0] = ip_part[3] & 0xFF;
    ip_part[1] = (ip_part[3] >> 8) & 0xFF;
    ip_part[2] = (ip_part[3] >> 16) & 0xFF;
    ip_part[3] = (ip_part[3] >> 24) & 0xFF;

    sprintf(ip, "%u.%u.%u.%u", ip_part[0], ip_part[1], ip_part[2], ip_part[3]);
    *port = port_part;
}

const char *tcp_state(const char *hex_state) {
    if (strcmp(hex_state, "01") == 0) return "ESTABLISHED";
    if (strcmp(hex_state, "02") == 0) return "SYN_SENT";
    if (strcmp(hex_state, "03") == 0) return "SYN_RECV";
    if (strcmp(hex_state, "04") == 0) return "FIN_WAIT1";
    if (strcmp(hex_state, "05") == 0) return "FIN_WAIT2";
    if (strcmp(hex_state, "06") == 0) return "TIME_WAIT";
    if (strcmp(hex_state, "07") == 0) return "CLOSE";
    if (strcmp(hex_state, "08") == 0) return "CLOSE_WAIT";
    if (strcmp(hex_state, "09") == 0) return "LAST_ACK";
    if (strcmp(hex_state, "0A") == 0) return "LISTEN";
    if (strcmp(hex_state, "0B") == 0) return "CLOSING";
    return "UNKNOWN";
}

int parse_proc_net(const char *path, const char *proto, NetConnection *conns, int max) {
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    char line[512];
    int count = 0;

    fgets(line, sizeof(line), fp); // Skip header

    while (fgets(line, sizeof(line), fp) && count < max) {
        char local[64], remote[64], state[8];
        int num;
        sscanf(line, "%d: %64[0-9A-Fa-f]:%*x %64[0-9A-Fa-f]:%*x %2s", &num, local, remote, state);

        char lip[64], rip[64];
        unsigned short lport, rport;
        hex_to_ip_port(local, lip, &lport);
        hex_to_ip_port(remote, rip, &rport);

        snprintf(conns[count].protocol, sizeof(conns[count].protocol), "%s", proto);
        snprintf(conns[count].local_address, sizeof(conns[count].local_address), "%s:%u", lip, lport);
        snprintf(conns[count].remote_address, sizeof(conns[count].remote_address), "%s:%u", rip, rport);
        snprintf(conns[count].state, sizeof(conns[count].state), "%s", strcmp(proto, "TCP") == 0 ? tcp_state(state) : "");

        count++;
    }

    fclose(fp);
    return count;
}

int get_network_connections(NetConnection *conns, int max_conns) {
    int total = 0;
    total += parse_proc_net("/proc/net/tcp", "TCP", conns + total, max_conns - total);
    total += parse_proc_net("/proc/net/udp", "UDP", conns + total, max_conns - total);
    return total;
}

void format_network_connections(char *buffer, int buffer_size, const NetConnection *conns, int count) {
    int offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "Proto     Local Address        Remote Address       State\n");

    for (int i = 0; i < count && offset < buffer_size; i++) {
        offset += snprintf(buffer + offset, buffer_size - offset,
            "%-8s %-20s %-20s %-15s\n",
            conns[i].protocol,
            conns[i].local_address,
            conns[i].remote_address,
            conns[i].state[0] ? conns[i].state : "-");
    }
}
