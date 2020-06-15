#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "battery.h" // TODO: better fix for circular imports
#include "curl.h"
#include "sys.h"
#include "time.h"

// TODO: improve circular dependency avoidance
//    #include "battery.h"



static battery_t *alloc_battery(void){
  battery_t *battery = calloc(1, sizeof(battery_t));
  if(battery == NULL){
    perror("Failed to allocate memory for battery");
    exit(EXIT_FAILURE);
  }
  return battery;
}

static void state_to_string(state_t state, char *string){
  switch (state) {
  case CHARGING: strncpy(string, "charging", MAX_LINE_SIZE);
    break;
  case DISCHARGING: strncpy(string, "discharging", MAX_LINE_SIZE);
    break;
  default:
    perror("Unable to convert state to string");
    exit(EXIT_FAILURE);
  }

}


static void log_battery_info(battery_t *battery){
  FILE *log_file = fopen(BATTERY_LOG_PATH, "a");
  if(log_file == NULL){
    perror("Failed to open battery logfile");
    exit(EXIT_FAILURE);
  }
  char state_string[MAX_LINE_SIZE];
  state_to_string(battery->state, state_string);

  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  fprintf(log_file, "State: %s, Percentage: %d, Time: %s", state_string, battery->percentage, asctime(timeinfo));

  if(fclose(log_file) != 0){
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }

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

  // char *str123 = malloc(120);
  // state_to_string(battery->state, str123);
  // printf("battery: %s\n", str123);
  // printf("perc: %d\n", battery->percentage);
  // free(str123);

  log_battery_info(battery);

  //
  monitor_battery(battery);

  // free memory assigned to battery struct
  free(battery);

  if (system(DELETE_TEMP_CONTENTS) != SYSTEM_SUCCESS) {
    perror("Failed to delete contents of temp");
  }

  printf("Exit success\n");

  return EXIT_SUCCESS;
}
