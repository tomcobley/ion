#define MACRO (textstr)

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


// the maximum length a value in the config file can take (some URLs may be long)
#define MAX_CONFIG_VALUE_LENGTH (2084)

// the maximum length a line in the config file can take
#define MAX_CONFIG_LINE_LENGTH (MAX_CONFIG_VALUE_LENGTH + 100)

#define CONFIG_FILE_PATH ("./.config")




// Note: whenever a config option is added, code in this file must be updated
//    in multiple places. These places are marked with a comment:
//       '//CONFIG_UPDATE'

//CONFIG_UPDATE
typedef struct config {
  char *charge_low_webhook_url;
  char *charge_high_webhook_url;
} config_t;

//CONFIG_UPDATE
// Note: the order of the config file must be identical to the order of the
//    definition below,
//    and CONFIG_OPTIONS_COUNT must represent the number of config options in
//    this definition and the definition above.
#define CONFIG_OPTIONS_COUNT (2)
char *config_format[CONFIG_OPTIONS_COUNT] = {
  "charge_low_webhook_url",
  "charge_high_webhook_url"
};


// returns a pointer to a heap allocated struct of type config_t
config_t *alloc_config(void) {
  config_t *config = malloc(sizeof(config_t));
  if (!config) {
    perror("Memory allocation failed for config. "); exit(EXIT_FAILURE);
  }

  return config;
}


// TODO: move to utils?
bool streq(const char *a, const char *b) {
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
static void set_config_option(config_t *config, char *key, char *value) {
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
bool read_config(FILE *config_file, config_t *config) {

  int count = 0;
  char line_buffer[MAX_CONFIG_LINE_LENGTH + 1];

  while (fgets(line_buffer, MAX_CONFIG_LINE_LENGTH, config_file)) {

    // tokenize at '=', line_buffer will point to key of line
    strtok(line_buffer, "=");
    if (!streq(line_buffer, config_format[count])) {
      // key string is not correct according to config_format, so return false
      printf("Incorrect string, expected %s, got %s\n", config_format[count], line_buffer);
      return false;
    }

    printf("line_buffer: %s\n", line_buffer);


    // key string is correct, so extract value
    //    (use NULL parameter to strtok to continue tokenizing line_buffer)
    char *value = strtok(NULL, ";");

    printf("val: %s\n", value);


    // line_buffer still points to key, val points to value for this line;
    //    save this value to relevant member in config
    set_config_option(config, line_buffer, value);

    count++;
  }


  if (count < CONFIG_OPTIONS_COUNT) {
    // not enough options specified, return false
    printf("%s, count = %d\n", "not enough options", count);
    return false;
  }

  // the config file was successfully read and transferred to 'config'
  return true;
}


static void update_config_option(char **dest, const char *prompt) {
  printf("%s:\n", prompt);
  if (*dest) {
    // *dest is not NULL, so present default option
    printf("- or press ENTER to use \"%s\"\n", *dest);
  }

  char response[MAX_CONFIG_VALUE_LENGTH];
  fgets(response, MAX_CONFIG_VALUE_LENGTH, stdin);
  // strip \n from response
  if (response[strlen(response - 1)] == '\n') {
    response[strlen(response - 1)] = '\0';
  }

  if (streq(response, "") && !*dest) {
    // empty response not allowed as no default exists, so ask again
    printf("%s\n", "Please enter a response: \n");
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
void get_config(config_t *config) {

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



void init(config_t *config) {

  FILE *config_file = fopen(CONFIG_FILE_PATH, "a+");
  if (config_file == NULL) {
    perror("Failed to open/create config file. "); exit(EXIT_FAILURE);
  }

  // rewind to start of file
  rewind(config_file);

  bool config_valid = read_config(config_file, config);

  if (config_valid) {
    printf("valid config\n");
    print_config(config);
  } else {
    printf("invalid config\n");
  }

  get_config(config);

  printf("Configuration updated successfully:\n");
  print_config(config);

}




int main(int argc, char const *argv[]) {

  config_t *config = alloc_config();

  init(config);

  free_config(config);



}
