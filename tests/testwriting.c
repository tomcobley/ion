#include "check.h"
#include <stdlib.h>
test_state struct_test_state;
test_state *state = &struct_test_state;

int main(void) {
 
  // *************** BEGIN TESTS ***************
  // TEST 1
  // 16 June 09.30, charging, 50%
  time_t current_time = 1592299800;
  battery_t *test_battery = alloc_battery();
  FILE *file_1 = fopen("./reading_tests/linux_1.txt", "r");
  if(file_1 == NULL){
    perror("Failed to open input file for test 1");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_1);
  
  FILE *log_1 = fopen("./writing_tests/log_1.txt", "w");
  FILE *analysis_1 = fopen("./writing_tests/analysis_1.csv", "w");
  if(log_1 == NULL || analysis_1 == NULL){
    perror("Failed to open writing files for test 1");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_1, analysis_1, current_time);

  equal_file("./writing_tests/log_1.txt","./writing_tests/test_log_1.txt", "Text file correctly written to for linux 1", state);
  equal_file("./writing_tests/analysis_1.csv", "./writing_tests/test_analysis_1.csv", "Csv file correctly written to for linux 1", state);
  
  fclose(file_1);
  fclose(log_1);
  fclose(analysis_1);
  // TEST 2
  // TEST 3
  // TEST 4
  free_battery(test_battery);
  // *************** END OF TESTS ***************

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS; 
}
