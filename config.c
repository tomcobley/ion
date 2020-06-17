#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#include "config.h"


//CONFIG_UPDATE
// Note: the order of the config file must be identical to the order of the
//    definition below,
//    and CONFIG_OPTIONS_COUNT (from config.h) must represent the number of
//    config options in the config_t typedef in config.h
char *config_format[CONFIG_OPTIONS_COUNT] = {
        "str_charge_low_webhook_url",
        "str_charge_high_webhook_url",
        "int_cycle_min_charge_percentage",
        "int_cycle_max_charge_percentage"
};


// returns a pointer to a heap allocated struct of type config_t
config_t *alloc_config(void) {
  config_t *config = malloc(sizeof(config_t));
  if (!config) {
    perror("Memory allocation failed for config. ");
    exit(EXIT_FAILURE);
  }
  //CONFIG_UPDATE
  config->str_charge_low_webhook_url = NULL;
  config->str_charge_high_webhook_url = NULL;
  config->int_cycle_min_charge_percentage = -1;
  config->int_cycle_max_charge_percentage = -1;
  return config;
}

static bool streq(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

static void check_percentage(int val) {
  if (val < 0 || val > 100) {
    fprintf(stderr,
            "Percentage value must be between 0 and 100: value given: %d\n", val
    );
  }
}

// function to copy the supplied src string into *dest.
//    if *dest is NULL, allocate memory for it
static void populate_string(char **dest_ptr, const char *src, int max_length) {
  if (!*dest_ptr) { *dest_ptr = malloc(max_length); }
  if (!*dest_ptr) {
    // if *dest_ptr is still NULL
    perror("Memory allocation failed. ");
    exit(EXIT_FAILURE);
  }
  assert(src != NULL);
  strncpy(*dest_ptr, src, max_length);
}

// set the value of the config option corresponding to the supplied key
//    if the struct member is not initialised, allocate memory for it
// if the value should be a string, it will be passed using str_value.
//    if it should be an int, it will be passed using int_value
void set_config_option(config_t *config, const char *key, const char *value) {

  bool use_int = false;
  int value_int;

  if (strncmp(key, "int_", 4) == 0) {
    // key is of int type
    use_int = true;
    value_int = strtol(value, NULL, 10);
    if (errno == EINVAL) {
      perror("Failed to convert str to int. ");
      exit(EXIT_FAILURE);
    }
  }

  //CONFIG_UPDATE
  if (streq(key, "str_charge_low_webhook_url")) {
    assert(!use_int);
    populate_string(&config->str_charge_low_webhook_url, value, MAX_CONFIG_VALUE_LENGTH);
  } else if (streq(key, "str_charge_high_webhook_url")) {
    assert(!use_int);
    populate_string(&config->str_charge_high_webhook_url, value, MAX_CONFIG_VALUE_LENGTH);
  } else if (streq(key, "int_cycle_min_charge_percentage")) {
    assert(use_int);
    check_percentage(value_int);
    config->int_cycle_min_charge_percentage = value_int;
  } else if (streq(key, "int_cycle_max_charge_percentage")) {
    assert(use_int);
    check_percentage(value_int);
    config->int_cycle_max_charge_percentage = value_int;
  } else {
    perror("Unrecognised config key.");
    exit(EXIT_FAILURE);
  }
}

// frees a config struct created by alloc_config
void free_config(config_t *config) {
  //CONFIG_UPDATE
  free(config->str_charge_low_webhook_url);
  free(config->str_charge_high_webhook_url);
  free(config);
}


// print config (for debugging purposes)
void print_config(config_t *config) {
  //CONFIG_UPDATE
  printf("str_charge_low_webhook_url: %s\n", config->str_charge_low_webhook_url);
  printf("str_charge_high_webhook_url: %s\n", config->str_charge_high_webhook_url);
  printf("int_cycle_min_charge_percentage: %d\n", config->int_cycle_min_charge_percentage);
  printf("int_cycle_max_charge_percentage: %d\n", config->int_cycle_max_charge_percentage);
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


static void get_response(char *dest, int max_length) {
  fgets(dest, max_length, stdin);

  // remove any trailing '\n's from dest
  for (int i = 0; dest[i]; i++) {
    if (dest[i] == '\n') {
      dest[i] = '\0';
      break;
    }
  }
}


static void give_prompt(const char *prompt, bool show_default, const char *default_str, int default_int) {
  printf("\n%s:\n", prompt);
  if (show_default && default_str) {
    printf("        [or ENTER to use \"%s\"]\n", default_str);
  } else if (show_default && default_int) {
    printf("        [or ENTER to use \"%d\"]\n", default_int);
  }
}


static void update_config_option(config_t *config, const char *key, const char *prompt, bool allow_default) {
  char response[MAX_CONFIG_VALUE_LENGTH];
  get_response(response, MAX_CONFIG_VALUE_LENGTH);

  if (streq(response, "") && !allow_default) {
    // empty response not allowed as no default exists, so ask again
    fprintf(stderr, "Please enter a response. ");
    update_config_option(config, key, prompt, allow_default);
  } else if (streq(response, "")) {
    // use default (i.e. don't change value at dest)
    printf("Using default.\n");
  } else {
    set_config_option(config, key, response);
  }
}

static void update_config_option__str(config_t *config, const char *key, const char *prompt, char **dest) {
  bool allow_default = *dest != NULL;
  give_prompt(prompt, *dest != NULL, *dest, 0);
  update_config_option(config, key, prompt, allow_default);
}

static void update_config_option__int(config_t *config, const char *key, const char *prompt, int prev_val) {
  // use previous val as default if it's greater than -1
  bool allow_default = prev_val >= 0;
  give_prompt(prompt, allow_default, NULL, prev_val);
  update_config_option(config, key, prompt, allow_default);
}


// take user input to populate the supplied config struct.
//    if values already exist in config, the user will be able to leave these
//    by entering an empty string for a new value
static void get_config(config_t *config) {

  // CONFIG_UPDATE
  for (int i = 0; i < CONFIG_OPTIONS_COUNT; i++) {
    char *key = config_format[i];
    if (streq(key, "str_charge_low_webhook_url")) {
      update_config_option__str(
              config, key,
              "Enter URL of webhook to be activated when battery charge reaches LOWER threshold",
              &config->str_charge_low_webhook_url
      );
    } else if (streq(key, "str_charge_high_webhook_url")) {
      update_config_option__str(
              config, key,
              "Enter URL of webhook to be activated when battery charge reaches UPPER threshold",
              &config->str_charge_high_webhook_url
      );
    } else if (streq(key, "int_cycle_min_charge_percentage")) {
      update_config_option__int(
              config, key,
              "Enter LOWER bound percentage of charge cycle (recommended value: "RECOMMENDED_MIN_CHARGE")",
              config->int_cycle_min_charge_percentage
      );
    } else if (streq(key, "int_cycle_max_charge_percentage")) {
      update_config_option__int(
              config, key,
              "Enter UPPER bound percentage of charge cycle (recommended value: "RECOMMENDED_MAX_CHARGE")",
              config->int_cycle_max_charge_percentage
      );
    }

  }
}


// Function to write config to config_file
// Pre: config_file points to START of file
void save_config(FILE *config_file, config_t *config) {

  //CONFIG_UPDATE
  fprintf(config_file, "%s=%s;\n", "str_charge_low_webhook_url",
          config->str_charge_low_webhook_url);
  fprintf(config_file, "%s=%s;\n", "str_charge_high_webhook_url",
          config->str_charge_high_webhook_url);
  fprintf(config_file, "%s=%d;\n", "int_cycle_min_charge_percentage",
          config->int_cycle_min_charge_percentage);
  fprintf(config_file, "%s=%d;\n", "int_cycle_max_charge_percentage",
          config->int_cycle_max_charge_percentage);
}

FILE *open_config_file(char *mode) {
  FILE *fp = fopen(CONFIG_FILE_PATH, mode);
  if (!fp && !streq(mode, "r")) {
    perror("Failed to open/create config file. ");
    exit(EXIT_FAILURE);
  }
  return fp;
}


void init(void) {

  config_t *config = alloc_config();
  printf("\n");

  // open config file for reading
  FILE *config_file = open_config_file("r");
  if (config_file == NULL) {
    // no config file exists, output message
    printf("No .config file found. ");
  } else {
    // config file exists, so read config from it
    //    return true (and populate config) if it is a valid config
    read_config(config_file, config);

    fclose(config_file);
  }

  printf("Enter config options below (or press ENTER to use previous values where given):\n");

  get_config(config);

  printf("Configuration updated successfully.\n");
  //print_config(config);

  // reopen config file for writing (to overwrite current contents)
  config_file = open_config_file("w");

  // write updated config to config file
  save_config(config_file, config);

  fclose(config_file);

}
