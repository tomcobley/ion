#include "check.h"
#include <stdlib.h>
test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {
 
  // *************** BEGIN TESTS ***************
  // TEST 1
  // 16 June 09.30
  time_t current_time = 1592299800;
  battery_t test_battery = alloc_battery();
  op_sys_t op_sys = determine_os();
  read_battery_info(test_battery, op_sys);
  
  FILE *log_1 = fopen("log_1.txt", "a");
  FILE *analysis_1 = fopen("analysis1.csv", "a");
  write_to_files(test_battery, log_1, analysis_1, current_time);

  equal_file("log_1.txt","test_log_1.txt", "Log one state", state);





  free_battery(test_battery);
  // *************** END OF TESTS ***************

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS; 
}
