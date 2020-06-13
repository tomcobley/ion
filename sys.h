#include "battery.h"

typedef enum op_sys {
  UBUNTU,
  MACOS
} op_sys_t;

void read_battery_info(battery_t *battery, op_sys_t op_sys);

// TODO:
//op_sys_t op_sys determine_os(void);


// OS dependent:
