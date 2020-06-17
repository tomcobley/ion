#include <stdlib.h>
#include "check.h"

test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {

  // *************** BEGIN TESTS ***************
  // TEST 1
  CURLcode res_1 = post_to_webhook("https://maker.ifttt.com/trigger/charge_low/with/key/dMgkSNQqbJM-9NI1ySrKdb");
  equal_res(res_1, CURLE_OK, "Turn plug on", state);

  // TEST 2
  CURLcode res_2 = post_to_webhook("https://maker.ifttt.com/trigger/charge_high/with/key/dMgkSNQqbJM-9NI1ySrKdb");
  equal_res(res_2, CURLE_OK, "Turn plug off", state);

  // *************** END OF TESTS **************

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS;

}
