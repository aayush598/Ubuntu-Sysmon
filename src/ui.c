#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include "../include/cpu.h"
#include "../include/memory.h"
#include "../include/uptime.h"
#include "../include/load.h"
#include "../include/disk.h"
#include "../include/network.h"

void start_ui() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(1000);  // Refresh every 1 second

    // Initial CPU and Network stats
    CPUStats prev_cpu, curr_cpu;
    read_cpu_stats(&prev_cpu);

    NetStats net_old, net_new;
    get_network_stats(&net_old);

    while (1) {
        clear();
        mvprintw(0, 2, "[Ubuntu System Monitor]  Press 'q' to quit");
        mvhline(1, 0, '=', COLS);

        int line = 3;

        // --- CPU Usage ---
        read_cpu_stats(&curr_cpu);
        double cpu_usage = calculate_cpu_usage(&prev_cpu, &curr_cpu);
        mvprintw(line++, 2, "CPU Usage: %.2f%%", cpu_usage);
        prev_cpu = curr_cpu;

        // --- Memory Usage ---
        MemoryStats mem;
        read_memory_stats(&mem);
        double mem_usage_percent = get_memory_usage_percent(&mem);
        unsigned long used = get_memory_used(&mem);
        mvprintw(line++, 2, "Memory Usage: %.2f%% (%lu MB / %lu MB)",
                 mem_usage_percent, used / 1024, mem.total / 1024);

        // --- Uptime ---
        char uptime_str[64];
        get_uptime_formatted(uptime_str, sizeof(uptime_str));
        mvprintw(line++, 2, "%s", uptime_str);

        // --- Load Average ---
        LoadAvg load;
        char load_str[64];
        read_load_avg(&load);
        format_load_avg(load_str, sizeof(load_str), &load);
        mvprintw(line++, 2, "%s", load_str);

        // --- Disk Usage ---
        DiskStats disk;
        char disk_str[128];
        get_disk_usage("/", &disk);
        format_disk_usage(disk_str, sizeof(disk_str), &disk);
        mvprintw(line++, 2, "%s", disk_str);

        // --- Network Stats ---
        get_network_stats(&net_new);
        double rx_rate = 0.0, tx_rate = 0.0;
        char net_str[128];
        compute_network_speed(&net_old, &net_new, &rx_rate, &tx_rate);
        format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
        mvprintw(line++, 2, "%s", net_str);
        net_old = net_new;

        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;
    }

    endwin();
}
