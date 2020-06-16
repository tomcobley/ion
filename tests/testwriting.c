#include "check.h"
#include <stdlib.h>
test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {
 
  // *************** BEGIN TESTS ***************
  // TEST 1
  // *************** END OF TESTS ***************

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS; 
}
