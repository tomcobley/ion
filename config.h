#ifndef CONFIG_H
#define CONFIG_H


#include <stdbool.h>

// the maximum length a value in the config file can take (some URLs may be long)
#define MAX_CONFIG_VALUE_LENGTH (2084)

// the maximum length a line in the config file can take
#define MAX_CONFIG_LINE_LENGTH (MAX_CONFIG_VALUE_LENGTH + 100)

// path to config file
#define CONFIG_FILE_PATH ("./.config")

// system recommended defaults
#define RECOMMENDED_MIN_CHARGE "30"
#define RECOMMENDED_MAX_CHARGE "75"

// command used to perform initialisation
#define INIT_COMMAND ("ion init")


// Note: whenever a config option is added, code in this file and config.c must
//    be updated in multiple places. These places are marked with a comment:
//       '//CONFIG_UPDATE'

//CONFIG_UPDATE
typedef struct config {
    char *str_charge_low_webhook_url;
    char *str_charge_high_webhook_url;
    int int_cycle_min_charge_percentage;
    int int_cycle_max_charge_percentage;
} config_t;

//CONFIG_UPDATE
#define CONFIG_OPTIONS_COUNT (4)

config_t *alloc_config(void);

void free_config(config_t *config);

void print_config(config_t *config);

bool read_config(FILE *config_file, config_t *config);

void init(void);

void set_config_option(config_t *config, const char *key, const char *value);

FILE *open_config_file(char *mode);

void save_config(FILE *config_file, config_t *config);

#endif
