#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "config.h"

battery_t *alloc_battery(void) {
  battery_t *battery = calloc(1, sizeof(battery_t));
  if (battery == NULL) {
    perror("Failed to allocate memory for battery");
    exit(EXIT_FAILURE);
  }
  battery->data = calloc(1, sizeof(battery_data_t));
  return battery;
}

void free_battery(battery_t *battery) {
  free(battery->data);
  free(battery);
}

#ifndef DEBUG


static void monitor_battery(battery_t *battery, config_t *config) {
  if (battery->percentage <= config->int_cycle_min_charge_percentage) {
    if (battery->state == DISCHARGING) {
      printf("Current battery level below lower bound and device is discharging, so switching plug on.\n");
      post_to_webhook(config->str_charge_low_webhook_url);
    } else {
      printf("Current battery level below lower bound but device is charging, so no action required.\n");
    }
  } else if (battery->percentage >= config->int_cycle_max_charge_percentage) {
    if (battery->state == CHARGING && !battery->data->pre_sleep) {
      printf("Current battery level too high and device is charging, so switching plug off\n");
      post_to_webhook(config->str_charge_high_webhook_url);
    } else if (battery->state == CHARGING && battery->data->pre_sleep) {
      printf("Current battery level high and device is charging, but pre_sleep detected so no action taken\n");
    } else {
      printf("Current battery level above upper bound but device is discharging, so no action required.\n");
    }
  } else {
    printf("Battery within desired charge region, no action required.\n");
  }
}

int main(int argc, char const *argv[]) {

  // determine the host operating system
  op_sys_t op_sys = determine_os();

  // determine root of codebase, used for accessing files in an absolute manner
  //    regardless of working directory / location of codebase
  determine_root_path(op_sys);

  if (argc > 1 && strcmp(argv[1], "init") == 0) {
    // call init method from config.c
    init();
    exit(EXIT_SUCCESS);
  }

  // allocate memory for config (settings) struct
  config_t *config = alloc_config();

  // read settings from config file and populate config
  FILE *config_file = open_config_file("r");
  if (!config_file || !read_config(config_file, config)) {
    // failed to populate config from config_file. Either config_file doesn't
    //    exist or is invalid. notify user and exit
    fprintf(stderr,
            "No valid .config file found. Please run '%s' to configure program. \n",
            INIT_COMMAND
    );
    exit(EXIT_FAILURE);
  }

  // configuration was read successfully and saved to config, so close config_file
  fclose(config_file);

  // allocate memory for the battery struct
  battery_t *battery = alloc_battery();


  // determine battery percentage and state, save information to battery
  read_battery_info(battery, op_sys);

  printf("\nCurrent state-of-charge: %d %% \n", battery->percentage);

  // log battery percentage and state for sleep time analysis
  log_battery_info(battery);

  // perform post to webhook depending on state of battery and configuration settings
  monitor_battery(battery, config);

  // free memory assigned to battery struct
  free_battery(battery);

  if (system(DELETE_TEMP_CONTENTS) != SYSTEM_SUCCESS) {
    perror("Failed to delete contents of temp");
  }

  printf("\nExit Success.\n");

  return EXIT_SUCCESS;
}

#endif
