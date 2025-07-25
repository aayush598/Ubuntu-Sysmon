#ifndef NETWORK_H
#define NETWORK_H

#define MAX_INTERFACES 10

typedef struct {
    char name[32];
    unsigned long rx_bytes;
    unsigned long tx_bytes;
} NetInterface;

typedef struct {
    NetInterface interfaces[MAX_INTERFACES];
    int count;
} NetStats;

typedef struct {
    char protocol[4];  // "TCP"/"UDP"
    char local_address[128];
    char remote_address[128];
    char state[32];  // For TCP only
} NetConnection;

void get_network_stats(NetStats* stats);
void compute_network_speed(const NetStats* old_stats, const NetStats* new_stats, double* rx_rate, double* tx_rate);
void format_network_usage(char* buffer, int buffer_size, double rx_rate, double tx_rate);

int get_network_connections(NetConnection *conns, int max_conns);
void format_network_connections(char *buffer, int buffer_size, const NetConnection *conns, int count);

#endif
