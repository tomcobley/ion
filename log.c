#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "battery.h"

void state_to_string(state_t state, char *string){
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

void monitor_sleep_time(time_t current_time, battery_t *battery, FILE* analysis_file){
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
  if(number_of_sleeps > 0){
    int average_sleep_time = sum_sleep_time / number_of_sleeps;
    battery->data->average_sleep_time = average_sleep_time;
    printf("Average sleep time = %d in minutes past midnight.\n",average_sleep_time);
    struct tm *current = localtime(&current_time);
    // checks if current time is within 20 minute window
    if(average_sleep_time - (current->tm_hour*60 + current->tm_min) < 30){
   	  battery->data->pre_sleep = true;
    }
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

  monitor_sleep_time(current_time, battery, analysis_file); 

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
