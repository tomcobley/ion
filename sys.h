#include "battery.h"

typedef enum op_sys {
  LINUX,
  MACOS
} op_sys_t;

op_sys_t determine_os(void);

void read_battery_info(battery_t *battery, op_sys_t op_sys);
