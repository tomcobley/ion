#ifndef LOG_H
#define LOG_H
#include <time.h>
#include <stdio.h>
#include "batterystate.h"
#include <stdio.h>
#define DAY_IN_SECONDS (86400)
#define HOUR_IN_SECONDS (3600)

void log_battery_info(battery_t *battery);

void state_to_string(state_t state, char *string);

void monitor_sleep_time(time_t current_time, battery_t *battery, FILE* analysis_file);
#endif
