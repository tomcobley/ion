#include <stdlib.h>
#include "check.h"
#include "../log.h"
#include "../battery.h"

test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {
 
  // *************** BEGIN TESTS ***************
  // TEST 1
  // 7 standard sleep times: 
  // 09 June: 16:30, 10 June: 16:40, 11 June: 16:50, 12 June: 17:00,
  // 13 June: 17:10, 14 June: 17:20, 15 June: 17:30
  battery_t *test_battery = alloc_battery();
  
  FILE *test_file_1 = fopen("./analysis_tests/analysistest1.csv", "r");
  
  // 16 June 09:30
  time_t current_time = 1592299800;
  
  monitor_sleep_time(current_time, test_battery, test_file_1); 
  
  equal_int(1020, test_battery->data->average_sleep_time, "7 standard sleep times", state);
  
  fclose(test_file_1);
  
  // TEST 2
  // 7 standard sleep times as above, but including normal usage throughout to check
  // they are not counted as sleep times

  FILE *test_file_2 = fopen("./analysis_tests/analysistest2.csv", "r");
  
  monitor_sleep_time(current_time, test_battery, test_file_2); 
  
  equal_int(1020, test_battery->data->average_sleep_time, "7 standard sleep times + normal non-sleep usage", state);
  
  fclose(test_file_2);
  // TEST 3
  // 7 standard sleep times + normal usage as above, but including 21:50 anomaly
  
  FILE *test_file_3 = fopen("./analysis_tests/analysistest3.csv", "r");
  
  monitor_sleep_time(current_time, test_battery, test_file_3); 
  
  equal_int(1056, test_battery->data->average_sleep_time, "7 standard sleep times + normal non-sleep usage + anomaly", state);
  
  fclose(test_file_3);
  // TEST 4
  // 7 standard sleep times + normal usage + anomaly as above, but including sleep
  // from before 7 day window to check they are not counted as sleep times
  
  FILE *test_file_4 = fopen("./analysis_tests/analysistest4.csv", "r");
  
  monitor_sleep_time(current_time, test_battery, test_file_4); 
  
  equal_int(1056, test_battery->data->average_sleep_time, "7 standard sleep times + normal non-sleep usage + anomaly + usage before 7 day window", state);
  
  fclose(test_file_4);
  
  free_battery(test_battery);
  // *************** END OF TESTS ***************

  printf("\n%d test(s) failed, %d test(s) passed\n", state->failures, state->successes);
  return EXIT_SUCCESS; 

}
