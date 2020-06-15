#define MACRO (textstr)

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


// the maximum length a value in the config file can take (some URLs may be long)
#define MAX_CONFIG_VALUE_LENGTH (2084)

// the maximum length a line in the config file can take
#define MAX_CONFIG_LINE_LENGTH (MAX_CONFIG_VALUE_LENGTH + 100)

#define CONFIG_FILE_PATH ("./.config")


#define CONFIG_OPTIONS_COUNT (2)

// Note: order of config options must be identical in the two definitions below,
//    and CONFIG_OPTIONS_COUNT must represent the number of config options in
//    the two definitions
// Note: all members of 'struct config' must be pointers

typedef struct config {
  char *charge_low_webhook_url;
  char *charge_high_webhook_url;
} config_t;

char *config_format[CONFIG_OPTIONS_COUNT] = {
  "charge_low_webhook_url",
  "charge_high_webhook_url"
};


// returns a pointer to a heap allocated struct of type config_t
//    all fields will be null
config_t *alloc_config(void) {
  config_t *config = calloc(1, sizeof(config_t));
  if (!config) {
    perror("Memory allocation failed for config. "); exit(EXIT_FAILURE);
  }

  uintptr_t config_struct_ptr = (void *) config;
  for (int i = 0; i < CONFIG_OPTIONS_COUNT; i++) {
    char *strptr = malloc(MAX_CONFIG_VALUE_LENGTH + 1);
    if (!strptr) {
      perror("Memory allocation failed for value. "); exit(EXIT_FAILURE);
    }
    *((char **) (config_struct_ptr + i * sizeof(char *))) = strptr;
  }
  return config;
}

// frees a config struct created by alloc_config
void free_config(config_t *config) {
  // TODO: implement
}


// reads supplied config file and returns true if the file is valid
//    (i.e. the file exists, is structured correctly and has been successfully
//     parsed to generate a config struct)
// if successful, the config values will be saved to the supplied struct
bool read_config(FILE *config_file, config_t *config) {

  // printf("config %x\n", (config) );
  // config->charge_low_webhook_url = "low-----------";
  // config->charge_high_webhook_url = "highkajshdflkashdflksadfhd";


  //
  // printf("addressA: %x\n", &(config->charge_low_webhook_url));
  // printf("addressB: %x\n", &(config->charge_high_webhook_url));
  // printf("valA: %x\n", config->charge_low_webhook_url);
  // printf("valB: %x\n", config->charge_high_webhook_url);
  //
  // printf("strA: %s\n", config->charge_low_webhook_url);
  // printf("strB: %s\n", config->charge_high_webhook_url);
  //
  // uintptr_t ptrA = config;
  // uintptr_t ptrB = ptrA + 8;
  //
  // printf("ptrA: %x\n", ptrA);
  // printf("ptrB: %x\n", ptrB);
  //
  // printf("strA: %s\n", *((char *) ptrA));
  // printf("strB: %s\n", *((char *) ptrB));




  int count = 0;
  char line_buffer[MAX_CONFIG_LINE_LENGTH + 1];
  uintptr_t config_struct_ptr = (void *) config;

  while (fgets(line_buffer, MAX_CONFIG_LINE_LENGTH, config_file)) {

    // tokenize at '=', line_buffer will point to key of line
    strtok(line_buffer, "=");
    if (strcmp(line_buffer, config_format[count]) != 0) {
      // key string is not correct according to config_format, so return false
      printf("incorrect string, expected %s, got %s\n", config_format[count], line_buffer);
      return false;
    }

    // key string is correct, so extract value
    //    (use NULL parameter to strtok to continue tokenizing line_buffer)
    strtok(NULL, ";");

    // line_buffer now points to value for this line; save this value to config
    strncpy((void *) (config_struct_ptr + (count * sizeof(char *))),
            line_buffer, MAX_CONFIG_VALUE_LENGTH);

    count++;
  }

  printf("line_buffer: %s\n", line_buffer);

  if (count < CONFIG_OPTIONS_COUNT) {
    // not enough options specified, return false
    printf("%s, count = %d\n", "not enough options", count);
    return false;
  }

  // the config file was successfully read and transferred to 'config'
  return true;
}

void init(config_t *config) {

  FILE *config_file = fopen(CONFIG_FILE_PATH, "r");
  if (config_file == NULL) {
    perror("Failed to open/create config file. "); exit(EXIT_FAILURE);
  }

  bool config_valid = read_config(config_file, config);

  if (config_valid) {
    printf("valid config\n");
    printf("%s\n", config->charge_low_webhook_url);
    printf("%s\n", config->charge_high_webhook_url);
  } else {
    printf("invalid config\n");
  }

}


int main(int argc, char const *argv[]) {

  printf("%d\n", MAX_CONFIG_LINE_LENGTH);

  config_t *config = alloc_config();

  init(config);

  free_config(config);


}
