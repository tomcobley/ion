#define SYSTEM_SUCCESS (0)
#define MAX_LINE_SIZE (1024)
#define CYCLE_LOWER_BOUND (30)
#define CYCLE_UPPER_BOUND (80)
#define BATTERY_INFO ("upower -i `upower -e | grep 'BAT'` | egrep 'state|percentage' > ./temp/batteryinfo.txt")
#define DELETE_TEMP_CONTENTS ("cd ./temp && rm -rf *")
#define BATTERY_INFO_PATH ("./temp/batteryinfo.txt")
#define BATTERY_LOG_PATH ("./log/batterylog.txt")
#define POWER_ON ("https://maker.ifttt.com/trigger/charge_low/with/key/dMgkSNQqbJM-9NI1ySrKdb")
#define POWER_OFF ("https://maker.ifttt.com/trigger/charge_high/with/key/dMgkSNQqbJM-9NI1ySrKdb")

typedef enum state {
  CHARGING,
  DISCHARGING
} state_t;

typedef struct battery {
  int percentage;
  state_t state;
} battery_t;
