#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "sys.h"
#include "main.h"

#if __APPLE__
  // required for determining root path (apple only)
  #include <libproc.h>
#endif

void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo);

void read_battery_info__macos(battery_t *battery, FILE *batteryinfo);



// function to change the working directory to that of the codebase's root
//  (useful for later file writing with relative paths)
void set_working_dir(void) {
  // determine the absolute path of the codebase's root
  char *path_buffer = malloc(MAX_BUFFER_SIZE);
  ssize_t sz;

  // determine path of current process (dependent on OS)
  #if __APPLE__
    sz = proc_pidpath(getpid(), path_buffer, MAX_BUFFER_SIZE);
  #elif __linux__
    sz = readlink("/proc/self/exe", path_buffer, MAX_BUFFER_SIZE);
  #else
    perror("Unsupported operating system.");
    exit(EXIT_FAILURE);
  #endif

  printf("[%s]\n", path_buffer);

  // Find final directory separator of path
  while (sz > 0 && path_buffer[sz-1] != '/') { --sz; }
  // Truncate string to directory name only
  path_buffer[sz] = '\0';

  // change directory to path
  if (chdir(path_buffer) != 0) {
    perror("Failed to change working directory to codebase root");
    exit(EXIT_FAILURE);
  }

  printf("[%s]\n", path_buffer);

  // printing current working directory
  char s[100];
  printf("wd = %s\n", getcwd(s, 100));
}


op_sys_t determine_os(void) {
  // determine operating system by compiler flags
#if __APPLE__
  printf("Apple OS detected.\n");
  // apple operating system
  return MACOS;
#elif __linux__
  printf("Linux OS detected.\n");
  // linux operating system
  return LINUX;
#else
  perror("Unsupported operating system.");
  exit(EXIT_FAILURE);
#endif
}


void read_battery_info(battery_t *battery, op_sys_t op_sys) {

  int info_status;

  void (*read_battery_info__func)(battery_t *battery, FILE *batteryinfo);

  if (op_sys == LINUX) {
    info_status = system(BATTERY_INFO__LINUX);
    read_battery_info__func = read_battery_info__ubuntu;
  } else if (op_sys == MACOS) {
    info_status = system(BATTERY_INFO__MACOS);
    read_battery_info__func = read_battery_info__macos;
  } else {
    perror("Unsupported operating sytem.");
    exit(EXIT_FAILURE);
  }

  if (info_status != SYSTEM_SUCCESS) {
    perror("Failed to execute BATTERY_INFO command");
    exit(EXIT_FAILURE);
  }

  FILE *batteryinfo = fopen(BATTERY_INFO_PATH, "r");
  if (batteryinfo == NULL) {
    perror("Failed to read battery information file");
    exit(EXIT_FAILURE);
  }

  read_battery_info__func(battery, batteryinfo);

  // if file is empty then device does not have battery/access to battery failed
  if (ftell(batteryinfo) == 0) {
    perror("Failed to access battery or device does not have a battery");
    exit(EXIT_FAILURE);
  }

  if (fclose(batteryinfo) != 0) {
    perror("Failed to close battery information file");
    exit(EXIT_FAILURE);
  }
}

static void update_battery(battery_t *battery, state_t state, int percentage) {
  if (percentage < 0 || percentage > 100) {
    perror("Invalid percentage passed as parameter to update battery");
    exit(EXIT_FAILURE);
  }
  if (state != CHARGING && state != DISCHARGING) {
    perror("Invalid state passed as parameter to update battery");
    exit(EXIT_FAILURE);
  }
  battery->state = state;
  battery->percentage = percentage;
}


static char *skip_white_space(char *str) {
  while(*str == ' '){str++;}
  return str;
}


/////////////////////////       LINUX        /////////////////////////



void read_battery_info__ubuntu(battery_t *battery, FILE *batteryinfo) {
  char buff[MAX_BUFFER_SIZE + 1];
  char status[MAX_BUFFER_SIZE + 1];
  int percentage = -1;

  while (fgets(buff, MAX_BUFFER_SIZE, batteryinfo)) {
    // get ptr to label of this line
    char *label_ptr = skip_white_space(buff);
    strtok(label_ptr, ":");

    // get following token (the information) up to the newline character
    char *info_ptr;
    info_ptr = strtok(NULL, "\n");
    info_ptr = skip_white_space(info_ptr);

    if (strcmp(label_ptr, "state") == 0) {
      strcpy(status, info_ptr);
    } else {
      percentage = strtol(info_ptr, NULL, 10);
    }
  }

  if (percentage == -1) {
    perror("Failed to parse percentage in read_battery_info__linux");
    exit(EXIT_FAILURE);
  }

  // initiliase battery struct using information from system
  if (strcmp(status, "fully-charged") == 0 || strcmp(status, "charging") == 0) {
    update_battery(battery, CHARGING, percentage);
  } else if (strcmp(status, "discharging") == 0) {
    update_battery(battery, DISCHARGING, percentage);
  } else {
    perror("Could not read battery status");
    exit(EXIT_FAILURE);
  }

}



/////////////////////////       macOS        /////////////////////////




void read_battery_info__macos(battery_t *battery, FILE *batteryinfo) {
  char buff[MAX_BUFFER_SIZE + 1];
  int percentage = -1;
  char status[MAX_BUFFER_SIZE + 1];

  // disregard first line of file, store second line (which will contain
  //    the relevant data into buff
  fgets(buff, MAX_BUFFER_SIZE, batteryinfo);
  fgets(buff, MAX_BUFFER_SIZE, batteryinfo);

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
    perror("Failed to parse percentage in read_battery_info__macos");
    exit(EXIT_FAILURE);
  }

  // (strptr + 2) represents start of state field, which contains leading
  //    whitespace and is terminated by a ';'
  str_ptr += 2;
  strtok(str_ptr, ";");
  strcpy(status, skip_white_space(str_ptr));

  // initiliase battery struct using information from system
  if (strcmp(status, "fully-charged") == 0 || strcmp(status, "charged") == 0 || strcmp(status, "charging") == 0) {
    update_battery(battery, CHARGING, percentage);
  } else {
    // battery is either "discharging" or "AC attached; not charging"
    //    so assume battery is discharging
    update_battery(battery, DISCHARGING, percentage);
  }

}
