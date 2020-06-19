#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "../config.h"
test_state struct_test_state;
test_state *state = &struct_test_state;

FILE *open_config_file_test(char *mode) {
  FILE *fp = fopen("../.config", mode);
  if (!fp && !strcmp(mode, "r")) {
    perror("Failed to open/create config file. ");
    exit(EXIT_FAILURE);
  }
  return fp;
}

int main(void) {

  config_t *config = alloc_config();
  FILE *config_file = open_config_file_test("r");
  if (!config_file || !read_config(config_file, config)) {
    fprintf(stderr,
            "No valid .config file found. Please run '%s' to configure program. \n",
            INIT_COMMAND
    );
    exit(EXIT_FAILURE);
  }
  fclose(config_file);

  // *************** BEGIN TESTS ***************
  // TEST 1
  CURLcode res_1 = post_to_webhook(config->str_charge_low_webhook_url);
  equal_res(res_1, CURLE_OK, "Turn plug on", state);

  // TEST 2
  CURLcode res_2 = post_to_webhook(config->str_charge_high_webhook_url);
  equal_res(res_2, CURLE_OK, "Turn plug off", state);

  // *************** END OF TESTS **************

  printf("%d test(s) failed, %d test(s) passed\n", state->failures, state->successes);
  printf("--------------------------------------------------------------------------------------------\n");
  return EXIT_SUCCESS;

}
