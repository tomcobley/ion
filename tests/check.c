#include "check.h"

bool check(bool truth_value, char test_name[], test_state *state) {
  printf( truth_value ? "/ - PASS" : "X - FAIL" );
  printf(" -- %s\n", test_name);
  if (truth_value) {
    state->successes ++;
  } else {
    state->failures ++;
  }
  return truth_value;
}

bool equal_int(int a, int b, char test_name[], test_state *state) {
  bool success = check( a == b , test_name, state);
  if (!success) {
    printf("            EXPECTED a == b\n");
    printf("                     BUT a = %u  [hex: 0x%x]\n", a, a);
    printf("                         b = %u  [hex: 0x%x]\n", b, b);
  }  
  return success;
} 

bool equal_battery(battery_t *battery, int percentage, state_t battery_state, char test_name[], test_state *state) {
  bool success = check( battery->percentage == percentage && battery->state == battery_state, test_name, state);
    if (!success) {
      char state_str[MAX_LINE_SIZE];
      state_to_string(battery_state, state_str);
      char battery_state_str[MAX_LINE_SIZE];
      state_to_string(battery->state, battery_state_str);
      printf("            EXPECTED PERCENTAGE: %d, EXPECTED STATE: %s \n", percentage, state_str);
    printf("                     BUT PERCENTAGE: %d\n", battery->percentage);
    printf("                     AND STATE: %s\n", battery_state_str);
  }  
  return success;
}
