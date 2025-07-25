// include/battery.h

#ifndef BATTERY_H
#define BATTERY_H

typedef struct {
    int capacity;
    char status[32];  // e.g., Charging, Discharging
} BatteryInfo;

int get_battery_info(BatteryInfo *info);
void format_battery_info(char *output, size_t size, const BatteryInfo *info);

#endif
