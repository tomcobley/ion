#ifndef LOG_H
#define LOG_H

#include "batterystate.h"
#include <stdio.h>
#define DAY_IN_SECONDS (86400)
#define HOUR_IN_SECONDS (3600)

void log_battery_info(battery_t *battery, FILE* log_file, FILE* analysis_file);

void state_to_string(state_t state, char *string);

#endif
