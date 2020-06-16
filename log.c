#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "log.h"

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

void log_battery_info(battery_t *battery){
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

  fprintf(log_file, "State: %s, Percentage: %d, Time: %s", state_string, battery->percentage, ctime(&current_time));


  // rawtime is seconds since 01/01/1970
  // 1 day = 24 * 3600 seconds
  //       = 86400 seconds

  // stores time of previous line in csv file
  time_t prev_time = current_time;

  // sum of 7 days previous sleep times stored in minutes past midnight
  int sum_sleep_time = 0;
  int number_of_sleeps = 0;
  char buff[MAX_LINE_SIZE + 1];
  while (fgets(buff, MAX_LINE_SIZE, analysis_file)){
    time_t temp_time = atol(buff);
    // only stores if within 7 days
    if(temp_time > (current_time - 7 * DAY_IN_SECONDS)){
      // only stores if there is a difference in 4 hours to prev
      // this means that prev was the sleep time
      if(prev_time < (temp_time - 4 * HOUR_IN_SECONDS)){
	struct tm *prev = localtime(&prev_time);
	// converts prev to minutes past midnight
	sum_sleep_time += prev->tm_hour * 60 + prev->tm_min; 
	number_of_sleeps ++;
      }
    prev_time = temp_time;
    }
  }
  int average_number_of_sleeps = sum_sleep_time / number_of_sleeps;
  if(number_of_sleeps > 0){
    printf("Average sleep time = %d in minutes past midnight.\n", average_number_of_sleeps);
  }
  /*
   * TODO: add field in battery struct for 'pre-sleep' (boolean)
   * struct tm *current = localtime(&current_time);
   * // checks if current time is within 20 minute window
   * if(average_number_of_sleeps - (current->tm_hour*60 + current->tm_min) < 20){
   *	battery->pre_sleep_time = true;
   * }
   */

  fprintf(analysis_file, "%ld,%s,%d\n", current_time, state_string, battery->percentage);
  
  if(fclose(log_file) != 0){
    perror("Failed to close battery log file");
    exit(EXIT_FAILURE);
  }
  
  if(fclose(analysis_file) != 0){
    perror("Failed to close battery analysis file");
    exit(EXIT_FAILURE);
  }
}
