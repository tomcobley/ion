#include <stdio.h>
#include "batterystate.h"

typedef enum op_sys {
  LINUX,
  MACOS
} op_sys_t;


op_sys_t determine_os(void);

void read_battery_info(battery_t *battery, op_sys_t op_sys);

void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo);

void read_battery_info__macos(battery_t *battery, FILE *batteryinfo);
