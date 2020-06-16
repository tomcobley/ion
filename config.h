// the maximum length a value in the config file can take (some URLs may be long)
#define MAX_CONFIG_VALUE_LENGTH (2084)

// the maximum length a line in the config file can take
#define MAX_CONFIG_LINE_LENGTH (MAX_CONFIG_VALUE_LENGTH + 100)

// path to config file
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


config_t *alloc_config(void);

void free_config(config_t *config);

void print_config(config_t *config);

bool read_config(FILE *config_file, config_t *config);

void init(config_t *config);
