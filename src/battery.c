// src/battery.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/battery.h"

#define BAT_PATH "/sys/class/power_supply/BAT0/"

static int read_sysfs_value(const char *filename, char *buffer, size_t size) {
    char path[128];
    snprintf(path, sizeof(path), "%s%s", BAT_PATH, filename);

    FILE *fp = fopen(path, "r");
    if (!fp) return 0;

    if (!fgets(buffer, size, fp)) {
        fclose(fp);
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    fclose(fp);
    return 1;
}

int get_battery_info(BatteryInfo *info) {
    char buffer[128];

    if (!read_sysfs_value("capacity", buffer, sizeof(buffer))) return 0;
    info->capacity = atoi(buffer);

    if (!read_sysfs_value("status", buffer, sizeof(buffer))) return 0;
    strncpy(info->status, buffer, sizeof(info->status));

    return 1;
}

void format_battery_info(char *output, size_t size, const BatteryInfo *info) {
    snprintf(output, size, "Battery: %d%% (%s)", info->capacity, info->status);
}
