#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl.h"
#include "log.h"

static battery_t *alloc_battery(void){
  battery_t *battery = calloc(1, sizeof(battery_t));
  if(battery == NULL){
    perror("Failed to allocate memory for battery");
    exit(EXIT_FAILURE);
  }
  return battery;
}

static void monitor_battery(battery_t *battery) {
  if (battery->percentage <= CYCLE_LOWER_BOUND && battery->state != CHARGING) {
    printf("Current battery level too low, switching plug on.\n");
    post_to_webhook(POWER_ON);
  } else if(battery->percentage >= CYCLE_UPPER_BOUND && battery->state != DISCHARGING) {
    printf("Current battery level too high, switching plug off\n");
    post_to_webhook(POWER_OFF);
  } else {
    printf("Battery within desired charge region, no action required.\n");
  }

}

int main(void) {

  // allocate memory for the battery struct
  battery_t *battery = alloc_battery();

  // determine the running operating system using compiler flags
  op_sys_t op_sys = determine_os();

  // determine battery percentage and state, save information to battery
  read_battery_info(battery, op_sys);

  log_battery_info(battery);

  monitor_battery(battery);

  // free memory assigned to battery struct
  free(battery);

  if (system(DELETE_TEMP_CONTENTS) != SYSTEM_SUCCESS) {
    perror("Failed to delete contents of temp");
  }

  printf("Exit success\n");

  return EXIT_SUCCESS;
}
