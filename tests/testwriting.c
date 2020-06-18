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
  if (file_1 == NULL) {
    perror("Failed to open input file for test 1");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_1);

  FILE *log_1 = fopen("./writing_tests/log_1.txt", "w");
  FILE *analysis_1 = fopen("./writing_tests/analysis_1.csv", "w");
  if (log_1 == NULL || analysis_1 == NULL) {
    perror("Failed to open writing files for test 1");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_1, analysis_1, current_time);

  fclose(file_1);
  fclose(log_1);
  fclose(analysis_1);

  equal_file("./writing_tests/log_1.txt", "./writing_tests/test_log_1.txt",
             "Text file correctly written to for linux 1", state);
  equal_file("./writing_tests/analysis_1.csv", "./writing_tests/test_analysis_1.csv",
             "CSV file correctly written to for linux 1", state);

  // TEST 2
  // 16 June 09.30, fully-charged, 100%
  // note fully-charged is written as charging
  FILE *file_2 = fopen("./reading_tests/linux_2.txt", "r");
  if (file_2 == NULL) {
    perror("Failed to open input file for test 2");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_2);

  FILE *log_2 = fopen("./writing_tests/log_2.txt", "w");
  FILE *analysis_2 = fopen("./writing_tests/analysis_2.csv", "w");
  if (log_2 == NULL || analysis_2 == NULL) {
    perror("Failed to open writing files for test 2");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_2, analysis_2, current_time);

  fclose(file_2);
  fclose(log_2);
  fclose(analysis_2);

  equal_file("./writing_tests/log_2.txt", "./writing_tests/test_log_2.txt",
             "Text file correctly written to for linux 2", state);
  equal_file("./writing_tests/analysis_2.csv", "./writing_tests/test_analysis_2.csv",
             "CSV file correctly written to for linux 2", state);

  // TEST 3
  // 16 June 09.30, discharging, 25%
  FILE *file_3 = fopen("./reading_tests/linux_3.txt", "r");
  if (file_3 == NULL) {
    perror("Failed to open input file for test 3");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_3);

  FILE *log_3 = fopen("./writing_tests/log_3.txt", "w");
  FILE *analysis_3 = fopen("./writing_tests/analysis_3.csv", "w");
  if (log_3 == NULL || analysis_3 == NULL) {
    perror("Failed to open writing files for test 3");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_3, analysis_3, current_time);

  fclose(file_3);
  fclose(log_3);
  fclose(analysis_3);

  equal_file("./writing_tests/log_3.txt", "./writing_tests/test_log_3.txt",
             "Text file correctly written to for linux 3", state);
  equal_file("./writing_tests/analysis_3.csv", "./writing_tests/test_analysis_3.csv",
             "CSV file correctly written to for linux 3", state);

  // TEST 4
  // 16 June 09.30, discharging, 100%
  FILE *file_4 = fopen("./reading_tests/linux_4.txt", "r");
  if (file_4 == NULL) {
    perror("Failed to open input file for test 4");
    exit(EXIT_FAILURE);
  }

  read_battery_info__ubuntu(test_battery, file_4);

  FILE *log_4 = fopen("./writing_tests/log_4.txt", "w");
  FILE *analysis_4 = fopen("./writing_tests/analysis_4.csv", "w");
  if (log_4 == NULL || analysis_4 == NULL) {
    perror("Failed to open writing files for test 4");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_4, analysis_4, current_time);

  fclose(file_4);
  fclose(log_4);
  fclose(analysis_4);

  equal_file("./writing_tests/log_4.txt", "./writing_tests/test_log_4.txt",
             "Text file correctly written to for linux 4", state);
  equal_file("./writing_tests/analysis_4.csv", "./writing_tests/test_analysis_4.csv",
             "CSV file correctly written to for linux 4", state);

  // TEST 5
  // 16 June 09.30, charging, 21% 
  FILE *file_5 = fopen("./reading_tests/mac_charging_21.txt", "r");
  if (file_5 == NULL) {
    perror("Failed to open input file for test 5");
    exit(EXIT_FAILURE);
  }

  read_battery_info__macos(test_battery, file_5);

  FILE *log_5 = fopen("./writing_tests/log_5.txt", "w");
  FILE *analysis_5 = fopen("./writing_tests/analysis_5.csv", "w");
  if (log_5 == NULL || analysis_5 == NULL) {
    perror("Failed to open writing files for test 5");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_5, analysis_5, current_time);

  fclose(file_5);
  fclose(log_5);
  fclose(analysis_5);

  equal_file("./writing_tests/log_5.txt", "./writing_tests/test_log_5.txt", "Text file correctly written to for mac 1",
             state);
  equal_file("./writing_tests/analysis_5.csv", "./writing_tests/test_analysis_5.csv",
             "CSV file correctly written to for mac 1", state);

  // TEST 6
  // 16 June 09.30, charging, 55% 
  FILE *file_6 = fopen("./reading_tests/mac_charging_55.txt", "r");
  if (file_6 == NULL) {
    perror("Failed to open input file for test 6");
    exit(EXIT_FAILURE);
  }

  read_battery_info__macos(test_battery, file_6);

  FILE *log_6 = fopen("./writing_tests/log_6.txt", "w");
  FILE *analysis_6 = fopen("./writing_tests/analysis_6.csv", "w");
  if (log_6 == NULL || analysis_6 == NULL) {
    perror("Failed to open writing files for test 6");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_6, analysis_6, current_time);

  fclose(file_6);
  fclose(log_6);
  fclose(analysis_6);

  equal_file("./writing_tests/log_6.txt", "./writing_tests/test_log_6.txt", "Text file correctly written to for mac 2",
             state);
  equal_file("./writing_tests/analysis_6.csv", "./writing_tests/test_analysis_6.csv",
             "CSV file correctly written to for mac 2", state);

  // TEST 7
  // 16 June 09.30, discharing, 18% 
  FILE *file_7 = fopen("./reading_tests/mac_discharging_18.txt", "r");
  if (file_7 == NULL) {
    perror("Failed to open input file for test 7");
    exit(EXIT_FAILURE);
  }

  read_battery_info__macos(test_battery, file_7);

  FILE *log_7 = fopen("./writing_tests/log_7.txt", "w");
  FILE *analysis_7 = fopen("./writing_tests/analysis_7.csv", "w");
  if (log_7 == NULL || analysis_7 == NULL) {
    perror("Failed to open writing files for test 7");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_7, analysis_7, current_time);

  fclose(file_7);
  fclose(log_7);
  fclose(analysis_7);

  equal_file("./writing_tests/log_7.txt", "./writing_tests/test_log_7.txt", "Text file correctly written to for mac 3",
             state);
  equal_file("./writing_tests/analysis_7.csv", "./writing_tests/test_analysis_7.csv",
             "CSV file correctly written to for mac 3", state);

  // TEST 8
  // 16 June 09.30, discharging, 83% 
  FILE *file_8 = fopen("./reading_tests/mac_discharging_83.txt", "r");
  if (file_8 == NULL) {
    perror("Failed to open input file for test 8");
    exit(EXIT_FAILURE);
  }

  read_battery_info__macos(test_battery, file_8);

  FILE *log_8 = fopen("./writing_tests/log_8.txt", "w");
  FILE *analysis_8 = fopen("./writing_tests/analysis_8.csv", "w");
  if (log_8 == NULL || analysis_8 == NULL) {
    perror("Failed to open writing files for test 8");
    exit(EXIT_FAILURE);
  }

  write_to_files(test_battery, log_8, analysis_8, current_time);

  fclose(file_8);
  fclose(log_8);
  fclose(analysis_8);

  equal_file("./writing_tests/log_8.txt", "./writing_tests/test_log_8.txt", "Text file correctly written to for mac 4",
             state);
  equal_file("./writing_tests/analysis_8.csv", "./writing_tests/test_analysis_8.csv",
             "CSV file correctly written to for mac 4", state);

  // *************** END OF TESTS ***************
  free_battery(test_battery);

  printf("%d test(s) failed, %d test(s) passed\n", state->failures, state->successes);
  printf("--------------------------------------------------------------------------------------------\n");
  return EXIT_SUCCESS;
}
