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
