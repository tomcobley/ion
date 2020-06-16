#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "../battery.h"
#include "../log.h"

typedef struct {
  int successes;
  int failures;
} test_state;

bool check(bool truth_value, char test_name[], test_state *state);

bool equal_int(int a, int b, char test_name[], test_state *state);

bool equal_battery(battery_t *battery, int percentage, state_t battery_state, char test_name[], test_state* state);

bool check_file(char file_path[], char correct_file_path[], char test_name[], test_state *state);

#endif
