#include "battery.h"

typedef enum system {
  UBUNTU,
  MACOS
} system_t;

void read_battery_info(battery_t *battery, system_t system);


// OS dependent:

static void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo);
static void read_battery_info__macos(battery_t *battery, FILE *batteryinfo);
