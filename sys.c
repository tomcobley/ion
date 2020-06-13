#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "sys.h"


static void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo);
static void read_battery_info__macos(battery_t *battery, FILE *batteryinfo);


void read_battery_info(battery_t *battery, op_sys_t op_sys) {

  int info_status;

  if (op_sys == UBUNTU) {
    info_status = system(BATTERY_INFO__UBUNTU);
  } else if (op_sys == MACOS) {
    info_status = system(BATTERY_INFO__MACOS);
  } else {
    // TODO: error
    assert(0);
  }

  // TODO: execute command based on operating system
  if(info_status != SYSTEM_SUCCESS){
    perror("Failed to execute upower command");
    exit(EXIT_FAILURE);
  }

  FILE *batteryinfo = fopen(BATTERY_INFO_PATH, "r");
  if(batteryinfo == NULL){
    perror("Failed to read battery information file");
    exit(EXIT_FAILURE);
  }

  // TODO: check for EMPTY file (in case of no access to battery, test on lab machines)

  // TODO: use function pointer instead
  if (op_sys == UBUNTU) {
    read_battery_info__ubuntu(battery, batteryinfo);
  } else if (op_sys == MACOS) {
    read_battery_info__macos(battery, batteryinfo);
  }

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


static char *skip_white_space(char *str) {
  // TODO: remove unneccessary ptr variable?
  char *ptr = str;
  while (*ptr == ' ') { ptr++; }
  return ptr;
}


/////////////////////////       UBUNTU        /////////////////////////



static void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo) {
  char buff[MAX_LINE_SIZE + 1];
  char status[MAX_LINE_SIZE + 1];
  int percentage = -1;

  // TODO: does this need to be a while loop?
  while (fgets(buff, MAX_LINE_SIZE, batteryinfo)) {
    // get ptr to label of this line
    char *label_ptr = skip_white_space(buff);
    strtok(label_ptr, ":");

    // get following token (the information) up to the newline character
    char *info_ptr;
    info_ptr = strtok(NULL, "\n");
    info_ptr = skip_white_space(info_ptr);

    if(strcmp(label_ptr, "state") == 0){
      strcpy(status, info_ptr);
    } else {
      percentage = strtol(info_ptr, NULL, 10);
    }
  }

  if (percentage == -1) {
    // TODO: handle error
  }

  // initiliase battery struct using information from system
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



/////////////////////////       macOS        /////////////////////////




static void read_battery_info__macos(battery_t *battery, FILE *batteryinfo) {
  char buff[MAX_LINE_SIZE + 1];
  int percentage = -1;
  // TODO: change size of buffer for status to be lower
  char status[MAX_LINE_SIZE + 1];

  // disregard first line of file, store second line (which will contain
  //    the relevant data into buff
  fgets(buff, MAX_LINE_SIZE, batteryinfo);
  fgets(buff, MAX_LINE_SIZE, batteryinfo);

  char *str_ptr = buff;
  for (; *str_ptr; str_ptr++) {
    if (*str_ptr == '%') {
      // % symbol reached, so read the 3 preceding non-whitespace chars
      //    as the charge percentage of the battery
      percentage = strtol(skip_white_space(str_ptr - 3), NULL, 10);
      break;
    }
  }

  if (percentage == -1) {
    // TODO: handle error
  }

  // (strptr + 2) represents start of state field, which contains leading
  //    whitespace and is terminated by a ';'
  str_ptr += 2;
  strtok(str_ptr, ";");
  strcpy(status, skip_white_space(str_ptr));

  // initiliase battery struct using information from system
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
