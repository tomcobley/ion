#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
  int successes;
  int failures;
} test_state;

bool check(bool truth_value, char test_name[], test_state *state);

bool equal_int(int a, int b, char test_name[], test_state *state);
