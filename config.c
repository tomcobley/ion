#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "config.h"


// returns a pointer to a heap allocated struct of type config_t
config_t *alloc_config(void) {
  config_t *config = malloc(sizeof(config_t));
  if (!config) {
    perror("Memory allocation failed for config. "); exit(EXIT_FAILURE);
  }
  return config;
}

static bool streq(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

// function to copy the supplied src string into *dest.
//    if *dest is NULL, allocate memory for it
static void populate_string(char **dest_ptr, const char *src, int max_length) {
  if (!*dest_ptr) { *dest_ptr = malloc(max_length); }
  if (!*dest_ptr) {
    // if *dest_ptr is still NULL
    perror("Memory allocation failed. "); exit(EXIT_FAILURE);
  }
  strncpy(*dest_ptr, src, max_length);
}

// set the value of the config option corresponding to the supplied key
//    if the struct member is not initialised, allocate memory for it
void set_config_option(config_t *config, char *key, char *value) {
  //CONFIG_UPDATE
  if (streq(key, "charge_low_webhook_url")) {
    populate_string(&config->charge_low_webhook_url, value, MAX_CONFIG_VALUE_LENGTH);
  } else if (streq(key, "charge_high_webhook_url")) {
    populate_string(&config->charge_high_webhook_url, value, MAX_CONFIG_VALUE_LENGTH);
  } else {
    // TODO:
    assert(false);
  }
}

// frees a config struct created by alloc_config
void free_config(config_t *config) {
  //CONFIG_UPDATE
  free(config->charge_low_webhook_url);
  free(config->charge_high_webhook_url);
  free(config);
}


// print config (for debugging purposes)
void print_config(config_t *config) {
  //CONFIG_UPDATE
  printf("charge_low_webhook_url: %s\n", config->charge_low_webhook_url);
  printf("charge_high_webhook_url: %s\n", config->charge_high_webhook_url);
}


// reads supplied config file and returns true if the file is valid
//    (i.e. the file exists, is structured correctly and has been successfully
//     parsed to generate a config struct)
// if successful, the config values will be saved to the supplied struct
// Pre: config_file should point to the START of the file
bool read_config(FILE *config_file, config_t *config) {

  int count = 0;
  char line_buffer[MAX_CONFIG_LINE_LENGTH + 1];

  while (fgets(line_buffer, MAX_CONFIG_LINE_LENGTH, config_file)) {

    // tokenize at '=', line_buffer will point to key of line
    strtok(line_buffer, "=");
    if (!streq(line_buffer, config_format[count])) {
      // key string is not correct according to config_format, so return false
      //printf("Incorrect string, expected %s, got %s\n", config_format[count], line_buffer);
      return false;
    }

    // key string is correct, so extract value
    //    (use NULL parameter to strtok to continue tokenizing line_buffer)
    char *value = strtok(NULL, ";");

    // line_buffer still points to key, val points to value for this line;
    //    save this value to relevant member in config
    set_config_option(config, line_buffer, value);

    count++;
  }


  if (count < CONFIG_OPTIONS_COUNT) {
    // not enough options specified, return false
    //printf("%s, count = %d\n", "not enough options", count);
    return false;
  }

  // the config file was successfully read and transferred to 'config'
  return true;
}


static void update_config_option(char **dest, const char *prompt) {
  printf("\n%s:\n", prompt);
  if (*dest) {
    // *dest is not NULL, so present default option
    printf("        [or ENTER to use \"%s\"]\n", *dest);
  }

  char response[MAX_CONFIG_VALUE_LENGTH];
  fgets(response, MAX_CONFIG_VALUE_LENGTH, stdin);

  // remove any trailing '\n's from response
  for (int i = 0; response[i]; i++) {
    if (response[i] == '\n') { response[i] = '\0'; break; }
  }

  if (streq(response, "") && !*dest) {
    // empty response not allowed as no default exists, so ask again
    fprintf(stderr, "Please enter a response. ");
    update_config_option(dest, prompt);
  } else if (streq(response, "")) {
    // use default (i.e. don't change value of dest)
    printf("Using default.\n");
  } else {
    populate_string(dest, response, MAX_CONFIG_VALUE_LENGTH);
  }
}

// take user input to populate the supplied config struct.
//    if values already exist in config, the user will be able to leave these
//    by entering an empty string for a new value
static void get_config(config_t *config) {

  for(int i = 0; i < CONFIG_OPTIONS_COUNT; i++) {
    char *key = config_format[i];
    if (streq(key, "charge_low_webhook_url")) {
      update_config_option(&config->charge_low_webhook_url,
        "Enter URL of webhook to be activated when battery charge reaches LOWER threshold");
    } else if (streq(key, "charge_high_webhook_url")) {
      update_config_option(&config->charge_high_webhook_url,
        "Enter URL of webhook to be activated when battery charge reaches UPPER threshold");
    }

  }
}


// Function to write config to config_file
// Pre: config_file points to START of file
static void save_config(FILE *config_file, config_t *config) {

  //CONFIG_UPDATE
  fprintf(config_file, "%s=%s;\n", "charge_low_webhook_url",
                            config->charge_low_webhook_url);
  fprintf(config_file, "%s=%s;\n", "charge_high_webhook_url",
                            config->charge_high_webhook_url);

}



void init(config_t *config) {

  bool config_valid = false;
  printf("\n");

  // open config file for reading
  FILE *config_file = fopen(CONFIG_FILE_PATH, "r");
  if (config_file == NULL) {
    // no config file exists, output message
    printf("No .config file found. ");
  } else {
    // config file exists, so read config from it
    //    return true (and populate config) if it is a valid config
    config_valid = read_config(config_file, config);

    fclose(config_file);
  }

  printf("Enter config options below (or press ENTER to use previous values where given):\n");

  get_config(config);

  printf("Configuration updated successfully.\n");
  //print_config(config);


  // reopen config file for writing (to overwrite current contents)
  config_file = fopen(CONFIG_FILE_PATH, "w");
  if (config_file == NULL) {
    perror("Failed to open/create config file. "); exit(EXIT_FAILURE);
  }

  // write updated config to config file
  save_config(config_file, config);

}




// int main(int argc, char const *argv[]) {
//
//   config_t *config = alloc_config();
//
//   init(config);
//
//   free_config(config);
//
// }
