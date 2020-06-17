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
  equal_battery(b1, 50, CHARGING, "read_battery_info__ubuntu: percentage: 50%, state: charging", state);
  free_battery(b1);
  fclose(linux_in1);

  battery_t *b2 = alloc_battery();
  FILE *linux_in2 = fopen("./reading_tests/linux_2.txt", "r");
  if (linux_in2 == NULL) {
    perror("Failed to open linux input 2");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b2, linux_in2);
  equal_battery(b2, 100, CHARGING, "read_battery_info__ubuntu: percentage: 100%, state: fully_charged", state);
  free_battery(b2);
  fclose(linux_in2);

  battery_t *b3 = alloc_battery();
  FILE *linux_in3 = fopen("./reading_tests/linux_3.txt", "r");
  if (linux_in3 == NULL) {
    perror("Failed to open linux input 3");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b3, linux_in3);
  equal_battery(b3, 25, DISCHARGING, "read_battery_info__ubuntu: percentage: 25%, state: discharging", state);
  free_battery(b3);
  fclose(linux_in3);

  battery_t *b4 = alloc_battery();
  FILE *linux_in4 = fopen("./reading_tests/linux_4.txt", "r");
  if (linux_in4 == NULL) {
    perror("Failed to open linux input 4");
    exit(EXIT_FAILURE);
  }
  read_battery_info__ubuntu(b4, linux_in4);
  equal_battery(b4, 100, DISCHARGING, "read_battery_info__ubuntu: percentage: 100%, state: discharging", state);
  free_battery(b4);
  fclose(linux_in4);


  // read_battery_info__macos
  battery_t *b5 = alloc_battery();
  FILE *mac_in5 = fopen("./reading_tests/mac_charging_21.txt", "r");
  if (mac_in5 == NULL) {
    perror("Failed to open mac input 5");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b5, mac_in5);
  equal_battery(b5, 21, CHARGING, "read_battery_info__macos: percentage: 21%, state: charging", state);
  free_battery(b5);
  fclose(mac_in5);

  battery_t *b6 = alloc_battery();
  FILE *mac_in6 = fopen("./reading_tests/mac_charging_55.txt", "r");
  if (mac_in6 == NULL) {
    perror("Failed to open mac input 6");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b6, mac_in6);
  equal_battery(b6, 55, CHARGING, "read_battery_info__macos: percentage: 55%, state: charging", state);
  free_battery(b6);
  fclose(mac_in6);

  battery_t *b7 = alloc_battery();
  FILE *mac_in7 = fopen("./reading_tests/mac_charging_83.txt", "r");
  if (mac_in7 == NULL) {
    perror("Failed to open mac input 7");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b7, mac_in7);
  equal_battery(b7, 83, CHARGING, "read_battery_info__macos: percentage: 83%, state: charging", state);
  free_battery(b7);
  fclose(mac_in7);

  battery_t *b8 = alloc_battery();
  FILE *mac_in8 = fopen("./reading_tests/mac_discharging_18.txt", "r");
  if (mac_in8 == NULL) {
    perror("Failed to open mac input 8");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b8, mac_in8);
  equal_battery(b8, 18, DISCHARGING, "read_battery_info__macos: percentage: 18%, state: discharging", state);
  free_battery(b8);
  fclose(mac_in8);

  battery_t *b9 = alloc_battery();
  FILE *mac_in9 = fopen("./reading_tests/mac_discharging_55.txt", "r");
  if (mac_in9 == NULL) {
    perror("Failed to open mac input 9");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b9, mac_in9);
  equal_battery(b9, 55, DISCHARGING, "read_battery_info__macos: percentage: 55%, state: discharging", state);
  free_battery(b9);
  fclose(mac_in9);

  battery_t *b10 = alloc_battery();
  FILE *mac_in10 = fopen("./reading_tests/mac_discharging_83.txt", "r");
  if (mac_in10 == NULL) {
    perror("Failed to open mac input 10");
    exit(EXIT_FAILURE);
  }
  read_battery_info__macos(b10, mac_in10);
  equal_battery(b10, 83, DISCHARGING, "read_battery_info__macos: percentage: 83%, state: discharging", state);
  free_battery(b10);
  fclose(mac_in10);

  


  // *************** END OF TESTS ***************

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS;
}
#endif
