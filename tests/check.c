#include <stdio.h>
#include <stdlib.h>
#include "check.h"
#include "string.h"

#define MAX_LINE_LENGTH 1023

bool check(bool truth_value, char test_name[], test_state *state) {
  printf(truth_value ? "/ - PASS" : "X - FAIL");
  printf(" -- %s\n", test_name);
  if (truth_value) {
    state->successes++;
  } else {
    state->failures++;
  }
  return truth_value;
}

bool equal_bool(bool a, bool b, char test_name[], test_state *state) {
  bool success = check(a == b, test_name, state);
  if (!success) {
    printf("            EXPECTED a == b\n");
    printf("                     BUT a = %s\n", a ? "true" : "false");
    printf("                         b = %s\n", b ? "true" : "false");
  }
  return success;
}

bool equal_int(int a, int b, char test_name[], test_state *state) {
  bool success = check(a == b, test_name, state);
  if (!success) {
    printf("            EXPECTED a == b\n");
    printf("                     BUT a = %u  [hex: 0x%x]\n", a, a);
    printf("                         b = %u  [hex: 0x%x]\n", b, b);
  }
  return success;
}

bool equal_res(CURLcode a, CURLcode b, char test_name[], test_state *state) {
  bool success = check(a == b, test_name, state);
  if (!success) {
    printf("            EXPECTED a == b\n");
    printf("                     BUT a = %u  [hex: 0x%x]\n", a, a);
    printf("                         b = %u  [hex: 0x%x]\n", b, b);
  }
  return success;
}

bool equal_battery(battery_t *battery, int percentage, state_t battery_state, char test_name[], test_state *state) {
  bool success = check(battery->percentage == percentage && battery->state == battery_state, test_name, state);
  if (!success) {
    char state_str[MAX_BUFFER_SIZE];
    state_to_string(battery_state, state_str);
    char battery_state_str[MAX_BUFFER_SIZE];
    state_to_string(battery->state, battery_state_str);
    printf("            EXPECTED PERCENTAGE: %d, EXPECTED STATE: %s \n", percentage, state_str);
    printf("                 BUT PERCENTAGE: %d\n", battery->percentage);
    printf("                      AND STATE: %s\n", battery_state_str);
  }
  return success;
}

// platform dependent implementation, but works on mac and linux which are the only systems which will
// run these tests
bool equal_file(char file_path[], char correct_file_path[], char test_name[], test_state *state) {
  char system_command[strlen("diff ") + strlen(file_path) + strlen(" ")
                      + strlen(correct_file_path) + strlen(" > ./temp/file_diff.txt") + 1];
  system_command[0] = '\0';
  strcat(system_command, "diff ");
  strcat(system_command, file_path);
  strcat(system_command, " ");
  strcat(system_command, correct_file_path);
  strcat(system_command, " > ./temp/file_diff.txt");

  if (system(system_command) == -1) {
    perror("System call to diff failed");
    exit(EXIT_FAILURE);
  }

  FILE *file_diff = fopen("./temp/file_diff.txt", "r");

  if (file_diff == NULL) {
    perror("Failed to open differences file");
    exit(EXIT_FAILURE);
  }

  if (fseek(file_diff, 0, SEEK_END) != 0) {
    perror("Failed to return to end of file");
  }

  int file_size = ftell(file_diff);

  bool success = check(file_size == 0, test_name, state);

  if (!success) {

    if (fseek(file_diff, 0, SEEK_SET) != 0) {
      perror("Failed to return to start of file");
    }

    printf("DIFFERENCES FOUND IN FILES: \n");
    char buffer[MAX_LINE_LENGTH + 1];
    while (fgets(buffer, MAX_LINE_LENGTH, file_diff)) {
      printf("%s", buffer);
    }
  }

  fclose(file_diff);

  return success;
}
