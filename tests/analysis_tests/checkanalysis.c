#include <stdlib.h>
#include "../check.h"
#include "../../log.h"

test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {
 
  // *************** BEGIN TESTS ***************
  // 7 standard sleep times: 16:30, 16:40, 16:50, 17:00, 17:10, 17:20, 17:30
  equal_int(1020, log_battery_info(), "log_battery_info: 7 standard sleep times", state);

  // 7 standard sleep times as above, but including normal usage throughout to check
  // they are not counted as sleep times

  // 7 standard sleep times + normal usage as above, but including 21:50 anomaly
  
  // 7 standard sleep times + normal usage + anomaly as above, but including sleep
  // from before 7 day window to check they are not counted as sleep times
  // *************** END OF TESTS ***************

  printf("\n%d test(s) failed, %d test(s) passed\n", state->failures, state->successes);
  return EXIT_SUCCESS; 

}
