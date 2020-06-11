#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SYSTEM_SUCCESS 0
#define MAX_LINE_SIZE 1024
#define CYCLE_LOWER_BOUND 30
#define CYCLE_UPPER_BOUND 80

typedef enum state {
  CHARGING,
  DISCHARGING,
  FULL
} state_t;

typedef struct battery {
  int percentage;
  state_t state;
} battery_t;

battery_t *new_battery(void){
  battery_t *battery = calloc(1, sizeof(battery_t));
  if(!battery){
    perror("Failed to allocate memory for battery");
    exit(EXIT_FAILURE);
  }
  return battery;
}

void init_battery(battery_t *battery, char *status, int percentage){
  if (strcmp(status, "fully-charged") == 0) {
     battery->state = FULL;
  } else if (strcmp(status, "discharging") == 0) {
     battery->state = DISCHARGING;
  } else if (strcmp(status, "charging") == 0) {
    battery->state = CHARGING;
  } else {
    perror("Could not read battery_status");
    exit(EXIT_FAILURE);
  }
  battery->percentage = percentage;
}

char *skipWhiteSpace(char *str) {
  char *ptr = str;
  while (*ptr == ' ') {
    ptr++;
  }
  return ptr;
}


int main(void) {

  battery_t *battery = new_battery();

  int grep = system("upower -i `upower -e | grep 'BAT'` | egrep 'state|percentage' > batteryinfo.txt");

  if(grep != SYSTEM_SUCCESS){
    perror("Failed to execute upower command");
    exit(EXIT_FAILURE);
  }

  FILE *batteryinfo = fopen("batteryinfo.txt", "r");

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

  printf("Percentage: %d, State: %d", battery->percentage, battery->state);

  free(battery);
  if(fclose(batteryinfo) != 0){
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }
  
  return EXIT_SUCCESS;
}
