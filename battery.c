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
  }

}

int main(void) {

  battery_t *battery = alloc_battery();

  // TODO: make dependent on OS
  read_battery_info(battery, UBUNTU);

  log_battery_info(battery);

  monitor_battery(battery);

  free(battery);

  if (system(DELETE_TEMP_CONTENTS) != SYSTEM_SUCCESS) {
    perror("Failed to delete contents of temp");
  }

  return EXIT_SUCCESS;
}
