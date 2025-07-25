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
#include "../include/theme.h"
#include "../include/process.h"

void start_ui() {
    setlocale(LC_ALL, "");

    initscr();
    init_theme();
    noecho();
    cbreak();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(1000);  // Refresh every 1s

    int scroll_offset = 0;

    ViewMode current_view = VIEW_ALL;

    CPUStats prev_cpu, curr_cpu;
    read_cpu_stats(&prev_cpu);

    NetStats net_old, net_new;
    get_network_stats(&net_old);

    while (1) {
        clear();

        // Help + Header
        const char* view_str =
            current_view == VIEW_ALL ? "All" :
            current_view == VIEW_CPU ? "CPU" :
            current_view == VIEW_MEMORY ? "Memory" :
            current_view == VIEW_NETWORK ? "Network" :
            current_view == VIEW_PROCESSES ? "Processes" : "Unknown";

        apply_color_title();
        mvprintw(0, 2, "[Ubuntu System Monitor]  View: %s  | Press: a=All, c=CPU, m=Memory, n=Net, p=Proc, q=Quit", view_str);
        reset_color();
        mvhline(1, 0, '=', COLS);

        int line = 3;

        // Refresh stats
        read_cpu_stats(&curr_cpu);
        get_network_stats(&net_new);

        double cpu_usage = calculate_cpu_usage(&prev_cpu, &curr_cpu);
        prev_cpu = curr_cpu;

        double rx_rate = 0.0, tx_rate = 0.0;
        compute_network_speed(&net_old, &net_new, &rx_rate, &tx_rate);
        net_old = net_new;

        switch (current_view) {
            case VIEW_ALL: {
                // --- CPU ---
                apply_color_label();
                mvprintw(line, 2, "CPU Usage:");
                reset_color();
                apply_color_value();
                printw(" %.2f%%", cpu_usage);
                reset_color();
                line++;

                // --- Memory ---
                MemoryStats mem;
                read_memory_stats(&mem);
                double mem_usage = get_memory_usage_percent(&mem);
                unsigned long used = get_memory_used(&mem);
                apply_color_label();
                mvprintw(line, 2, "Memory Usage:");
                reset_color();
                apply_color_value();
                printw(" %.2f%% (%lu MB / %lu MB)", mem_usage, used / 1024, mem.total / 1024);
                reset_color();
                line++;

                // --- Uptime ---
                char uptime_str[64];
                get_uptime_formatted(uptime_str, sizeof(uptime_str));
                apply_color_label();
                mvprintw(line++, 2, "Uptime: ");
                reset_color();
                apply_color_value();
                printw("%s", uptime_str);
                reset_color();

                // --- Load Average ---
                LoadAvg load;
                read_load_avg(&load);
                char load_str[64];
                format_load_avg(load_str, sizeof(load_str), &load);
                apply_color_label();
                mvprintw(line++, 2, "%s", load_str);
                reset_color();

                // --- Disk ---
                DiskStats disk;
                char disk_str[128];
                get_disk_usage("/", &disk);
                format_disk_usage(disk_str, sizeof(disk_str), &disk);
                apply_color_label();
                mvprintw(line++, 2, "%s", disk_str);
                reset_color();

                // --- Network ---
                char net_str[128];
                format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
                apply_color_label();
                mvprintw(line++, 2, "%s", net_str);
                reset_color();

                break;
            }

            case VIEW_CPU:
                apply_color_label();
                mvprintw(line, 2, "CPU Usage:");
                reset_color();
                apply_color_value();
                printw(" %.2f%%", cpu_usage);
                reset_color();
                line++;
                break;

            case VIEW_MEMORY: {
                MemoryStats mem;
                read_memory_stats(&mem);
                double mem_usage = get_memory_usage_percent(&mem);
                unsigned long used = get_memory_used(&mem);
                apply_color_label();
                mvprintw(line, 2, "Memory Usage:");
                reset_color();
                apply_color_value();
                printw(" %.2f%% (%lu MB / %lu MB)", mem_usage, used / 1024, mem.total / 1024);
                reset_color();
                line++;
                break;
            }

            case VIEW_NETWORK: {
                char net_str[128];
                format_network_usage(net_str, sizeof(net_str), rx_rate, tx_rate);
                apply_color_label();
                mvprintw(line++, 2, "%s", net_str);
                reset_color();
                break;
            }

            case VIEW_PROCESSES: {
                ProcessInfo plist[MAX_PROCESSES];
                int count = get_process_list(plist, MAX_PROCESSES);

                int visible_lines = LINES - line - 2;  // leave room for footer
                if (visible_lines > 20) visible_lines = 20;

                if (scroll_offset > count - visible_lines) scroll_offset = count - visible_lines;
                if (scroll_offset < 0) scroll_offset = 0;

                attron(A_BOLD | COLOR_PAIR(3));
                mvprintw(line++, 2, " %-6s %-8s %-8s %s", "PID", "CPU%", "MEM%", "Command");
                attroff(A_BOLD | COLOR_PAIR(3));

                for (int i = scroll_offset; i < count && i < scroll_offset + visible_lines; i++) {
                    mvprintw(line++, 2, " %-6d %-8.2f %-8.2f %.40s",
                            plist[i].pid, plist[i].cpu_percent, plist[i].mem_percent, plist[i].cmd);
                }

                mvprintw(LINES - 1, 2, "[↑/↓ to scroll, q=quit, a=All, c=CPU, m=Mem, n=Net, p=Proc]");
                break;
            }


            default:
                apply_color_label();
                mvprintw(line++, 2, "Unknown view.");
                reset_color();
                break;
        }

        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        ViewMode new_view = handle_input(ch);
        if (new_view != VIEW_NO_CHANGE) {
            current_view = new_view;
        }

        // Pagination keys
        if (current_view == VIEW_PROCESSES) {
            if (ch == KEY_UP) {
                scroll_offset--;
            } else if (ch == KEY_DOWN) {
                scroll_offset++;
            }
        }
    }

    endwin();
}
