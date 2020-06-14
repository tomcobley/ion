#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "battery.h" // TODO: better fix for circular imports
#include "curl.h"
#include "sys.h"
#include "time.h"
#include "battery.h"
#define SEVEN_DAYS_IN_SECONDS (604800)

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
  
  FILE *analysis_file = fopen(BATTERY_ANALYSIS_PATH, "a+");
  if(analysis_file == NULL){
    perror("Failed to open battery analysis file");
    exit(EXIT_FAILURE);
  }
  
  
  char state_string[MAX_LINE_SIZE];
  // converts enum into string representation
  state_to_string(battery->state, state_string);

  // calculates current time in second past 01/01/1970
  time_t current_time = time(NULL);
  // creates a struct that stores the current time in the form of
  // sec/min/hour/day/month/year since 1900
  struct tm *current = malloc(sizeof(struct tm));
  current = localtime(&current_time);

  fprintf(log_file, "State: %s, Percentage: %d, Time: %s", state_string, battery->percentage, ctime(&current_time));

  // TODO: add field in battery struct for 'pre-sleep' (boolean)

  // rawtime is second since 01/01/1970
  // 7 days = 7 * 24 * 3600 seconds
  //        = 604800 seconds

  // stores time of previous line in csv file
  time_t prev_time = current_time;
  struct tm *prev = malloc(sizeof(struct tm));
  prev = localtime(&prev_time);

  // sum of 7 days previous sleep times stored in minutes past midnight
  int sum_sleep_time = 0;
  char buff[MAX_LINE_SIZE + 1];
  while (fgets(buff, MAX_LINE_SIZE, analysis_file)){
    time_t temp_time = atol(buff);
    struct tm *temp = malloc(sizeof(struct tm));
    temp = localtime(&temp_time);
    // only stores if within 7 days
    //if(temp_time > (current_time - SEVEN_DAYS_IN_SECONDS)){
      // only stores if day is different to prev, this means that prev
      // was the sleep time
      if(prev->tm_mday != temp->tm_mday){
	sum_sleep_time += (prev->tm_hour * 60 + prev->tm_min); 
      }
    prev_time = temp_time;
      //}
    free(temp);
  }
  printf("Average sleep time = %d\n", sum_sleep_time / 7);

  //fprintf(analysis_file, "%ld,%s,%d\n", current_time, state_string, battery->percentage);

  free(current);
  free(prev);
  
  if(fclose(log_file) != 0){
    perror("Failed to close battery log file");
    exit(EXIT_FAILURE);
  }
  
  if(fclose(analysis_file) != 0){
    perror("Failed to close battery analysis file");
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
