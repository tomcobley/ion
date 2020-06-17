#include <stdbool.h>

#ifndef BATTERYSTATE_H
#define BATTERYSTATE_H

typedef enum state {
    CHARGING,
    DISCHARGING
} state_t;

typedef struct battery_data {
    int average_sleep_time;
    bool pre_sleep;
} battery_data_t;

typedef struct battery {
    int percentage;
    state_t state;
    battery_data_t *data;
} battery_t;

#endif
