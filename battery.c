#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl.h"
#include "time.h"

#define SYSTEM_SUCCESS (0)
#define MAX_LINE_SIZE (1024)
#define CYCLE_LOWER_BOUND (30)
#define CYCLE_UPPER_BOUND (80)
#define BATTERY_INFO ("upower -i `upower -e | grep 'BAT'` | egrep 'state|percentage' > ./temp/batteryinfo.txt")
#define DELETE_TEMP_CONTENTS ("cd ./temp && rm -rf *")
#define BATTERY_INFO_PATH ("./temp/batteryinfo.txt")
#define BATTERY_LOG_PATH ("./log/batterylog.txt")
#define POWER_ON ("https://maker.ifttt.com/trigger/charge_low/with/key/dMgkSNQqbJM-9NI1ySrKdb")
#define POWER_OFF ("https://maker.ifttt.com/trigger/charge_high/with/key/dMgkSNQqbJM-9NI1ySrKdb")

typedef enum state {
  CHARGING,
  DISCHARGING
} state_t;

typedef struct battery {
  int percentage;
  state_t state;
} battery_t;

static battery_t *new_battery(void){
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

static void init_battery(battery_t *battery, char *status, int percentage){
  if (strcmp(status, "fully-charged") == 0 || strcmp(status, "charging") == 0) {
    battery->state = CHARGING;
  } else if (strcmp(status, "discharging") == 0) {
    battery->state = DISCHARGING;
  } else {
    perror("Could not read battery_status");
    exit(EXIT_FAILURE);
  }
  battery->percentage = percentage;
}

static void log_battery_info(battery_t *battery){
  FILE *log_file = fopen(BATTERY_LOG_PATH, "a");
  if(log_file == NULL){
    perror("Failed to open battery log file");
    exit(EXIT_FAILURE);
  }
  char state_string[MAX_LINE_SIZE];
  state_to_string(battery->state, state_string);
  
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  
  fprintf(log_file, "Time: %s , State: %s, Percentage: %d\n", asctime(timeinfo) , state_string, battery->percentage);

  if(fclose(log_file) != 0){
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }
  
}

static void monitor_battery(battery_t *battery) {
  if (battery->percentage <= CYCLE_LOWER_BOUND) {
    printf("Current battery level too low, switching plug on.\n");
    post_to_webhook(POWER_ON);
  } else if(battery->percentage >= CYCLE_UPPER_BOUND) {
    printf("Current battery level too high, switching plug off\n");
    post_to_webhook(POWER_OFF);
  }
  
}

static char *skipWhiteSpace(char *str) {
  char *ptr = str;
  while (*ptr == ' ') {
    ptr++;
  }
  return ptr;
}

static void read_battery_info(battery_t *battery) {

  if(system(BATTERY_INFO) != SYSTEM_SUCCESS){
    perror("Failed to execute upower command");
    exit(EXIT_FAILURE);
  }

  FILE *batteryinfo = fopen(BATTERY_INFO_PATH, "r");

  if(batteryinfo == NULL){
    perror("Failed to read battery information file");
    exit(EXIT_FAILURE);
  }

  char buff[MAX_LINE_SIZE + 1];

  char status[MAX_LINE_SIZE + 1];
  int percentage;

  while (fgets(buff, MAX_LINE_SIZE, batteryinfo)) {
    char *label = skipWhiteSpace(buff);
    char *information;
    __strtok_r(label, ":", &information);
    information = skipWhiteSpace(information);
    strtok(information, "\n");
    if(strcmp(label, "state") == 0){
      strcpy(status, information);
    } else {
      percentage = strtol(information, NULL, 10);
    }
  }

  init_battery(battery, status, percentage);

  
  if(fclose(batteryinfo) != 0){
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }

}

int main(void) {

  battery_t *battery = new_battery();

  read_battery_info(battery);

  log_battery_info(battery);
  
  monitor_battery(battery);

  free(battery);

  if (system(DELETE_TEMP_CONTENTS) != SYSTEM_SUCCESS) {
    perror("Failed to delete contents of temp");
  }
  
  return EXIT_SUCCESS;
}
