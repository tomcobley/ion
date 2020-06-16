// the maximum length a value in the config file can take (some URLs may be long)
#define MAX_CONFIG_VALUE_LENGTH (2084)

// the maximum length a line in the config file can take
#define MAX_CONFIG_LINE_LENGTH (MAX_CONFIG_VALUE_LENGTH + 100)

// path to config file
#define CONFIG_FILE_PATH ("./.config")

// system recommended defaults
#define RECOMMENDED_MIN_CHARGE (40)
#define RECOMMENDED_MAX_CHARGE (80)


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
// Note: the order of the config file must be identical to the order of the
//    definition below,
//    and CONFIG_OPTIONS_COUNT must represent the number of config options in
//    this definition and the definition above.
#define CONFIG_OPTIONS_COUNT (4)
char *config_format[CONFIG_OPTIONS_COUNT] = {
  "str_charge_low_webhook_url",
  "str_charge_high_webhook_url",
  "int_cycle_min_charge_percentage",
  "int_cycle_max_charge_percentage"
};


config_t *alloc_config(void);

void free_config(config_t *config);

void print_config(config_t *config);

bool read_config(FILE *config_file, config_t *config);

void init(config_t *config);

void set_config_option(config_t *config, const char *key, const char *value);
void save_config(FILE *config_file, config_t *config);
