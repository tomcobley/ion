#ifndef BATTERYSTATE_H
#define BATTERYSTATE_H

typedef enum state {
  CHARGING,
  DISCHARGING
} state_t;

typedef struct battery {
  int percentage;
  state_t state;
} battery_t;

#endif
