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
#include "../include/input.h"

void start_ui() {
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(1000);  // Refresh every 1s

    ViewMode current_view = VIEW_ALL;

    CPUStats prev_cpu, curr_cpu;
    read_cpu_stats(&prev_cpu);

    NetStats net_old, net_new;
    get_network_stats(&net_old);

    while (1) {
        clear();

        // Help + header
        const char* view_str =
            current_view == VIEW_ALL ? "All" :
            current_view == VIEW_CPU ? "CPU" :
            current_view == VIEW_MEMORY ? "Memory" :
            current_view == VIEW_NETWORK ? "Network" :
            current_view == VIEW_PROCESSES ? "Processes" : "Unknown";

        mvprintw(0, 2, "[Ubuntu System Monitor]  View: %s (a/h) | Press key: a=All, c=CPU, m=Memory, n=Net, p=Proc | q=Quit", view_str);
        mvhline(1, 0, '=', COLS);

        int line = 3;

        // Refresh CPU/network stats for speed calc
        read_cpu_stats(&curr_cpu);
        get_network_stats(&net_new);

        double cpu_usage = calculate_cpu_usage(&prev_cpu, &curr_cpu);
        prev_cpu = curr_cpu;

        double rx_rate = 0.0, tx_rate = 0.0;
        compute_network_speed(&net_old, &net_new, &rx_rate, &tx_rate);
        net_old = net_new;

        // --- Render based on current view ---
        switch (current_view) {
            case VIEW_ALL:
                // CPU
                mvprintw(line++, 2, "CPU Usage: %.2f%%", cpu_usage);

                // Memory
                MemoryStats mem;
                read_memory_stats(&mem);
                double mem_usage = get_memory_usage_percent(&mem);
                unsigned long used = get_memory_used(&mem);
                mvprintw(line++, 2, "Memory Usage: %.2f%% (%lu MB / %lu MB)", 
                         mem_usage, used / 1024, mem.total / 1024);

                // Uptime
                char uptime_str[64];
                get_uptime_formatted(uptime_str, sizeof(uptime_str));
                mvprintw(line++, 2, "%s", uptime_str);

                // Load avg
                LoadAvg load;
                read_load_avg(&load);
                char load_str[64];
                format_load_avg(load_str, sizeof(load_str), &load);
                mvprintw(line++, 2, "%s", load_str);

                // Disk
                DiskStats disk;
                char disk_str[128];
                get_disk_usage("/", &disk);
                format_disk_usage(disk_str, sizeof(disk_str), &disk);
                mvprintw(line++, 2, "%s", disk_str);

                // Network
                char net_str[128];
                format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
                mvprintw(line++, 2, "%s", net_str);
                break;

            case VIEW_CPU:
                mvprintw(line++, 2, "CPU Usage: %.2f%%", cpu_usage);
                break;

            case VIEW_MEMORY: {
                MemoryStats mem;
                read_memory_stats(&mem);
                double mem_usage = get_memory_usage_percent(&mem);
                unsigned long used = get_memory_used(&mem);
                mvprintw(line++, 2, "Memory Usage: %.2f%% (%lu MB / %lu MB)",
                         mem_usage, used / 1024, mem.total / 1024);
                break;
            }

            case VIEW_NETWORK: {
                char net_str[128];
                format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
                mvprintw(line++, 2, "%s", net_str);
                break;
            }

            case VIEW_PROCESSES:
                mvprintw(line++, 2, "Processes view not implemented yet.");
                break;

            default:
                mvprintw(line++, 2, "Unknown view.");
                break;
        }

        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        ViewMode new_view = handle_input(ch);
        if (new_view != VIEW_NO_CHANGE) {
            current_view = new_view;
        }
    }

    endwin();
}
