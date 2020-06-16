#include "check.h"
#include <stdlib.h>
#include <stdio.h>

test_state struct_test_state;
test_state *state = &struct_test_state;

#ifdef DEBUG
int main(void) {

  // *************** BEGIN TESTS ***************

  // read_battery_info__ubuntu
  battery_t *b1 = alloc_battery();
  FILE *linux_in1 = fopen("./reading_tests/linux_1.txt", "r");
  if (linux_in1 == NULL) {
    perror("Failed to open linux input 1");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b1, linux_in1);
  check_battery(b1, 50, CHARGING, "read_battery_info__ubuntu: percentage: 50%, state: charging", state);
  free_battery(b1);
  fclose(linux_in1);

  battery_t *b2 = alloc_battery();
  FILE *linux_in2 = fopen("./reading_tests/linux_2.txt", "r");
  if (linux_in2 == NULL) {
    perror("Failed to open linux input 2");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b2, linux_in2);
  check_battery(b2, 100, CHARGING, "read_battery_info__ubuntu: percentage: 100%, state: fully_charged", state);
  free_battery(b2);
  fclose(linux_in2);

  battery_t *b3 = alloc_battery();
  FILE *linux_in3 = fopen("./reading_tests/linux_3.txt", "r");
  if (linux_in3 == NULL) {
    perror("Failed to open linux input 3");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b3, linux_in3);
  check_battery(b3, 25, DISCHARGING, "read_battery_info__ubuntu: percentage: 25%, state: discharging", state);
  free_battery(b3);
  fclose(linux_in3);

  battery_t *b4 = alloc_battery();
  FILE *linux_in4 = fopen("./reading_tests/linux_4.txt", "r");
  if (linux_in4 == NULL) {
    perror("Failed to open linux input 4");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b4, linux_in4);
  check_battery(b4, 100, DISCHARGING, "read_battery_info__ubuntu: percentage: 100%, state: discharging", state);
  free_battery(b4);
  fclose(linux_in4);


  // TODO: ADD MACOS READING FILES

  check_file("./reading_tests/linux_3.txt", "./reading_tests/linux_4.txt", "test, remove after", state);

  // *************** END OF TESTS ***************

  printf("\n%d test(s) failed, %d test(s) passed\n", state->failures, state->successes);
  return EXIT_SUCCESS;
}
#endif
