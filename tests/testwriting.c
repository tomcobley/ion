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
  
  fclose(file_1);
  fclose(log_1);
  fclose(analysis_1);

  equal_file("./writing_tests/log_1.txt","./writing_tests/test_log_1.txt", "Text file correctly written to for linux 1", state);
  equal_file("./writing_tests/analysis_1.csv", "./writing_tests/test_analysis_1.csv", "Csv file correctly written to for linux 1", state);
  
  // TEST 2
  // 16 June 09.30, fully-charged, 100%
  // note fully-charged is written as charging
  FILE *file_2 = fopen("./reading_tests/linux_2.txt", "r");
  if(file_2 == NULL){
    perror("Failed to open input file for test 2");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_2);
  
  FILE *log_2 = fopen("./writing_tests/log_2.txt", "w");
  FILE *analysis_2 = fopen("./writing_tests/analysis_2.csv", "w");
  if(log_2 == NULL || analysis_2 == NULL){
    perror("Failed to open writing files for test 2");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_2, analysis_2, current_time);
  
  fclose(file_2);
  fclose(log_2);
  fclose(analysis_2);

  equal_file("./writing_tests/log_2.txt","./writing_tests/test_log_2.txt", "Text file correctly written to for linux 2", state);
  equal_file("./writing_tests/analysis_2.csv", "./writing_tests/test_analysis_2.csv", "Csv file correctly written to for linux 2", state);
  
  // TEST 3
  // 16 June 09.30, discharging, 25%
  FILE *file_3 = fopen("./reading_tests/linux_3.txt", "r");
  if(file_3 == NULL){
    perror("Failed to open input file for test 3");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_3);
  
  FILE *log_3 = fopen("./writing_tests/log_3.txt", "w");
  FILE *analysis_3 = fopen("./writing_tests/analysis_3.csv", "w");
  if(log_3 == NULL || analysis_3 == NULL){
    perror("Failed to open writing files for test 3");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_3, analysis_3, current_time);
  
  fclose(file_3);
  fclose(log_3);
  fclose(analysis_3);

  equal_file("./writing_tests/log_3.txt","./writing_tests/test_log_3.txt", "Text file correctly written to for linux 3", state);
  equal_file("./writing_tests/analysis_3.csv", "./writing_tests/test_analysis_3.csv", "Csv file correctly written to for linux 3", state);
  
  // TEST 4
  // 16 June 09.30, discharging, 100%
  FILE *file_4 = fopen("./reading_tests/linux_4.txt", "r");
  if(file_4 == NULL){
    perror("Failed to open input file for test 4");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_4);
  
  FILE *log_4 = fopen("./writing_tests/log_4.txt", "w");
  FILE *analysis_4 = fopen("./writing_tests/analysis_4.csv", "w");
  if(log_4 == NULL || analysis_4 == NULL){
    perror("Failed to open writing files for test 4");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_4, analysis_4, current_time);
  
  fclose(file_4);
  fclose(log_4);
  fclose(analysis_4);

  equal_file("./writing_tests/log_4.txt","./writing_tests/test_log_4.txt", "Text file correctly written to for linux 4", state);
  equal_file("./writing_tests/analysis_4.csv", "./writing_tests/test_analysis_4.csv", "Csv file correctly written to for linux 4", state);
  
  // *************** END OF TESTS ***************
  free_battery(test_battery);

  printf("%d test(s) failed, %d test(s) passed\n\n", state->failures, state->successes);
  return EXIT_SUCCESS; 
}
