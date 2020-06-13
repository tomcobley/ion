#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sys.h"


static void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo);
//static void read_battery_info__macos(battery_t *battery, FILE *batteryinfo);


void read_battery_info(battery_t *battery, op_sys_t op_sys) {

  // TODO: execute command based on operating system
  if(system(BATTERY_INFO__UBUNTU) != SYSTEM_SUCCESS){
    perror("Failed to execute upower command");
    exit(EXIT_FAILURE);
  }

  FILE *batteryinfo = fopen(BATTERY_INFO_PATH, "r");
  if(batteryinfo == NULL){
    perror("Failed to read battery information file");
    exit(EXIT_FAILURE);
  }

  // TODO: check for EMPTY file (in case of no access to battery, test on lab machines)



  read_battery_info__ubuntu(battery, batteryinfo);


  if(fclose(batteryinfo) != 0){
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }
}

static void update_battery(battery_t *battery, state_t state, int percentage) {
  // TODO: handle errors properly
  assert(percentage >= 0 && percentage <= 100);
  assert(state == CHARGING || state == DISCHARGING);
  battery->state = state;
  battery->percentage = percentage;
}

/////////////////////////       UBUNTU        /////////////////////////

static void init_battery__ubuntu(battery_t *battery, char *status, int percentage){
  if (strcmp(status, "fully-charged") == 0 || strcmp(status, "charging") == 0) {
    update_battery(battery, CHARGING, percentage);
  } else if (strcmp(status, "discharging") == 0) {
    update_battery(battery, DISCHARGING, percentage);
  } else {
    // TODO: handle error properly
    perror("Could not read battery_status");
    exit(EXIT_FAILURE);
  }
}

static char *skipWhiteSpace(char *str) {
  // TODO: remove unneccessary ptr variable?
  char *ptr = str;
  while (*ptr == ' ') { ptr++; }
  return ptr;
}

static void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo) {
  char buff[MAX_LINE_SIZE + 1];
  char status[MAX_LINE_SIZE + 1];
  int percentage;

  while (fgets(buff, MAX_LINE_SIZE, batteryinfo)) {
    // get ptr to label of this line
    char *label_ptr = skipWhiteSpace(buff);
    strtok(label_ptr, ":");

    // get following token (the information) up to the newline character
    char *info_ptr;
    info_ptr = strtok(NULL, "\n");
    info_ptr = skipWhiteSpace(info_ptr);

    if(strcmp(label_ptr, "state") == 0){
      strcpy(status, info_ptr);
    } else {
      percentage = strtol(info_ptr, NULL, 10);
    }
  }

  init_battery__ubuntu(battery, status, percentage);

}



/////////////////////////       macOS        /////////////////////////


// TODO: implement read_battery_info__macos
