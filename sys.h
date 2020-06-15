#include "battery.h"

typedef enum op_sys {
  UBUNTU,
  MACOS
} op_sys_t;

typedef enum state {
  CHARGING,
  DISCHARGING
} state_t;

typedef struct battery {
  int percentage;
  state_t state;
} battery_t;

void read_battery_info(battery_t *battery, op_sys_t op_sys);


// OS dependent:
